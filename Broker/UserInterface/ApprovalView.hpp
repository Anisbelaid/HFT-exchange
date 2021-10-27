#ifndef APPROVAL_HPP
#define APPROVAL_HPP

#include <Session/BrokerSession.hpp>
#include <UserInterface/ApprovalTreeView.hpp>

#include <gtkmm.h>
#include <gtkmm/window.h>



namespace broker
{
  template <class Allocator = core::LinearAllocator>
  class ApprovalView : public core::UserWindow <Allocator>
  {
    //Child widgets:
    private :
      using BaseWindow = core::UserWindow <Allocator>;
      
      const std::string                   cWindowName{"Order Approval"};
      Gtk::Box                             mVBox;
      Gtk::ScrolledWindow                  mScrolledWindow;
      ApprovalViewTreeViewPopup<Allocator> *mTreeView;
      Gtk::ButtonBox                       mButtonBox;
      /// The trading session of the user
      std::string                         mUserIndex;
    public:
      ApprovalView(Allocator*aAllocator,std::string aStyle,core::Configuration *aConfigurationManager,core::SessionManager* aSessionManager,std::string aUserIndex):core::UserWindow <Allocator>("Approval",aStyle,aAllocator,aSessionManager,aConfigurationManager),
      mVBox(Gtk::ORIENTATION_VERTICAL),mUserIndex(aUserIndex)
      {
        BaseWindow::set_title("Approval");
        Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
        int lWidth = screen->get_width();
        int lHeight = screen->get_height();
        BaseWindow::set_default_size (lWidth/2,lHeight/2);
        BaseWindow::set_title(cWindowName+" User :"+getBrokerName());

        BaseWindow::add(mVBox);

        BrokerSession*lSession = dynamic_cast<BrokerSession*>(BaseWindow::mSessionManager->GetTCPClientSessionById(mUserIndex));
        
        mTreeView=new ApprovalViewTreeViewPopup<Allocator>(aSessionManager,aAllocator,lSession);
        //Add the TreeView, inside a ScrolledWindow, with the button underneath:
        mScrolledWindow.add(*mTreeView);

        //Only show the scrollbars when they are necessary:
        mScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

        mVBox.pack_start(mScrolledWindow);
        mVBox.pack_start(mButtonBox, Gtk::PACK_SHRINK);

        mButtonBox.set_border_width(5);
        mButtonBox.set_layout(Gtk::BUTTONBOX_END);

        BaseWindow::show_all_children();


      }
      virtual ~ApprovalView()
      {
        delete mTreeView;
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
#endif //GTKMM_EXAMPLE_RANGEWIDGETS_H