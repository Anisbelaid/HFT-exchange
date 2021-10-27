#ifndef BROKER_TRADES_HPP
#define BROKER_TRADES_HPP

#include <Session/BrokerSession.hpp>
#include <UserInterface/TradesTreeView.hpp>

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/liststore.h>


namespace broker
{
  template <class Allocator = core::LinearAllocator>
  class TradesView : public core::UserWindow <Allocator>
  {
    private :
      using BaseWindow = core::UserWindow <Allocator>;
      Gtk::Box mVBox;
      Gtk::ScrolledWindow mScrolledWindow;
      TradesTreeViewPopup<Allocator> mTreeView;
      
    public:
      TradesView(Allocator*aAllocator,std::string aStyle,core::Configuration *aConfigurationManager,core::SessionManager* aSessionManager):core::UserWindow <Allocator>("Market Watch",aStyle,aAllocator,aSessionManager,aConfigurationManager),
      mVBox(Gtk::ORIENTATION_VERTICAL),mTreeView(aSessionManager,BaseWindow::mAllocator)
      {
        BaseWindow::set_title("Trades");
        BaseWindow::set_border_width(5);
        BaseWindow::set_default_size(400, 200);
        BaseWindow::add(mVBox);
        mScrolledWindow.add(mTreeView);
        mScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        mVBox.pack_start(mScrolledWindow);
        
        BaseWindow::show_all_children();
      }

      virtual ~TradesView()
      {
      }
   };

  
}
#endif //GTKMM_EXAMPLE_RANGEWIDGETS_H