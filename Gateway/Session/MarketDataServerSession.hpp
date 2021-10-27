#ifndef _MARKET_DATA_SERVER_SESSION_HPP_
#define _MARKET_DATA_SERVER_SESSION_HPP_

#include <skeleton/InstrumentManager.hpp>
#include <skeleton/Session.hpp>
#include <Application/Defines.hpp>

#include <client/UDPClient.hpp>
#include <EncoderDecoder.hpp>

#include <Trading/Order.hpp>


namespace gateway {
    
    class MarketDataClient : public core::UDPClientSession<1024*1024,core::LinearAllocator> {
             
        public :
            
            MarketDataClient (std::string aInterface,std::string aIp,int aPort,uint64_t aSessionName) : core::UDPClientSession<>(aInterface,aIp,aPort,aSessionName){
            };

            ~MarketDataClient () = default ;
            
             virtual void TreatInstrument (void*aMessage) override {
                INFO(Gateway,"Recieved new Order ");
                
            };
            
            virtual void TreatAddOrder (void*aMessage) override {
                INFO(Gateway,"Recieved new Order ");
                
            };
            
            virtual void TreatTrade (void*aMessage) override {
                INFO(Gateway,"Recieved Trade ");
                
            }; 
            
                        /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            void onDisconnect (){
                INFO(Gateway,"Gateway Server ::onDisconnect Connection Id <"<<mSessionName<<">");
                notifyObservers(core::Event::ClientDisconnection,mSessionName);
            };

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Gateway,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                 DEBUGG(Gateway,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(Gateway,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(Gateway,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(Gateway,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Gateway,"::onThreadAdded Market Data Session"<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Gateway,"::onThreadAdded Market Data Session"<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Gateway,"::onThreadAdded Market Data Session"<<aMessage);
            };
            

    };
}

#endif


