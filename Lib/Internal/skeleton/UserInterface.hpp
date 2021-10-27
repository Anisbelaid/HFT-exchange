#ifndef _USER_INTERFACE_HPP
#define _USER_INTERFACE_HPP

#include <gtkmm.h>
#include <skeleton/SessionManager.hpp>
#include <skeleton/Configuration.hpp>
#include <allocator/LinearAllocator.hpp>
#include <pattern/Observer.hpp>
#include <unordered_map>
#include <Trading/Order.hpp>
namespace core 
{

    template <class Allocator = core::LinearAllocator>
    class UserWindow : public  Gtk::Window , public Subject , public Observer
    {
        protected :
            Configuration                                       *mConfigurationManager{nullptr};
            SessionManager                                      *mSessionManager{nullptr};
            Allocator                                           *mAllocator {nullptr};
            uint64_t                                             mIds;
            std::unordered_map <uint64_t,UserWindow<Allocator>*> mChildWindow;
            std::string                                          mCssStylePath;
        public :
            UserWindow (std::string aName,std::string aStylePath,Allocator*aAllocator,SessionManager*aSessionManager,Configuration *aConfigurationManager):mSessionManager(aSessionManager),mIds(0)
            ,mConfigurationManager(aConfigurationManager),mAllocator(aAllocator),mCssStylePath(aStylePath)
            {
                mIds = std::hash<std::string>{}(aName);
                mAllocator = new Allocator (),
                mAllocator->Init (1024*1024*1024,64);
                try
                {
                    auto css = Gtk::CssProvider::create();
                    css->load_from_path(mCssStylePath.c_str());
                    Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css,GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
                }
                catch(const Gtk::CssProviderError error)
                {
                    std::cout << "Failed to load style:" <<error.code() << std::endl;
                }
            }
            /**
             * @brief 
             * 
             */
            virtual void onWindowChildCreated (UserWindow<Allocator> *aUserWidow){
                mChildWindow.insert(std::make_pair(aUserWidow->getId(),aUserWidow));
                aUserWidow->show();
            };
            /**
             * @brief 
             * 
             */
            virtual void onWindowChildDeleted (UserWindow<Allocator> *aUserWidow){
                aUserWidow->hide();
                delete aUserWidow;
                mChildWindow.erase(aUserWidow->getId());
            };
            /**
             * @brief 
             * 
             */
            virtual void onWindowChildOpened  (UserWindow<Allocator> *aUserWidow){
                aUserWidow->show();
            };
            /**
             * @brief 
             * 
             */
            virtual void onWindowChildClosed  (UserWindow<Allocator> *aUserWidow){
                aUserWidow->hide();
            };
            /**
             * @brief 
             * 
             */
            virtual void onNewOrder  (trading::Order *aOrder){
            };
            /**
             * @brief 
             * 
             */
            virtual void onCancelOrder  (trading::Cancel *aCancel){
            };
             /**
             * Update the state of this observer
             */
            void update(Event,  StructData * aData = nullptr) 
            {

            }

            virtual void update(Event,uint64_t aData = 0) {};

            /**
             * Update the state of this observer
             */
            void update(Event aEvent ,void* aData= 0) 
            {   
                
                switch (aEvent)
                {
                    case Event::NewWindow :
                    {
                        UserWindow<Allocator> * lUserWidow = reinterpret_cast< UserWindow<Allocator>* >(aData);
                        onWindowChildCreated (lUserWidow);
                    }
                    break;
                    case Event::DeleteWindow :
                    {
                        UserWindow<Allocator> * lUserWidow = reinterpret_cast< UserWindow<Allocator>* >(aData);
                        onWindowChildDeleted (lUserWidow);
                        
                    }
                    break;
                    case Event::WindowOpened :
                    {
                        UserWindow<Allocator> * lUserWidow = reinterpret_cast< UserWindow<Allocator>* >(aData);
                        onWindowChildOpened (lUserWidow);
                        
                    }
                    break;
                    case Event::WindowClosed :
                    {
                        UserWindow<Allocator> * lUserWidow = reinterpret_cast< UserWindow<Allocator>* >(aData);
                        onWindowChildClosed (lUserWidow);
                    }
                    break;
                    case Event::NewOrder :
                    {
                        trading::Order * lOrder = reinterpret_cast< trading::Order* >(aData);
                        onNewOrder (lOrder);
                    }
                    break;
                    case Event::CancelOrder :
                    {
                       trading::Cancel * lCancel = reinterpret_cast< trading::Cancel* >(aData);
                        onCancelOrder (lCancel);
                    }
                    break;
                    default:
                    break;
                } 
            }
            
            const uint64_t& getId ()
            {
                return mIds;
            }
            
            virtual ~UserWindow ()
            {
               for (auto lIt: mChildWindow)
               {
                   lIt.second->hide();
                   delete lIt.second;
               }
            }

    };

    template <class Allocator = core::LinearAllocator>
    class UserInterface : public Gtk::Application 
    {
        using Base = Gtk::Application;
        protected:
            Configuration                                       *mConfigurationManager{nullptr};
            SessionManager                                      *mSessionManager{nullptr};
            Allocator                                           *mAllocator {nullptr};
            UserWindow<Allocator>                               *mMainWindow {nullptr};
        public :
            
            UserInterface (SessionManager*aSessionManager,Configuration *aConfigurationManager):mSessionManager(aSessionManager),mConfigurationManager(aConfigurationManager)
            {
                mAllocator = new Allocator (),
                mAllocator->Init (1024*1024,64);
            }

            virtual ~UserInterface (){
                if (mMainWindow)
                {
                    delete mMainWindow;
                }
                 if (mAllocator)
                {
                    delete mAllocator;
                }
            }

            
            virtual int InitUI (int argc,char** argv) = 0;
        
    };
}
#endif 