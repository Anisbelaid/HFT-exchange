#ifndef APPROVAL_TREE_VIEW_HPP
#define APPROVAL_TREE_VIEW_HPP

#include <gtkmm.h>
#include <gtkmm/liststore.h>

namespace broker {
    
    //Tree model columns:
    class ApprovalModelColumns : public Gtk::TreeModel::ColumnRecord
    {
        public:

        ApprovalModelColumns()
        { add(mStockId); add(mOrderId); add(mOrderQuantity); add(mOrderPrice);add(mOrderStatus);}

        Gtk::TreeModelColumn<uint64_t> mStockId;
        Gtk::TreeModelColumn<uint64_t> mOrderId;
        Gtk::TreeModelColumn<uint64_t> mOrderQuantity;
        Gtk::TreeModelColumn<uint64_t> mOrderPrice;
        Gtk::TreeModelColumn<Glib::ustring> mOrderStatus;
    };
    template <class Allocator = core::LinearAllocator>
    class ApprovalViewTreeViewPopup : public Gtk::TreeView
    {
        private:
        
            core::SessionManager         * mSessionManager{nullptr};
            core::Session                * mSession{nullptr};
            Allocator                    * mAllocator{nullptr};
            ApprovalModelColumns           mColumns;
            Glib::RefPtr<Gtk::ListStore>   mRefTreeModel;
            Gtk::Menu                      mMenuPopup;
            std::mutex                     mMutexTree;
        public:
            ApprovalViewTreeViewPopup(core::SessionManager *aSessionManger,Allocator *aAllocator,core::Session *aSession):mSessionManager(aSessionManger),mSession(aSession),mAllocator(aAllocator)
            {
                //Create the Tree model:
                mRefTreeModel = Gtk::ListStore::create(mColumns);
                set_model(mRefTreeModel);
                //Add the TreeView's view columns:
                append_column("Stock ID", mColumns.mStockId);
                append_column("Order Id", mColumns.mOrderId);
                append_column("Qty", mColumns.mOrderQuantity);
                append_column("Price", mColumns.mOrderPrice);
                append_column("Status", mColumns.mOrderStatus);

                //Fill popup menu:
                auto item = Gtk::make_managed<Gtk::MenuItem>("Approve", true);
                item->signal_activate().connect(
                sigc::mem_fun(*this, &ApprovalViewTreeViewPopup::onMenuSelected));
                mMenuPopup.append(*item);

                mMenuPopup.accelerate(*this);
                mMenuPopup.show_all(); 

                mSession->RegisterApproval([this](void *aMessage){this->OnOrderApproval(aMessage);});
                
            };
            virtual ~ApprovalViewTreeViewPopup()
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
                        ApproveOrder(iter);
                    }
                };
            }

            void ApproveOrder (Gtk::TreeModel::iterator iter )
            {
                std::lock_guard<std::mutex> lk(mMutexTree);
                trading::OrderApproved  *lApprove = new trading::OrderApproved ();
                uint64_t lId = (*iter)[mColumns.mStockId];
                uint64_t lOrderId = (*iter)[mColumns.mOrderId];
                lApprove->TYPE = 'Y';
                lApprove->STOCK = lId;
                lApprove->ORDERID = lOrderId;
                (*iter)[mColumns.mOrderStatus] = "A";
                queue_draw();
                dynamic_cast<BrokerSession *>(mSession)->push((void *)lApprove, sizeof(trading::OrderApproved));
            }
        public:
            void OnOrderApproval (void *aMessage)
            {
                std::lock_guard<std::mutex> lk(mMutexTree);
                trading::RequestApproval * lReply = (trading::RequestApproval*)aMessage;
                Gtk::TreeModel::Row row = *(mRefTreeModel->append());
                row[mColumns.mStockId] = lReply->STOCK;
                row[mColumns.mOrderId] = lReply->ORDERID;
                row[mColumns.mOrderQuantity] = lReply->SHARES;
                row[mColumns.mOrderPrice] = lReply->PRICE;
                row[mColumns.mOrderStatus] = "WA";
                queue_draw();
            }


    };
}
#endif