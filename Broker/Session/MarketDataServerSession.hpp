#ifndef _MARKET_DATA_SERVER_SESSION_HPP_
#define _MARKET_DATA_SERVER_SESSION_HPP_

#include <skeleton/InstrumentManager.hpp>
#include <skeleton/Session.hpp>
#include <Application/Defines.hpp>

#include <client/UDPClient.hpp>
#include <EncoderDecoder.hpp>

#include <Trading/Order.hpp>


namespace broker {
    
    class MarketDataClient : public core::UDPClientSession<> {
             
        public :
            
            MarketDataClient (std::string aInterface,std::string aIp,int aPort,uint64_t aSessionName) : core::UDPClientSession<>(aInterface,aIp,aPort,aSessionName){
            };

            ~MarketDataClient () = default ;
            
             virtual void TreatInstrument (void*aMessage) override {
                INFO(Broker,"Recieved new Instrument ");
                for (auto lIt : mInstrumentCallback)
                    lIt(aMessage);
            };
            
            virtual void TreatAddOrder (void*aMessage) override {
                INFO(Broker,"Recieved new Order ");
            };
            
            virtual void TreatTrade (void*aMessage) override {
                INFO(Broker,"Recieved Trade ");
                for (auto lIt : mTradesCallback)
                    lIt(aMessage);
            }; 

            virtual void TreatCancelOrder (void*aMessage) override {
                INFO(Broker,"Recieved Cancel ");
                
            };
            
                        /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            void onDisconnect (){
                INFO(Broker,"Broker Server ::onDisconnect Connection Id <"<<mSessionName<<">");
                notifyObservers(core::Event::ClientDisconnection,mSessionName);
            };

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Broker,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                 DEBUGG(Broker,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(Broker,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(Broker,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(Broker,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Broker,"::onThreadAdded Market Data Session"<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Broker,"::onThreadAdded Market Data Session"<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Broker,"::onThreadAdded Market Data Session"<<aMessage);
            };
            

    };
}

#endif


