#ifndef BROKER_ORDER_BOOK_HPP
#define BROKER_ORDER_BOOK_HPP

#include <Session/BrokerSession.hpp>
#include <UserInterface/OrderBottleTreeView.hpp>

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <UserInterface/EntryBox.hpp>

/**
 * @brief The broker namespace 
 * 
 */
namespace broker
{
  class InstrumentModelColumns : public Gtk::TreeModel::ColumnRecord
  {
    public:
      /**
       * @brief Construct a new Broker Model Columns object
       * 
       */
      InstrumentModelColumns()
      { add(mInstrumentId); add(mInstrumentName);  }
      ///
      Gtk::TreeModelColumn<uint64_t> mInstrumentId;
      ///
      Gtk::TreeModelColumn<Glib::ustring> mInstrumentName;
  };

  /**
   * @brief a window that will contains User Orders and excutions
   * 
   * @tparam 
   */
  template <class Allocator = core::LinearAllocator>
  class OrderBottleView : public core::UserWindow <Allocator>
  {
    /// an alias to the base window
    using BaseWindow = core::UserWindow <Allocator>;

    const std::string                   cWindowName{"Order container"};
    /// a first widget container 
    Gtk::Box                            mTreeViewBox;
    /// a scrolled window that conatins the tree view
    Gtk::ScrolledWindow                 mScrolledWindow;
    /// a tree view that will contains the Orders
    OrderBottleTreeViewPopup<Allocator> *mTreeView{nullptr};
    /// a conatiners to the window butons 
    Gtk::ButtonBox                      mButtonBox;
    /// a button used to display the entry box
    Gtk::Button                         mButtonSendOrder;
    ///
    Gtk::ComboBox                       mComboInstrument;
        ///
    Glib::RefPtr<Gtk::ListStore>        mRefTreeModel;
    ///
    InstrumentModelColumns              mColumns;
    /// The trading session of the user
    std::string                         mUserIndex;
    /// A Pointer to the entry box
    core::UserWindow <Allocator>        *mWidnowEntryBox {nullptr};
    public:
      /**
       * @brief Construct a new Order Bottle View object
       * 
       * @param aAllocator a pointer to the main allocator
       * @param aStyle a path to the stle css file
       * @param aConfigurationManager a pointer to the configuration manager 
       * @param aSessionManager a pointer to the session manager
       * @param mUserIndex the User Index in xonfiguration file
       */
      OrderBottleView(Allocator*aAllocator,std::string aStyle,core::Configuration *aConfigurationManager,core::SessionManager* aSessionManager,std::string aUserIndex):core::UserWindow <Allocator>("Order Bottle",aStyle,aAllocator,aSessionManager,aConfigurationManager), 
      mButtonBox(Gtk::ORIENTATION_VERTICAL),mButtonSendOrder("Send Order"),mUserIndex(aUserIndex)
      {
        Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
        int lWidth = screen->get_width();
        int lHeight = screen->get_height();
        BaseWindow::set_default_size (lWidth/2,lHeight/2);
        BaseWindow::set_title(cWindowName+" User :"+getBrokerName());

        BrokerSession*lSession = dynamic_cast<BrokerSession*>(BaseWindow::mSessionManager->GetTCPClientSessionById(mUserIndex));
        if (lSession)
        {
          if (lSession->prepare ())
          {
              lSession->open(std::to_string(lSession->getSessionName())); 
              lSession->Sendlogon ();
          }
          if (lSession->IsConnected())
          {
            mTreeView = new OrderBottleTreeViewPopup<Allocator>(aSessionManager,aAllocator,lSession);
          }
          else
          {
            throw std::string("cannot connect Session");
          }
        }
        else
        {
          throw std::string("Session not found");
        }

        BaseWindow::add(mTreeViewBox);
        mScrolledWindow.add(*mTreeView);
        mScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        mTreeViewBox.pack_start(mScrolledWindow);
        mTreeViewBox.pack_start(mButtonBox, Gtk::PACK_SHRINK);

        mRefTreeModel = Gtk::ListStore::create(mColumns);
        mComboInstrument.set_model(mRefTreeModel);

        mComboInstrument.pack_start(mColumns.mInstrumentId);
        mComboInstrument.pack_start(mColumns.mInstrumentName);

        mButtonBox.pack_start(mButtonSendOrder);
        mButtonBox.pack_start(mComboInstrument);
        mButtonBox.set_border_width(5);

        BaseWindow::mSessionManager->getFeedSession ()->RegisterInstrument([this](void *aMessage){this->OnNewInstrument(aMessage);});
        mButtonSendOrder.signal_clicked().connect( sigc::mem_fun(*this,&OrderBottleView::OnSendOrder) );

        BaseWindow::show_all_children();


      }
      /**
       * @brief Destroy the Order Bottle View object
       * 
       */
      ~OrderBottleView ()
      {
        delete mTreeView;
        if (mWidnowEntryBox)
        {
          delete mWidnowEntryBox;
        }
      }
      /**
       * @brief 
       * 
       */
      void onNewOrder  (trading::Order *aOrder){
        mTreeView->OnNewOrder(aOrder);
      };
      /**
       * @brief 
       * 
       */
      void onCancelOrder  (trading::Cancel *aCancel){
        mTreeView->OnCancelOrder(aCancel);
      };
      /**
       * @brief 
       * 
       */
      void OnSendOrder ()
      {
         const auto iter = mComboInstrument.get_active();
          if(iter)
          {
            const auto row = *iter;
            if(row)
            {
              uint64_t lId = row[mColumns.mInstrumentId];
              Glib::ustring lName = row[mColumns.mInstrumentName];
              if (mWidnowEntryBox)
              {
                delete mWidnowEntryBox;
              }
              mWidnowEntryBox = new EntryBox<Allocator>(lId,lName,"../config/EntryBox.css",BaseWindow::mAllocator,BaseWindow::mConfigurationManager,BaseWindow::mSessionManager);
              mWidnowEntryBox->registerObserver(this);
              mWidnowEntryBox->notifyObservers(core::Event::NewWindow,mWidnowEntryBox);
            }
          }
      }

      /**
       * @brief 
       * 
       * @param aInstrument 
       */
      void OnNewInstrument (void*aInstrument)
      {
          feed::Instrument * lInst = (feed::Instrument*)aInstrument;
          uint64_t lKey = lInst->STOCK;
          for (auto row: mRefTreeModel->children())
          {
              if(*row)
              {
                  uint64_t lId = (*row)[mColumns.mInstrumentId];
                  if (lKey==lId) return;
              }
          }
          std::string lSymbol(lInst->SYMBOL,8);
          Gtk::TreeModel::Row row = *(mRefTreeModel->append());
          row[mColumns.mInstrumentId] = lKey;
          row[mColumns.mInstrumentName] = lSymbol;
      }

      /**
       * @brief Get the Broker Name object
       * 
       * @return const std::string& 
       */
      const std::string& getBrokerName ()
      {
        std::vector <BrokerSection*> lConf= dynamic_cast<broker::Configuration*>(BaseWindow::mConfigurationManager)->getBrokersConfiguration ();
        for (auto lElement : lConf)
        {
          if (std::to_string (lElement->mIndex)==mUserIndex) return lElement->mBrokerName;
        }
        return "";

      } 
   };

}
#endif 