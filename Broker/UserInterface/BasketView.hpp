#ifndef BASKET_ORDER_BOOK_HPP
#define BASKET_ORDER_BOOK_HPP

#include <Session/BrokerSession.hpp>
#include <UserInterface/BasketTreeView.hpp>
#include <rapidcsv.h>
#include <thread>

namespace broker
{

  class ConnectionColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ConnectionColumns()
    {
      add(mBrokerId);
      add(mBrokerName);
    }

    Gtk::TreeModelColumn<Glib::ustring> mBrokerId; //The data to choose - this must be text.
    Gtk::TreeModelColumn<Glib::ustring> mBrokerName;
  };

  template <class Allocator = core::LinearAllocator>
  class BasketView : public core::UserWindow<Allocator>
  {
    //Child widgets:
  private:
    using BaseWindow = core::UserWindow<Allocator>;
    Gtk::Box mVBox;
    Gtk::ScrolledWindow mScrolledWindow;
    BasketTreeView<Allocator> mTreeView;
    Gtk::ButtonBox mButtonBox;
    Gtk::ButtonBox mSendBox;
    Gtk::Button mSendButton;
    Glib::RefPtr<Gtk::ListStore> mRefTreeModel;
    ConnectionColumns mColumns;
    Gtk::Label mConnectionLabel;
    Gtk::ComboBox mConnection;
    Gtk::Entry mCsvFilePath;
    Gtk::Label mLabelCsvFilePath;
    Gtk::Button mLoadButton;
    Gtk::Button mButtonStop;
    std::string mCsvFile;
    Gtk::ProgressBar mProgressBar;
    double           mProgressBarFraction{0.0};
    sigc::connection mTimeout;
    Gtk::Alignment mAlign;
    std::thread *mSendingThread{nullptr};

  public:
    BasketView(Allocator *aAllocator,std::string aStyle, core::Configuration *aConfigurationManager, core::SessionManager *aSessionManager) : core::UserWindow<Allocator>("Order Bottle",aStyle, aAllocator, aSessionManager, aConfigurationManager),
                                                                                                                           mTreeView(aSessionManager, aAllocator), mVBox(Gtk::ORIENTATION_VERTICAL), mSendButton("Send"), mLoadButton("Load"), mButtonStop("Stop"), mAlign(0.5, 0.5)
    {
      BaseWindow::set_title("Basket");
      BaseWindow::set_border_width(5);
      BaseWindow::set_default_size(400, 200);

      BaseWindow::add(mVBox);
      mConnectionLabel.set_label("Session");
      mRefTreeModel = Gtk::ListStore::create(mColumns);
      mConnection.set_model(mRefTreeModel);

      mConnection.pack_start(mColumns.mBrokerId);
      mConnection.pack_start(mColumns.mBrokerName);

      mLabelCsvFilePath.set_label("Basket Path");
      mCsvFilePath.set_editable(false);
      //Add the TreeView, inside a ScrolledWindow, with the button underneath:
      mScrolledWindow.add(mTreeView);

      //Only show the scrollbars when they are necessary:
      mScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

      mVBox.pack_start(mScrolledWindow);
      mVBox.pack_start(mButtonBox, Gtk::PACK_SHRINK);
      mVBox.pack_start(mSendBox, Gtk::PACK_SHRINK);
      mVBox.pack_start(mAlign, false, false, 5);

      mProgressBar.set_margin_end(5);
      mProgressBar.set_size_request(100, -1);
      mProgressBar.set_text("Waiting");

      mAlign.add(mProgressBar);
      //mProgessBarBox.set_layout(Gtk::BUTTONBOX_SPREAD);

      mButtonBox.pack_start(mLabelCsvFilePath);
      mButtonBox.pack_start(mCsvFilePath);
      mButtonBox.pack_start(mLoadButton);
      mButtonBox.set_border_width(5);
      mButtonBox.set_layout(Gtk::BUTTONBOX_SPREAD);

      mSendBox.pack_start(mConnectionLabel);
      mSendBox.pack_start(mConnection);
      //mSendBox.pack_start(mProgressBar);
      mSendBox.pack_start(mSendButton);
      mSendBox.pack_start(mButtonStop);
      mSendBox.set_border_width(5);
      mSendBox.set_layout(Gtk::BUTTONBOX_SPREAD);

      mButtonStop.signal_clicked().connect(sigc::mem_fun(*this, &BasketView::onButtonQuit));
      mLoadButton.signal_clicked().connect(sigc::mem_fun(*this, &BasketView::onButtonLoad));
      mSendButton.signal_clicked().connect(sigc::mem_fun(*this, &BasketView::onButtonSend));
      PopulateClientConnection();

      BrokerSession *lSession = nullptr;
      std::vector<BrokerSection *> lConf = dynamic_cast<broker::Configuration *>(BaseWindow::mConfigurationManager)->getBrokersConfiguration();
      for (auto lElement : lConf)
      {
       
        lSession = dynamic_cast<BrokerSession *>(BaseWindow::mSessionManager->GetTCPClientSessionById(std::to_string(lElement->mIndex)));
        lSession->RegisterOrdeRejected([this](void *aMessage){this->OnOrderRejected(aMessage);});
        lSession->RegisterOrderAccecpted([this](void *aMessage){this->onOrderAccepted(aMessage);});
        lSession->RegisterOrderAccecpted([this](void *aMessage){this->OnOrderCanceled(aMessage);});
      }  

      mTimeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,&BasketView::on_timeout), 1);
      BaseWindow::show_all_children();
    }
    virtual ~BasketView()
    {
      if (mSendingThread)
      {
        mSendingThread->detach();
        delete mSendingThread;
      }
    }
    long round(double var)
    {
      return (long)(var * 100 + .5);
    }

    void onCsvLoaded()
    {
      mTreeView.onClear();
      mCsvFilePath.set_text(mCsvFile);
      rapidcsv::Document lDoc(mCsvFile);
      std::vector<unsigned long> lIds = lDoc.GetColumn<unsigned long>("Event ID");
      std::vector<std::string> lTypes = lDoc.GetColumn<std::string>("Event Type");
      std::vector<std::string> lLimits = lDoc.GetColumn<std::string>("Limit Price (USD)");
      std::vector<std::string> lQtys = lDoc.GetColumn<std::string>("Original Quantity (BTC)");
      std::vector<std::string> lSides = lDoc.GetColumn<std::string>("Side");
      std::vector<uint64_t>    lOrderIds =lDoc.GetColumn<uint64_t>("Order ID");
      std::vector<uint64_t>    lEventIds =lDoc.GetColumn<uint64_t>("Event ID");
      //0,Event ID,Event Date,Event Time,Event Millis,Order ID,Execution Options,Event Type,Symbol,Order Type,Side,Limit Price (USD),Original Quantity (BTC),Gross Notional Value (USD),Fill Price (USD),Fill Quantity (BTC),Total Exec Quantity (BTC),Remaining Quantity (BTC),Avg Price (USD),Fees (USD),Auction ID,Account ID,IOI ID,Order Cancel Reason

      for (int i = 0; i < lIds.size(); i++)
      {
        mTreeView.onOrderAdded(lTypes[i], 500, lSides[i], round(atof(lQtys[i].c_str())), round(atof(lLimits[i].c_str())),lOrderIds[i],lEventIds[i]);
        unsigned long lNB=mTreeView.getNumberAction();
        std::string lNBs = std::to_string(lNB);
        mProgressBar.set_text(lNBs);
        mProgressBar.set_show_text(true);
      }
      mProgressBarFraction=(double)1/(double)mTreeView.getNumberAction();
    }

    void PopulateClientConnection()
    {
      std::vector<BrokerSection *> lConf = dynamic_cast<broker::Configuration *>(BaseWindow::mConfigurationManager)->getBrokersConfiguration();
      for (auto lElement : lConf)
      {
        Gtk::TreeModel::Row row = *(mRefTreeModel->append());
        row[mColumns.mBrokerId] = std::to_string(lElement->mIndex);
        row[mColumns.mBrokerName] = lElement->mBrokerName;
      }
    }

    void onButtonQuit()
    {
      BaseWindow::hide();
    }

    void onButtonSend()
    {
      BrokerSession *lSession = nullptr;
      const auto iter = mConnection.get_active();
      if (iter)
      {
        const auto row = *iter;
        if (row)
        {
          Glib::ustring lId         = row[mColumns.mBrokerId];
          lSession = dynamic_cast<BrokerSession *>(BaseWindow::mSessionManager->GetTCPClientSessionById(lId));
        }
      }
      mSendingThread =new std::thread ([this](BrokerSession *aSession){this->SendOrder(aSession);},lSession);
      mSendingThread->detach(); 
      
    }

    void SendOrder (BrokerSession *aSession)
    {
        mTreeView.onSendBasket(aSession);
        
    }

    void onOrderAccepted (void*aMessage)
    {
        mTreeView.onOrderAccepted(aMessage);
        
    }

    void OnOrderRejected (void*aMessage)
    {
      mTreeView.onOrderRejected(aMessage);  
    }


    void OnOrderCanceled (void*aMessage)
    {
      mTreeView.OnOrderCanceled(aMessage);  
    }

    bool on_timeout()
    {
        std::string lValue=mProgressBar.get_text();
        std::string lNewValue=std::to_string(mTreeView.getNumberAction());
        if (lValue!=lNewValue) 
        {
          mProgressBar.pulse();
          return true;
        }
        /*mProgressBar.set_text(std::to_string(mTreeView.getNumberAction()));
        mProgressBar.set_show_text(true);
        // Calculate the value of the progress bar using the value range set in the adjustment object
        double new_value = mProgressBar.get_fraction() + mProgressBarFraction;
        // Set the new value
        mProgressBar.set_fraction(new_value); */
        return true;
    }
    void onButtonLoad()
    {
      // Create the dialog box FileChooser
      Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
      dialog.set_transient_for(*this);

      //Add response buttons the the dialog:
      dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
      dialog.add_button("Open", Gtk::RESPONSE_OK);

      //Add filters, so that only certain file types can be selected:
      Glib::RefPtr filter_csv = Gtk::FileFilter::create();
      filter_csv->set_name("CSV files");
      filter_csv->add_mime_type("text/csv");
      dialog.add_filter(filter_csv);

      //Show the dialog and wait for a user response:
      int result = dialog.run();

      //Handle the response:
      switch (result)
      {
      case (Gtk::RESPONSE_OK):
      {
        mCsvFile = dialog.get_filename();
        onCsvLoaded();
        break;
      }
      case (Gtk::RESPONSE_CANCEL):
      {
        break;
      }
      default:
      {
        break;
      }
      };
    };
  };

}
#endif //GTKMM_EXAMPLE_RANGEWIDGETS_H