
#ifndef BROKER_USERINTERFACE_HPP
#define BROKER_USERINTERFACE_HPP


#include <UserInterface/ConnectWindow.hpp>
#include <UserInterface/MainWindow.hpp>
#include <UserInterface/MarketWatch.hpp>

#include <skeleton/UserInterface.hpp>

namespace broker {
    
    /**
    *   a class representing the broker user interface  
    *
    */
   template <class Allocator = core::LinearAllocator>
    class UserInterface : public core::UserInterface<Allocator> 
    {   
        using BaseInterface = core::UserInterface<Allocator> ;
        public :
            UserInterface(core::SessionManager*aSessionManager,core::Configuration*aConfigurationManager):core::UserInterface<>(aSessionManager,aConfigurationManager)
            {
            }

            int InitUI (int argc,char** argv) override 
            {
                BaseInterface::mMainWindow = new MainWindow<Allocator>(BaseInterface::mAllocator,"../config/MainWindow.css",BaseInterface::mSessionManager,BaseInterface::mConfigurationManager);
                return BaseInterface::run(*BaseInterface::mMainWindow);
            };            
    };  
}


#endif 



