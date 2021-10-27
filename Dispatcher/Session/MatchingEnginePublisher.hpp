#ifndef MARKETDATA_SESSION_HPP_
#define MARKETDATA_SESSION_HPP_

#include <Application/Defines.hpp>

#include <skeleton/Session.hpp>
#include <client/TCPClient.hpp>
#include <allocator/LinearAllocator.hpp>


namespace dispatcher {

    /**
    * Implementation of a generic UDP multicast server allowing implement functional layer
    */
    class MatchingEnginePublisher : public core::TCPClientSession<>  {
            using Client = core::TCPClientSession<>;
        public :

            MatchingEnginePublisher (const std::string& aAddress,const uint16_t aPort,const std::string& aInterface ,uint64_t aSessionName) :
            Client(aAddress,aPort,aInterface,aSessionName){
            }

            ~MatchingEnginePublisher () = default ;
            

            bool close () override {
                INFO(Dispatcher,"Matching Engine Session Session ::close Session Id <"<<mSessionName<<">");
                return Client::close ();
            }

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Dispatcher,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                 DEBUGG(Dispatcher,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(Dispatcher,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(Dispatcher,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(Dispatcher,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Dispatcher,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Dispatcher,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Dispatcher,"::onCloseThread "<<aMessage);
            };
            
    };    
};


#endif
