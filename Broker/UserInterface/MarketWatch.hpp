#ifndef BROKER_MARKET_WATCH_HPP
#define BROKER_MARKET_WATCH_HPP

#include <Session/BrokerSession.hpp>
#include <UserInterface/MarketTreeView.hpp>

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/liststore.h>


namespace broker
{
  template <class Allocator = core::LinearAllocator>
  class MarketWatchView : public core::UserWindow <Allocator>
  {
    private :
      using BaseWindow = core::UserWindow <Allocator>;
      Gtk::Box mVBox;
      Gtk::ScrolledWindow mScrolledWindow;
      MarketTreeViewPopup<Allocator> mTreeView;
      
    public:
      MarketWatchView(Allocator*aAllocator,std::string aStyle,core::Configuration *aConfigurationManager,core::SessionManager* aSessionManager):core::UserWindow <Allocator>("Market Watch",aStyle,aAllocator,aSessionManager,aConfigurationManager),
      mVBox(Gtk::ORIENTATION_VERTICAL),mTreeView(aSessionManager,aConfigurationManager,BaseWindow::mAllocator)
      {
        BaseWindow::set_title("Market Watch");
        BaseWindow::set_border_width(5);
        BaseWindow::set_default_size(400, 200);
        BaseWindow::add(mVBox);
        mScrolledWindow.add(mTreeView);
        mScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        mVBox.pack_start(mScrolledWindow);
        BaseWindow::show_all_children();
      }

      virtual ~MarketWatchView()
      {
      }
   };

  
}
#endif //GTKMM_EXAMPLE_RANGEWIDGETS_H