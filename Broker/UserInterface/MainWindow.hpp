#ifndef _MAIN_WINDOW_HPP
#define _MAIN_WINDOW_HPP

#include <skeleton/UserInterface.hpp>
#include <UserInterface/MarketWatch.hpp>
#include <UserInterface/Trades.hpp>
#include <UserInterface/BasketView.hpp>

/**
 * @brief broker namespace
 * 
 */
namespace broker 
{
    /**
     * @brief define the entry main window to the appilcation
     * 
     * @tparam Allocator 
     */
    template <class Allocator = core::LinearAllocator>
    class MainWindow : public core::UserWindow <Allocator>
    {
        /// an alias to base window
        using BaseWindow = core::UserWindow <Allocator>;
        ///
        Gtk::Box                                  mBox ;
        ///
        Gtk::ToggleButton                         mMarketWatchButton ;
        ///
        Gtk::ToggleButton                         mTradesButton ;
        ///
        Gtk::ToggleButton                         mClientButton ;
        ///
        Gtk::ToggleButton                         mBasketButton ;
        ///
        Glib::RefPtr<Gtk::CssProvider>            mRefCssProvider;
        /// 
        core::UserWindow <Allocator>            * mConnectWidow{nullptr};
        ///
        core::UserWindow <Allocator>            * mMarketWatchWidow{nullptr};
        ///
        core::UserWindow <Allocator>            * mTradesWidow{nullptr};
        ///
        core::UserWindow <Allocator>            * mBaksetWindow{nullptr};
        
        public:
            /**
             * @brief Construct a new Main Window object
             * 
             * @param aAllocator 
             * @param aStyle 
             * @param aSessionManager 
             * @param aCongurationManager 
             */
            MainWindow(Allocator*aAllocator,std::string aStyle,core::SessionManager *aSessionManager,core::Configuration *aCongurationManager):core::UserWindow <Allocator>("MainWindow",aStyle,aAllocator,aSessionManager,aCongurationManager),
            mBox(Gtk::ORIENTATION_VERTICAL)
            {
                Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
                int lWidth = screen->get_width();
                int lHeight = screen->get_height();
                BaseWindow::set_default_size (lWidth/2,lHeight/2);
                BaseWindow::set_title("Eaumex Client");
                mMarketWatchButton.set_label("Market watch");
                mTradesButton.set_label("Trades");
                mClientButton.set_label("Client Connect");
                mBasketButton.set_label("Basket");
    
                mBox.pack_start( mMarketWatchButton, true, true );
                mBox.pack_start( mTradesButton, true, true );
                mBox.pack_start( mClientButton, true, true );
                mBox.pack_start( mBasketButton, true, true );
                BaseWindow::add( mBox );

                BaseWindow::show_all();

                mClientButton.signal_clicked().connect( sigc::bind(sigc::mem_fun(*this, &MainWindow<Allocator>::OnClientConnect),&mClientButton) );
                mMarketWatchButton.signal_clicked().connect( sigc::bind(sigc::mem_fun(*this, &MainWindow<Allocator>::OnMarketWatch),&mMarketWatchButton) );
                mTradesButton.signal_clicked().connect( sigc::bind(sigc::mem_fun(*this, &MainWindow<Allocator>::OnTrade),&mTradesButton) );
                mBasketButton.signal_clicked().connect( sigc::bind(sigc::mem_fun(*this, &MainWindow<Allocator>::OnNewBasket),&mBasketButton) );
            }

            /**
             * @brief Destroy the Main Window object
             * 
             */
            virtual ~MainWindow()
            {
                if (mConnectWidow)
                    delete mConnectWidow;
                if (mTradesWidow)
                    delete mTradesWidow;
                if (mBaksetWindow)
                    delete mBaksetWindow;
                if (mMarketWatchWidow)
                    delete mMarketWatchWidow;
            }

            /**
             * @brief called when window is hidden
             * 
             */
            void OnConnectWindowHidden ()
            {
                mClientButton.set_active (false);
            }
             /**
             * @brief called when window is hidden
             * 
             */
            void OnMarketWatchHidden ()
            {
                mMarketWatchButton.set_active (false);
            }
             /**
             * @brief called when window is hidden
             * 
             */
            void OnTradeWindowHidden ()
            {
                mTradesButton.set_active (false);
            }
            
             /**
             * @brief called when window is hidden
             * 
             */
            void OnBasketWindowHidden ()
            {
                mBasketButton.set_active (false);
            }

            /**
             * @brief called when toggle button is pressed
             * 
             * @param button 
             */
            void OnClientConnect (Gtk::ToggleButton *button)
            {
                if (!mConnectWidow)
                {
                    mConnectWidow = new ConnectWindow<Allocator> (BaseWindow::mAllocator,"../config/Connect.css",BaseWindow::mSessionManager,BaseWindow::mConfigurationManager);
                    mConnectWidow->signal_hide().connect (sigc::mem_fun(*this, &MainWindow<Allocator>::OnConnectWindowHidden));
                    mConnectWidow->registerObserver (this);
                    mConnectWidow->notifyObservers(core::Event::NewWindow,mConnectWidow);
                }
                if (button->get_active())
                {
                    mConnectWidow->notifyObservers(core::Event::WindowOpened,mConnectWidow);
                }
                else
                {
                    mConnectWidow->notifyObservers(core::Event::WindowClosed,mConnectWidow);
                }  
            }
            /**
             * @brief called when toggle button is pressed
             * 
             * @param button 
             */
            void OnMarketWatch(Gtk::ToggleButton *button)
            {
                if (!mMarketWatchWidow)
                {
                    mMarketWatchWidow = new MarketWatchView<Allocator> (BaseWindow::mAllocator,"../config/MarketWatch.css",BaseWindow::mConfigurationManager,BaseWindow::mSessionManager);
                    mMarketWatchWidow->signal_hide().connect (sigc::mem_fun(*this, &MainWindow<Allocator>::OnMarketWatchHidden));
                    mMarketWatchWidow->registerObserver (this);
                    mMarketWatchWidow->notifyObservers(core::Event::NewWindow,mMarketWatchWidow);
                }
                if (button->get_active())
                {
                    mMarketWatchWidow->notifyObservers(core::Event::WindowOpened,mMarketWatchWidow);
                }
                else
                {
                    mMarketWatchWidow->notifyObservers(core::Event::WindowClosed,mMarketWatchWidow);
                }  
                
            }
            /**
             * @brief called when toggle button is pressed
             * 
             * @param button 
             */
            void OnOrderBook ()
            {
            }
            /**
             * @brief called when toggle button is pressed
             * 
             * @param button 
             */
            void OnTrade (Gtk::ToggleButton *button)
            {
                if (!mTradesWidow)
                {
                    mTradesWidow = new TradesView<Allocator> (BaseWindow::mAllocator,"../config/Trades.css",BaseWindow::mConfigurationManager,BaseWindow::mSessionManager);
                    mTradesWidow->signal_hide().connect (sigc::mem_fun(*this, &MainWindow<Allocator>::OnMarketWatchHidden));
                    mTradesWidow->registerObserver (this);
                    mTradesWidow->notifyObservers(core::Event::NewWindow,mTradesWidow);
                }
                if (button->get_active())
                {
                    mTradesWidow->notifyObservers(core::Event::WindowOpened,mTradesWidow);
                }
                else
                {
                    mTradesWidow->notifyObservers(core::Event::WindowClosed,mTradesWidow);
                }  
            }
            /**
             * @brief called when toggle button is pressed
             * 
             * @param button 
             */
            void OnNewBasket (Gtk::ToggleButton *button)
            {
                if (!mBaksetWindow)
                {
                    mBaksetWindow = new BasketView<Allocator> (BaseWindow::mAllocator,"../config/Basket.css",BaseWindow::mConfigurationManager,BaseWindow::mSessionManager);
                    mBaksetWindow->signal_hide().connect (sigc::mem_fun(*this, &MainWindow<Allocator>::OnBasketWindowHidden));
                    mBaksetWindow->registerObserver (this);
                    mBaksetWindow->notifyObservers(core::Event::NewWindow,mBaksetWindow);
                }
                if (button->get_active())
                {
                    mBaksetWindow->notifyObservers(core::Event::WindowOpened,mBaksetWindow);
                }
                else
                {
                    mBaksetWindow->notifyObservers(core::Event::DeleteWindow,mBaksetWindow);
                    mBaksetWindow=nullptr;
                }  
            }
    };
}


#endif