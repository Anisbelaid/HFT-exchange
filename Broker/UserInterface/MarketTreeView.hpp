#ifndef MARKET_TREE_VIEW_HPP
#define MARKET_TREE_VIEW_HPP

#include <gtkmm.h>
#include <gtkmm/liststore.h>

#include <UserInterface/EntryBox.hpp>

namespace broker {
    //Tree model columns:
    class ModelMarketWatch: public Gtk::TreeModel::ColumnRecord
    {
        public:

        ModelMarketWatch()
        { add(mStockId); add(mStockName); }

        Gtk::TreeModelColumn<Glib::ustring> mStockId;
        Gtk::TreeModelColumn<Glib::ustring> mStockName;
    };

    template <class Allocator = core::LinearAllocator>
    class MarketTreeViewPopup : public Gtk::TreeView
    {
        private :
            core::SessionManager         * mSessionManager{nullptr};
            core::Configuration          * mConfigurationManager{nullptr};
            core::Session                * mSession{nullptr};
            Allocator                    * mAllocator{nullptr};
            ModelMarketWatch             mColumns;
            Glib::RefPtr<Gtk::ListStore> mRefTreeModel;
            Gtk::Menu                    mMenuPopup;
        public:
            MarketTreeViewPopup(core::SessionManager *aSessionManger,core::Configuration *aConfigurationManager,Allocator *aAllocator):mSessionManager(aSessionManger),mConfigurationManager(aConfigurationManager),mSession (aSessionManger->getFeedSession()),mAllocator(aAllocator)
            {
                //Create the Tree model:
                mRefTreeModel = Gtk::ListStore::create(mColumns);
                set_model(mRefTreeModel);

                //Add the TreeView's view columns:
                append_column("ID", mColumns.mStockId);
                append_column("Name", mColumns.mStockName);

                //Fill popup menu:
                auto item = Gtk::make_managed<Gtk::MenuItem>("Send Order", true);
                item->signal_activate().connect(
                sigc::mem_fun(*this, &MarketTreeViewPopup::onMenuSelected));
                mMenuPopup.append(*item);

                mMenuPopup.accelerate(*this);
                mMenuPopup.show_all(); //Show all menu items when the menu pops up

                mSession->RegisterInstrument([this](void *aMessage){this->OnNewInstrument(aMessage);});
            };
            virtual ~MarketTreeViewPopup()
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
                        //Glib::ustring id = (*iter)[mColumns.mStockId];
                        //EntryBox<Allocator> *lEntryBox = new EntryBox<Allocator>(id,"../config/EntryBox.css",mAllocator,mConfigurationManager,mSessionManager);
                        //lEntryBox->show();
                    }
                };

            };

            void OnNewInstrument (void *aMessage)
            {
                feed::Instrument * lInst = (feed::Instrument*)aMessage;
                std::string lKey = std::to_string(lInst->STOCK);
                std::string lSymbol(lInst->SYMBOL,8);
                for (auto row: mRefTreeModel->children())
                {
                    if(*row)
                    {
                        Glib::ustring lId = (*row)[mColumns.mStockId];
                        if (lKey==lId) return;
                    }
                }
                
                Gtk::TreeModel::Row row = *(mRefTreeModel->append());
                row[mColumns.mStockId] = lKey;
                row[mColumns.mStockName] = lSymbol;
                queue_draw();
            }

    };
}
#endif