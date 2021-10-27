#ifndef ORDERBOOK_TREE_VIEW_HPP
#define ORDERBOOK_TREE_VIEW_HPP

#include <gtkmm.h>
#include <gtkmm/liststore.h>

namespace broker {
    
    //Tree model columns:
    class ModelColumns : public Gtk::TreeModelFilter::ColumnRecord
    {
        public:

        ModelColumns()
        { add(mStockId); add(mOrderId); add(mOrderQuantity); add(mOrderPrice);add(mClientOrderId);add(mOrderStatus);}

        Gtk::TreeModelColumn<uint64_t> mStockId;
        Gtk::TreeModelColumn<uint64_t> mOrderId;
        Gtk::TreeModelColumn<uint64_t> mOrderQuantity;
        Gtk::TreeModelColumn<uint64_t> mOrderPrice;
        Gtk::TreeModelColumn<uint64_t> mClientOrderId;
        Gtk::TreeModelColumn<Glib::ustring> mOrderStatus;
    };

    template <class Allocator = core::LinearAllocator>
    class OrderBottleTreeViewPopup : public Gtk::TreeView
    {
    protected:
        
        core::SessionManager                  * mSessionManager{nullptr};
        core::Session                         * mSession{nullptr};
        Allocator                             * mAllocator{nullptr};
        ModelColumns                          mColumns;
        Glib::RefPtr<Gtk::ListStore>          mRefTreeModel;
        Glib::RefPtr<Gtk::TreeModelFilter>    mRefTreeModelFilter;
        Gtk::Menu                             mMenuPopup;
        std::mutex                            mMutexTree;
        Glib::Dispatcher                      mDrawerDispatcher;
        sigc::connection                      conn;
        std::unordered_map <uint64_t,Gtk::TreeRow>   mOrdersIds;
    public:
        OrderBottleTreeViewPopup(core::SessionManager *aSessionManger,Allocator *aAllocator,core::Session *aSession):mSessionManager(aSessionManger),mSession(aSession),mAllocator(aAllocator)
        {
            //Create the Tree model:
            mRefTreeModel = Gtk::ListStore::create(mColumns);
            mRefTreeModelFilter = Gtk::TreeModelFilter::create(mRefTreeModel);
            set_model(mRefTreeModel);
            mRefTreeModelFilter->set_visible_func(sigc::mem_fun(*this, &OrderBottleTreeViewPopup<Allocator>::OnFilter)); 
            
            set_model(mRefTreeModelFilter);
            //Add the TreeView's view columns:
            append_column("Stock ID", mColumns.mStockId);
            append_column("Order Id", mColumns.mOrderId);
            append_column("Qty", mColumns.mOrderQuantity);
            append_column("Price", mColumns.mOrderPrice);
            append_column("Client Order Id", mColumns.mClientOrderId);
            append_column("Status", mColumns.mOrderStatus);

            //Fill popup menu:
            auto item = Gtk::make_managed<Gtk::MenuItem>("Cancel Order", true);
            item->signal_activate().connect(
            sigc::mem_fun(*this, &OrderBottleTreeViewPopup::onMenuSelected));
            mMenuPopup.append(*item);

            mMenuPopup.accelerate(*this);
            mMenuPopup.show_all();
           
            mDrawerDispatcher.connect(sigc::mem_fun(*this, &OrderBottleTreeViewPopup::onNotificationFromWorkerThread));
            // This is where we connect the slot to the Glib::signal_timeout()
            sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,&OrderBottleTreeViewPopup::on_timeout), 1);
            conn = Glib::signal_timeout().connect(my_slot,10);
            mSession->RegisterOrdeRejected([this](void *aMessage){this->OnOrderRejected(aMessage);});
            mSession->RegisterOrderAccecpted([this](void *aMessage){this->OnOrderAccepted(aMessage);});
        };

        bool on_timeout(int timer_number)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            mDrawerDispatcher.emit();
        }

        void onNotificationFromWorkerThread() {
            queue_draw();
        }

        bool OnFilter (const Gtk::TreeModel::const_iterator& iter)
        {
           return true;
        }

        virtual ~OrderBottleTreeViewPopup()
        {

        };

    protected:
    
        // Override Signal handler:
        // Alternatively, use signal_button_press_event().connect_notify()
        bool on_button_press_event(GdkEventButton* button_event) override
        {
            bool return_value = false;

            //Call base class, to allow normal handling,
            //such as allowing the row to be selected by the right-click:
            return_value = TreeView::on_button_press_event(button_event);

            //Then do our custom stuff:
            if( (button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3) )
            {
                mMenuPopup.popup_at_pointer((GdkEvent*)button_event);
            }

            return return_value;
        };

        //Signal handler for popup menu items:
        void onMenuSelected()
        {
            auto refSelection = get_selection();
            if(refSelection)
            {
                Gtk::TreeModel::iterator iter = refSelection->get_selected();
                if(iter)
                {
                    CancelOrder(iter);
                }
            };
        }

        void CancelOrder (Gtk::TreeModel::iterator iter )
        {
            trading::Cancel  * lCancel =(trading::Cancel *) mAllocator->Allocate (sizeof (trading::Cancel ));
            uint64_t lId = (*iter)[mColumns.mStockId];
            uint64_t lOrderId = (*iter)[mColumns.mOrderId];
            lCancel->TYPE = 'C';
            lCancel->STOCK = lId;
            lCancel->ORDERID = lOrderId;
            dynamic_cast<BrokerSession *>(mSession)->push((void *)lCancel, sizeof(trading::Cancel));
        }
    public:
        void OnNewOrder(trading::Order *aOrder)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            Gtk::TreeModel::Row row = *(mRefTreeModel->append());
            row[mColumns.mStockId] = aOrder->STOCK;
            row[mColumns.mOrderQuantity] = aOrder->SHARES;
            row[mColumns.mOrderPrice] = aOrder->PRICE;
            row[mColumns.mClientOrderId] = aOrder->CLIENTORDERID;
            row[mColumns.mOrderStatus] = "Sent";
            mOrdersIds.insert (std::make_pair(aOrder->CLIENTORDERID,*row));
        }

        void OnCancelOrder (trading::Cancel *aOrder)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            Gtk::TreeModel::Row row = mOrdersIds[aOrder->CLIENTORDERID];
            row[mColumns.mOrderStatus] = "Canceled";
        }

        void OnOrderRejected (void *aMessage)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            trading::Reject * lReply = (trading::Reject*)aMessage;
            auto lIt = mOrdersIds.find (lReply->CLIENTORDERID);
            if (lIt == mOrdersIds.end()) return;
            Gtk::TreeModel::Row row = mOrdersIds[lReply->CLIENTORDERID];
            row[mColumns.mOrderId] = lReply->ORDERID;
            row[mColumns.mOrderStatus] = "Rejected";
        }

        void OnOrderAccepted (void *aMessage)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            trading::Reject * lReply = (trading::Reject*)aMessage;
            auto lIt = mOrdersIds.find (lReply->CLIENTORDERID);
            if (lIt == mOrdersIds.end()) return;
            Gtk::TreeModel::Row row = mOrdersIds[lReply->CLIENTORDERID];
            row[mColumns.mOrderId] = lReply->ORDERID;
            row[mColumns.mOrderStatus] = "Accepetd";
        }

    };
}
#endif