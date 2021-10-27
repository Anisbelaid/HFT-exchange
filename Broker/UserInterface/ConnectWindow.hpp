#ifndef BROKER_CONNECT_WINDOW_HPP
#define BROKER_CONNECT_WINDOW_HPP

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <UserInterface/OrderBottleView.hpp>
#include <UserInterface/ApprovalView.hpp>


/**
 * @brief The broker namespace 
 * 
 */
namespace broker {

  /**
   * @brief Order Container treemodel 
   * 
   */
  class BrokerModelColumns : public Gtk::TreeModel::ColumnRecord
  {
    public:
      /**
       * @brief Construct a new Broker Model Columns object
       * 
       */
      BrokerModelColumns()
      { add(mBrokerId); add(mBrokerName); add(mTCPAddress);add(mTCPInterface);add(mTCPPort);add(mClientType); }
      ///
      Gtk::TreeModelColumn<Glib::ustring> mBrokerId;
      ///
      Gtk::TreeModelColumn<Glib::ustring> mBrokerName;
      ///
      Gtk::TreeModelColumn<Glib::ustring> mTCPAddress;
      ///
      Gtk::TreeModelColumn<Glib::ustring> mTCPInterface;
      ///
      Gtk::TreeModelColumn<Glib::ustring> mTCPPort;
      ///
      Gtk::TreeModelColumn<Glib::ustring> mClientType;
  };
  /**
   * @brief a window that allow users to connect
   * 
   * @tparam Allocator 
   */
  template <class Allocator = core::LinearAllocator>
  class ConnectWindow : public core::UserWindow <Allocator>
  {
    typedef std::map <std::string,Gtk::Window*> SessionToWidgetMap_;
    /// alias to the base window
    using BaseWindow = core::UserWindow <Allocator>;
    ///
    SessionToWidgetMap_                 OrderBookViews;
    ///
    SessionToWidgetMap_                 ApprovalViews;
    ///
    Glib::RefPtr<Gtk::ListStore>        mRefTreeModel;
    ///
    BrokerModelColumns                  mColumns;
    ///
    Gtk::Table                          mTable;
    ///
    Gtk::Label                          mLabelCombo;
    ///
    Gtk::Label                          mLabelTCP;
    ///
    Gtk::Label                          mLabelTCPValue;
    ///
    Gtk::Label                          mLabelInterface;
    ///
    Gtk::Label                          mLabelInterfaceValue;
    ///
    Gtk::Label                          mLabelPort;
    ///
    Gtk::Label                          mLabelPortValue;
    ///
    Gtk::Label                          mClientType;
    ///
    Gtk::Label                          mClientTypeValue;
    ///
    Gtk::ComboBox                       mCombo;
    ///
    Gtk::Button                         mButtonConnect;
    ///
    Gtk::Button                         mButtonDisconnect;
    ///
    Gtk::MessageDialog                  mDialog ;
    
    public:
      /**
       * @brief Construct a new Connect Window object
       * 
       * @param aAllocator 
       * @param aStyle 
       * @param aSessionManager 
       * @param aCongurationManager 
       */
      ConnectWindow(Allocator*aAllocator,std::string aStyle,core::SessionManager *aSessionManager,core::Configuration *aCongurationManager):core::UserWindow <Allocator>("Connect Window",aStyle,aAllocator,aSessionManager,aCongurationManager),
      mTable(2, 2, true),mDialog(*this,"Info Message")
      {
        Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
        int lWidth = screen->get_width();
        int lHeight = screen->get_height();
        BaseWindow::set_default_size (lWidth/4,lHeight/4);
        BaseWindow::set_title("Client Connect");

        mRefTreeModel = Gtk::ListStore::create(mColumns);
        mCombo.set_model(mRefTreeModel);

        mCombo.pack_start(mColumns.mBrokerId);
        mCombo.pack_start(mColumns.mBrokerName);

        mLabelCombo.set_label("Choose a Broker");
        mLabelTCP.set_label("TCP address");
        mLabelInterface.set_label("TCP Interface");
        mLabelPort.set_label("TCP Port");
        mClientType.set_label("Client Type");
        mButtonConnect.set_label( "Connect" );
        mButtonDisconnect.set_label( "Disconnect" );
       
        mTable.attach(mLabelCombo,0,1,0,1);
        mTable.attach(mLabelTCP,0,1,1,2);
        mTable.attach(mLabelInterface,0,1,2,3);
        mTable.attach(mLabelPort,0,1,3,4);
        mTable.attach(mClientType,0,1,4,5);
        mTable.attach(mLabelTCPValue,1,2,1,2);
        mTable.attach(mLabelInterfaceValue,1,2,2,3);
        mTable.attach(mLabelPortValue,1,2,3,4);
        mTable.attach(mClientTypeValue,1,2,4,5);
        mTable.attach(mCombo,1,2,0,1);
        mTable.attach(mButtonConnect,2,3,1,3);
        mTable.attach(mButtonDisconnect,2,3,3,5);
        
        BaseWindow::add(mTable);

        BaseWindow::show_all_children();
        mCombo.signal_changed().connect( sigc::mem_fun(*this, &ConnectWindow::OnComboChanged) );
        mButtonConnect.signal_clicked().connect( sigc::mem_fun(*this, &ConnectWindow::OnConnect) );
        mButtonDisconnect.signal_clicked().connect( sigc::mem_fun(*this, &ConnectWindow::OnDisconnect) );
        PopulateClientConnection ();
      };

      /**
       * @brief populate the combox box with the user configured
       * 
       */
      void PopulateClientConnection ()
      {
          std::vector <BrokerSection*> lConf= dynamic_cast<broker::Configuration*>(BaseWindow::mConfigurationManager)->getBrokersConfiguration ();
          for (auto lElement : lConf)
          {
              Gtk::TreeModel::Row row = *(mRefTreeModel->append());
              row[mColumns.mBrokerId] = std::to_string(lElement->mIndex);
              row[mColumns.mBrokerName] = lElement->mBrokerName;
              row[mColumns.mTCPAddress] = lElement->mAddress;
              row[mColumns.mTCPInterface] = lElement->mInterface;
              row[mColumns.mTCPPort] = std::to_string(lElement->mPort);
              row[mColumns.mClientType] = lElement->mType;
          }

      }
      /**
       * @brief Destroy the Connect Window object
       * 
       */
      ~ConnectWindow() override {
        for (auto lIt : OrderBookViews)
        {
            delete lIt.second;
        }
        for (auto lIt : ApprovalViews)
        {
            delete lIt.second;
        }
        ApprovalViews.clear();
        OrderBookViews.clear();

      };

    private:

      /**
       * @brief 
       * 
       */
      void OnComboChanged()
      {
        const auto iter = mCombo.get_active();
        if(iter)
        {
          const auto row = *iter;
          if(row)
          {
            Glib::ustring lTCPAddress = row[mColumns.mTCPAddress];
            Glib::ustring lTCPInterface = row[mColumns.mTCPInterface];
            Glib::ustring lTCPPort = row[mColumns.mTCPPort];
            Glib::ustring lClientType = row[mColumns.mClientType];
            mLabelTCPValue.set_label(lTCPAddress);
            mLabelInterfaceValue.set_label(lTCPInterface);
            mLabelPortValue.set_label(lTCPPort);
            mClientTypeValue.set_label(lClientType);
          }
        }
      };
      /**
       * @brief 
       * 
       */
      void OnConnect()
      {
        const auto iter = mCombo.get_active();
        if(iter)
        {
          const auto row = *iter;
          if(row)
          {
            Glib::ustring lId = row[mColumns.mBrokerId];
            auto lIt =OrderBookViews.find (lId);
            if (lIt == OrderBookViews.end ())
            { 
              if (mClientTypeValue.get_label()=="Retail")
              {
                OrderBottleView<Allocator> *lOrderBook = nullptr;
                try
                {
                  lOrderBook = new OrderBottleView<Allocator>(BaseWindow::mAllocator,"../config/OrderBottle.css",BaseWindow::mConfigurationManager,BaseWindow::mSessionManager,lId);               
                }
                catch (std::string & aMessage)
                {
                  mDialog.set_secondary_text(aMessage.c_str());
                  mDialog.set_modal(true);
                  mDialog.signal_response().connect(sigc::hide(sigc::mem_fun(mDialog, &Gtk::Widget::hide)));
                  mDialog.run();
                  return ;
                }
                lOrderBook->show();
                OrderBookViews[lId]=lOrderBook;
              }
              else
              {
                OrderBottleView<Allocator> *lOrderBook = nullptr;
                try
                {
                  lOrderBook=new OrderBottleView<Allocator>(BaseWindow::mAllocator,"../config/OrderBottle.css",BaseWindow::mConfigurationManager,BaseWindow::mSessionManager,lId);               
                }
                catch (std::string &aMessage)
                {
                  mDialog.set_secondary_text(aMessage.c_str());
                  mDialog.set_modal(true);
                  mDialog.signal_response().connect(sigc::hide(sigc::mem_fun(mDialog, &Gtk::Widget::hide)));
                  mDialog.run();
                  return ;
                }
                lOrderBook->show();
                OrderBookViews[lId]=lOrderBook;
                ApprovalView<Allocator>*lApprovalView=new ApprovalView<Allocator>(BaseWindow::mAllocator,"../config/Approval.css",BaseWindow::mConfigurationManager,BaseWindow::mSessionManager,lId);                              
                lApprovalView->show();
                ApprovalViews[lId]=lApprovalView;
              }
            }
            else
            {
              if (mClientTypeValue.get_label()=="Retail")
              {
                lIt->second->show();
              }
              else
              {
                lIt->second->show();
                ApprovalViews.find (lId)->second->show();
              }              
              
            } 
          }
        }
      }; 
      /**
       * @brief 
       * 
       */
      void OnDisconnect()
      {
        const auto iter = mCombo.get_active();
        if(iter)
        {
          const auto row = *iter;
          if(row)
          {
            Glib::ustring lId = row[mColumns.mBrokerId];
            auto lIt =OrderBookViews.find (lId);
            auto lIt1=ApprovalViews.find (lId);
            if (lIt != OrderBookViews.end ())
            { 
              lIt->second->hide();
              delete lIt->second;
              OrderBookViews.erase(lIt);
            }
            else
            {
              mDialog.set_secondary_text("Session not connected");
              mDialog.set_modal(true);
              mDialog.signal_response().connect(sigc::hide(sigc::mem_fun(mDialog, &Gtk::Widget::hide)));
              mDialog.run();
              return;
            }
            if (lIt1 != ApprovalViews.end ())
            { 
              lIt1->second->hide();
              delete lIt1->second;
              ApprovalViews.erase(lIt1);
            }
            else
            {
              mDialog.set_secondary_text("Session not connected");
              mDialog.set_modal(true);
              mDialog.signal_response().connect(sigc::hide(sigc::mem_fun(mDialog, &Gtk::Widget::hide)));
              mDialog.run();
            }
          }
        }
      } 
  };    
}

#endif //GTKMM_EXAMPLE_RANGEWIDGETS_H