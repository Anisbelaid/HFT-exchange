#ifndef _LOGGING_SERVER_HPP_
#define _LOGGING_SERVER_HPP_

#include <functional>
#include <boost/core/noncopyable.hpp>
#include <Singleton.hpp>
#include <CustomArray.hpp>
#include <Trading/Order.hpp>
#include <Trading/Trade.hpp>
#include <Trading/Reject.hpp>
#include <Trading/OrderReply.hpp>
#include <Feed/Order.hpp>
#include <Feed/Trade.hpp>
#include <Feed/Instrument.hpp>
#include <Application/Defines.hpp>



namespace logger {
    
    /**
    * Implementation of a generic logger server allowing implement functional layer
    */
    class LoggerHelper : public core::Singleton<LoggerHelper > {
        public:
            
            friend class core::Singleton<LoggerHelper >;
            enum class EventEnum : uint8_t {
                Order = 1,
                Cancel = 2,
                Trade =3,
            };
            
        private :
           
            ///File Path
            std::string mFilePath;
            
            core::CustomArray<std::function<void(void*)>,1,128,true> mCallbacks;
        public :
            
            void onNewEvent (EventEnum aEvent,void*aMessage = nullptr);
            
            /**
            * Destroy a Trading Server.
            *
            * @return Destuctor
            */
            ~LoggerHelper () = default;
        private :
            
            /**
            * Create a DataBaseHelper to access Database.
            */
            LoggerHelper () ;
            
            void onNewOrder (void*);
            
            void onCancel (void*);
            
            void onTrade (void*);
            
            void onUnkownEvent (void*);
                  
    };
   
}
    logger::LoggerHelper::LoggerHelper() : Singleton<LoggerHelper>(), mCallbacks([&](void* aMessage ){this->onUnkownEvent(aMessage);}) {
        mCallbacks.add(1,[&](void* aMessage ){this->onNewOrder(aMessage);});
        mCallbacks.add(2,[&](void* aMessage ){this->onCancel(aMessage);});
        mCallbacks.add(3,[&](void* aMessage ){this->onTrade(aMessage);});
    }; 
    
    void logger::LoggerHelper::onNewEvent(EventEnum aEvent,void*aMessage) {
        mCallbacks[(uint8_t)aEvent](aMessage);
    };
    
    void logger::LoggerHelper::onNewOrder(void* aOrder) {
        INFO(PERSISTANCE," on New Order = ");
    };
    
    void logger::LoggerHelper::onCancel(void* aCancel) {
    };
    
    void logger::LoggerHelper::onTrade(void* aTrade) {
    };
    
    void logger::LoggerHelper::onUnkownEvent(void* aTrade) {
    };


    



#endif

