#ifndef BASKET_TREE_VIEW_TREE_VIEW_HPP
#define BASKET_TREE_VIEW_TREE_VIEW_HPP

#include <gtkmm.h>
#include <gtkmm/liststore.h>
#include <unistd.h>
#include <atomic>
namespace broker {
    


    //Tree model columns:
    class BasketModelColumns : public Gtk::TreeModel::ColumnRecord
    {
        public:

        BasketModelColumns()
        { add(mAction);add(mStockId); add(mSide); add(mOrderQuantity); add(mOrderPrice);add(mOrderStatus);add(mOrderId);add(mOrderRef);add(mEventId);}
        Gtk::TreeModelColumn<Glib::ustring> mAction;
        Gtk::TreeModelColumn<uint64_t>      mStockId;
        Gtk::TreeModelColumn<Glib::ustring> mSide;
        Gtk::TreeModelColumn<uint64_t>      mOrderQuantity;
        Gtk::TreeModelColumn<uint64_t>      mOrderPrice;
        Gtk::TreeModelColumn<Glib::ustring> mOrderStatus;
        Gtk::TreeModelColumn<uint64_t>      mOrderId;
        Gtk::TreeModelColumn<uint64_t>      mOrderRef;
        Gtk::TreeModelColumn<uint64_t>      mEventId;
    };

    template <class Allocator = core::LinearAllocator>
    class BasketTreeView : public Gtk::TreeView
    {
    protected:
        
        core::SessionManager         * mSessionManager{nullptr};
        Allocator                    * mAllocator{nullptr};
        BasketModelColumns             mColumns;
        Glib::RefPtr<Gtk::ListStore>   mRefTreeModel;
        Gtk::Menu                      mMenuPopup;
        std::atomic_uint64_t           mNumberAction{0};
        std::unordered_map <uint64_t,Gtk::TreeRow>   mOrdersIds;
        std::mutex                     mMutexTree;
        Glib::Dispatcher               mDrawerDispatcher;
        sigc::connection               conn;

    public:
        BasketTreeView(core::SessionManager *aSessionManger,Allocator *aAllocator):mSessionManager(aSessionManger),mAllocator(aAllocator)
        {
            //Create the Tree model:
            mRefTreeModel = Gtk::ListStore::create(mColumns);
            set_model(mRefTreeModel);
            //Add the TreeView's view columns:
            append_column("Action", mColumns.mAction);
            append_column("Stock ID", mColumns.mStockId);
            append_column("Buy/Sell", mColumns.mSide);
            append_column("Qty", mColumns.mOrderQuantity);
            append_column("Price", mColumns.mOrderPrice);
            append_column("Status", mColumns.mOrderStatus);
            append_column("Order ID", mColumns.mOrderId);
            append_column("Order Ref", mColumns.mOrderRef);
            append_column("Event ID", mColumns.mEventId);
            //Fill popup menu:
            auto item = Gtk::make_managed<Gtk::MenuItem>("Remove", true);
            item->signal_activate().connect(sigc::mem_fun(*this, &BasketTreeView::onMenuSelected));
            mDrawerDispatcher.connect(sigc::mem_fun(*this, &BasketTreeView::onNotificationFromWorkerThread));
            mMenuPopup.append(*item);
            mMenuPopup.accelerate(*this);
            mMenuPopup.show_all(); 

            sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
              &BasketTreeView::on_timeout), 1);

        // This is where we connect the slot to the Glib::signal_timeout()
           conn = Glib::signal_timeout().connect(my_slot,
                10);
        };
        
        bool on_timeout(int timer_number)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            mDrawerDispatcher.emit();
        }

        void onNotificationFromWorkerThread() {
            queue_draw();
        }

        void onOrderAdded (const std::string& aEventType,const uint64_t& aStockId,const std::string&aSide,const uint64_t& aQty,const uint64_t& aPrice,const uint64_t& aOrderId,const uint64_t& aEventId)
        {
            Gtk::TreeModel::Row row = *(mRefTreeModel->append());
            row[mColumns.mAction]= aEventType;
            row[mColumns.mStockId] = aStockId;
            row[mColumns.mSide] = aSide;
            row[mColumns.mOrderQuantity] = aQty;
            row[mColumns.mOrderPrice] = aPrice;
            row[mColumns.mOrderStatus] = "Ready";
            row[mColumns.mOrderRef] = aOrderId;
            row[mColumns.mEventId] = aEventId;
            queue_draw();
            mNumberAction++;
        }
        
        Gtk::TreeModel::iterator getOrderById (const uint64_t&aId)
        {
            for (auto row: mRefTreeModel->children())
            {
                if(*row)
                {
                    uint64_t lId = (*row)[mColumns.mOrderRef];
                    if (aId==lId) return row;
                }
            }
        }

        Gtk::TreeModel::iterator getOrderByEventId (const uint64_t&aId)
        {
            for (auto row: mRefTreeModel->children())
            {
                if(*row)
                {
                    uint64_t lId = (*row)[mColumns.mEventId];
                    if (aId==lId) return row;
                }
            }
        }

        Gtk::TreeModel::iterator getOrderByOrderId (const uint64_t&aId)
        {
            for (auto row: mRefTreeModel->children())
            {
                if(*row)
                {
                    uint64_t lId = (*row)[mColumns.mOrderId];
                    if (aId==lId) return row;
                }
            }
        }

        const uint64_t getNumberAction () const {return mNumberAction.load (std::memory_order_release);}

        virtual ~BasketTreeView()
        {

        };

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
                    mRefTreeModel->erase(iter);
                }
            };
            queue_draw();
        }

        void onClear()
        {
            mRefTreeModel->clear();

        }

        void onOrderAccepted (void*aMessage)
        {   
            std::lock_guard<std::mutex> lk(mMutexTree);
            trading::Accepted * lReply=(trading::Accepted *)aMessage;
            auto row = mOrdersIds[lReply->CLIENTORDERID];
            row[mColumns.mOrderStatus] = "Accpeted";
            row[mColumns.mOrderId] = lReply->ORDERID;
        }

        void onOrderRejected (void*aMessage)
        {   
            std::lock_guard<std::mutex> lk(mMutexTree);
            trading::Reject * lReject = (trading::Reject*)aMessage;
            auto row = mOrdersIds[lReject->CLIENTORDERID];
            row[mColumns.mOrderStatus] = "Rejected";

        }

        void OnOrderCanceled (void*aMessage)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            trading::Canceled * lCanceled = (trading::Canceled*)aMessage;
            auto row = mOrdersIds[lCanceled->CLIENTORDERID];
            row[mColumns.mOrderStatus] = "Canceled";
        }

       void onSendBasket (BrokerSession *aSession)
       {
            Gtk::TreeModel::iterator row=  mRefTreeModel->children().begin();
            while (row!=mRefTreeModel->children().end())
            {
                mMutexTree.lock();
                Glib::ustring lAction = (*row)[mColumns.mAction];
                if (lAction=="Initial" ||lAction=="Place" ||lAction=="Fill")
                {
                    trading::Order * lOrder =new trading::Order();//(trading::Order *) mAllocator->Allocate (sizeof (trading::Order));
                    lOrder->TYPE='O';
                    lOrder->SHARES        = (*row)[mColumns.mOrderQuantity];
                    lOrder->PRICE         = (*row)[mColumns.mOrderPrice];
                    lOrder->BUYSELL       = ((*row)[mColumns.mSide]=="buy"?'B':'S');
                    lOrder->STOCK         = (*row)[mColumns.mStockId];
                    lOrder->CLIENTORDERID = (*row)[mColumns.mOrderRef];
                    (*row)[mColumns.mOrderStatus] = "Sent";
                    lOrder->ORDERTYPE  =trading::OrderType::Broker;
                    uint64_t lOrderRef = (*row)[mColumns.mOrderRef];
                    std::pair<std::unordered_map <uint64_t,Gtk::TreeRow>::iterator,bool> lResult = mOrdersIds.insert (std::make_pair(lOrderRef,*row));
                    aSession->push((void *)lOrder, sizeof(trading::Order));
                    
                }
                else
                {
                    trading::Cancel * lCancel = new trading::Cancel(); //(trading::Cancel *) mAllocator->Allocate (sizeof (trading::Cancel));
                    lCancel->TYPE='C';
                    lCancel->STOCK   = (*row)[mColumns.mStockId];
                    uint64_t lOrderRef = (*row)[mColumns.mOrderRef];
                    while (mOrdersIds[lOrderRef][mColumns.mOrderStatus]=="Sent") {
                        mMutexTree.unlock();
                        mMutexTree.lock();
                    }
                    lCancel->ORDERID = mOrdersIds[lOrderRef][mColumns.mOrderId];
                    lCancel->CLIENTORDERID = (*row)[mColumns.mEventId];
                    if (lCancel->CLIENTORDERID==0 || lCancel->ORDERID==0) return;
                    (*row)[mColumns.mOrderStatus] = "Sent";
                    mOrdersIds.insert (std::make_pair((*row)[mColumns.mEventId],*row));
                    aSession->push((void *)lCancel, sizeof(trading::Cancel));
                }
                usleep(1);
                mNumberAction--;
                row++;
                mMutexTree.unlock();
            }
       }
    };
}
#endif