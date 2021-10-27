#ifndef BROKER_ENTRY_BOX_WINDOW_HPP
#define BROKER_ENTRY_BOX_WINDOW_HPP

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>

#include <Trading/Order.hpp>

namespace broker
{
  class ModelEntryBoxColumns : public Gtk::TreeModel::ColumnRecord
  {
    public:

      ModelEntryBoxColumns()
      { add(mBrokerId); add(mBrokerName); }

      Gtk::TreeModelColumn<uint64_t> mBrokerId;
      Gtk::TreeModelColumn<Glib::ustring> mBrokerName; 
  };
  template <class Allocator = core::LinearAllocator>
  class EntryBox : public core::UserWindow <Allocator>
  {
    private :
    using BaseWindow = core::UserWindow <Allocator>;
      Gtk::Table                   mTable;
      Glib::RefPtr<Gtk::ListStore> mRefTreeModel;
      ModelEntryBoxColumns         mColumns;
      Gtk::Label                   mLConnectionStockId;
      Gtk::ComboBox                mConnection;
      Gtk::Label                   mLabelStockId;
      Gtk::Entry                   mTextStockId;
      Gtk::Label                   mLabelOrderQty;
      Gtk::Entry                   mTextOrderQty;
      Gtk::Label                   mLabelOrderPrice;
      Gtk::Entry                   mTextOrderPrice;
      Gtk::Label                   mLabelBrokerId;
      Gtk::Entry                   mTextBrokerId;
      Gtk::Label                   mLabelClientOrderId;
      Gtk::Entry                   mTextClientOrderId;
      Gtk::Button                  mButtonBuy;
      Gtk::Button                  mButtonSell;
      uint64_t                     mStockId{0};

  public:
    EntryBox(uint64_t aStockId,std::string aStockName,std::string aStyle,Allocator*aAllocator,core::Configuration *aConfigurationManager,core::SessionManager* aSessionManager):core::UserWindow <Allocator>("Order Bottle",aStyle,aAllocator,aSessionManager,aConfigurationManager),  
    mTable(2, 2, true),mStockId(aStockId)
    {
      // Init the window
      BaseWindow::set_title("Connect Window");
      //set_default_size( 800, 600 );
      BaseWindow::set_border_width(10);

      mRefTreeModel = Gtk::ListStore::create(mColumns);
      mConnection.set_model(mRefTreeModel);
      mConnection.pack_start(mColumns.mBrokerId);
      mConnection.pack_start(mColumns.mBrokerName);

      mLConnectionStockId.set_label("Connection");
      mLabelStockId.set_label("Stock Id");
      mLabelOrderQty.set_label("Qty");
      mLabelOrderPrice.set_label("Price");
      mLabelBrokerId.set_label("Broker Id");
      mLabelClientOrderId.set_label("Client Order Id");
      mButtonBuy.set_label("Buy");
      mButtonSell.set_label("Sell");


      mTextStockId.set_text(aStockName);
      mTextStockId.set_editable(false);
      PopulateClientConnection ();
      mButtonBuy.signal_clicked().connect(sigc::mem_fun(*this, &EntryBox::SendBuyOrder));
      mButtonSell.signal_clicked().connect(sigc::mem_fun(*this, &EntryBox::SendSellOrder));

      mTable.attach(mLConnectionStockId, 0, 1, 0, 1, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      mTable.attach(mConnection, 1, 2, 0, 1, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);

      mTable.attach(mLabelStockId, 0, 1, 1, 2, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      mTable.attach(mTextStockId, 1, 2, 1, 2, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      
      mTable.attach(mLabelOrderQty, 0, 1, 2, 3, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      mTable.attach(mTextOrderQty, 1, 2, 2, 3, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);

      mTable.attach(mLabelOrderPrice, 0, 1, 3, 4, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      mTable.attach(mTextOrderPrice, 1, 2, 3, 4, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);

      mTable.attach(mLabelBrokerId, 0, 1, 4, 5, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      mTable.attach(mTextBrokerId, 1, 2, 4, 5, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);

      mTable.attach(mLabelClientOrderId, 0, 1, 5, 6, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      mTable.attach(mTextClientOrderId, 1, 2, 5, 6, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);

      mTable.attach(mButtonBuy, 0, 1, 6, 7, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      mTable.attach(mButtonSell, 1, 2, 6, 7, Gtk::AttachOptions::SHRINK, Gtk::AttachOptions::SHRINK);
      //Add the ComboBox to the window.
      BaseWindow::add(mTable);

      BaseWindow::show_all_children();
      //Connect signal handler:
      //mCombo.signal_changed().connect( sigc::mem_fun(*this, &ConnectWindow::OnComboChanged) );
      //mButton.signal_clicked().connect( sigc::mem_fun(*this, &ConnectWindow::OnConnect) );
    };

    ~EntryBox() override{};

  private:
      void PopulateClientConnection ()
      {
        std::vector <BrokerSection*> lConf= dynamic_cast<broker::Configuration*>(BaseWindow::mConfigurationManager)->getBrokersConfiguration ();
        for (auto lElement : lConf)
        {
            Gtk::TreeModel::Row row = *(mRefTreeModel->append());
            row[mColumns.mBrokerId] = lElement->mIndex;
            row[mColumns.mBrokerName] = lElement->mBrokerName;
        }
      }

      BrokerSession* GetActiveSession()
      {
        const auto iter = mConnection.get_active();
        if(iter)
        {
          const auto row = *iter;
          if(row)
          {
            uint64_t lId = row[mColumns.mBrokerId];
            return dynamic_cast <BrokerSession*>(BaseWindow::mSessionManager->GetTCPClientSessionById(std::to_string(lId)));
          }
        };
      }

    void SendBuyOrder()
    {
      trading::Order * lOrder =(trading::Order *) BaseWindow::mAllocator->Allocate (sizeof (trading::Order));
      lOrder->TYPE = 'O';
      lOrder->SHARES = atoi(mTextOrderQty.get_text().c_str());
      lOrder->PRICE = atoi(mTextOrderPrice.get_text().c_str());;
      lOrder->BUYSELL = 'B';
      lOrder->STOCK = mStockId;
      lOrder->CLIENTORDERID= atoi(mTextClientOrderId.get_text().c_str());
      std::string lDestination = mTextBrokerId.get_text();
      lOrder->ORDERTYPE  =(lDestination.empty()?trading::OrderType::Broker:trading::OrderType::Retail);
      BrokerSession * lSession= GetActiveSession();
      strcpy(lOrder->USER,lSession->getUser().c_str());
      strcpy(lOrder->BROKER,mTextBrokerId.get_text().c_str());
      BaseWindow::notifyObservers(core::Event::NewOrder,lOrder);
      lSession->push((void *)lOrder, sizeof(trading::Order));
    }

    void SendSellOrder()
    {
      trading::Order * lOrder =(trading::Order *) BaseWindow::mAllocator->Allocate (sizeof (trading::Order));
      lOrder->TYPE = 'O';
      lOrder->SHARES = atoi(mTextOrderQty.get_text().c_str());
      lOrder->PRICE = atoi(mTextOrderPrice.get_text().c_str());
      lOrder->BUYSELL = 'S';
      lOrder->STOCK = mStockId;
      lOrder->CLIENTORDERID= atoi(mTextClientOrderId.get_text().c_str());
      std::string lDestination = mTextBrokerId.get_text();
      lOrder->ORDERTYPE  =(lDestination.empty()?trading::OrderType::Broker:trading::OrderType::Retail);
      BrokerSession * lSession = GetActiveSession();
      strcpy(lOrder->USER,lSession->getUser().c_str());
      strcpy(lOrder->BROKER,lDestination.c_str());
      BaseWindow::notifyObservers(core::Event::NewOrder,lOrder);
      lSession->push((void *)lOrder, sizeof(trading::Order));
    }
  };
};

#endif //GTKMM_EXAMPLE_RANGEWIDGETS_H