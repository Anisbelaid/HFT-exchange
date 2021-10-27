#ifndef TRADES_TREE_VIEW_HPP
#define TRADES_TREE_VIEW_HPP

#include <gtkmm.h>
#include <gtkmm/liststore.h>

#include <Trading/Trade.hpp>

namespace broker {
    //Tree model columns:
    class ModelTrades: public Gtk::TreeModel::ColumnRecord
    {
        public:

        ModelTrades()
        { add(mTradeId); add(mStockId); add(mSide); add(mExecutedQty);add(mExecutedPrice);add(mExecutionId);}

        Gtk::TreeModelColumn<Glib::ustring> mTradeId;
        Gtk::TreeModelColumn<uint64_t>      mStockId;
        Gtk::TreeModelColumn<Glib::ustring> mSide;
        Gtk::TreeModelColumn<uint64_t>      mExecutedQty;
        Gtk::TreeModelColumn<uint64_t>      mExecutedPrice;
        Gtk::TreeModelColumn<uint64_t>      mExecutionId;
    };

    template <class Allocator = core::LinearAllocator>
    class TradesTreeViewPopup : public Gtk::TreeView
    {
        private :
            core::SessionManager         * mSessionManager{nullptr};
            core::Session                * mSession{nullptr};
            Allocator                    * mAllocator{nullptr};
            ModelTrades                  mColumns;
            Glib::RefPtr<Gtk::ListStore> mRefTreeModel;
            Gtk::Menu                    mMenuPopup;
             std::mutex                            mMutexTree;
            Glib::Dispatcher                      mDrawerDispatcher;
            sigc::connection                      conn;
        public:
            TradesTreeViewPopup(core::SessionManager *aSessionManger,Allocator *aAllocator):mSessionManager(aSessionManger),mSession (aSessionManger->getFeedSession()),mAllocator(aAllocator)
            {
                //Create the Tree model:
                mRefTreeModel = Gtk::ListStore::create(mColumns);
                set_model(mRefTreeModel);

                //Add the TreeView's view columns:
                append_column("ID", mColumns.mTradeId);
                append_column("Stock Id", mColumns.mStockId);
                append_column("Side", mColumns.mSide);
                append_column("Exec Qty", mColumns.mExecutedQty);
                append_column("Exec Price", mColumns.mExecutedPrice);
                append_column("Exec Id", mColumns.mExecutionId);

                //Fill popup menu:
                auto item = Gtk::make_managed<Gtk::MenuItem>("Cancel Trade", true);
                item->signal_activate().connect(
                sigc::mem_fun(*this, &TradesTreeViewPopup::onMenuSelected));
                mMenuPopup.append(*item);

                mMenuPopup.accelerate(*this);
                mMenuPopup.show_all(); //Show all menu items when the menu pops up

                  mDrawerDispatcher.connect(sigc::mem_fun(*this, &TradesTreeViewPopup::onNotificationFromWorkerThread));
                // This is where we connect the slot to the Glib::signal_timeout()
                sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,&TradesTreeViewPopup::on_timeout), 1);
                conn = Glib::signal_timeout().connect(my_slot,10);
                mSession->RegisterTrades([this](void *aMessage){this->OnNewTrades(aMessage);});
            };

            bool on_timeout(int timer_number)
        {
            std::lock_guard<std::mutex> lk(mMutexTree);
            mDrawerDispatcher.emit();
        }

        void onNotificationFromWorkerThread() {
            queue_draw();
        }
            virtual ~TradesTreeViewPopup()
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

            };

            void OnNewTrades (void *aMessage)
            {
                std::lock_guard<std::mutex> lk(mMutexTree);
                feed::Trade * lTrade = (feed::Trade*)aMessage;
                std::string lKey = std::to_string(lTrade->MATCHNUMBER);
                for (auto row: mRefTreeModel->children())
                {
                    if(*row)
                    {
                        Glib::ustring lId = (*row)[mColumns.mTradeId];
                        if (lKey==lId) return;
                    }
                }
                
                Gtk::TreeModel::Row row = *(mRefTreeModel->append());
                row[mColumns.mTradeId] = lKey;
                row[mColumns.mStockId] = lTrade->STOCK;
                row[mColumns.mExecutedQty] = lTrade->SHARES ;
                row[mColumns.mExecutedPrice] = lTrade->PRICE ;
                row[mColumns.mExecutionId] = lTrade->MATCHNUMBER ;

                queue_draw();
            }

    };
}
#endif