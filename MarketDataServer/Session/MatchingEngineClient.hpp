#ifndef _GATEWAY_CLIENT_HPP_
#define _GATEWAY_CLIENT_HPP_

#include <Application/Defines.hpp>
#include <Application/Defines.hpp>
#include <server/TCPServer.hpp>
#include <allocator/LinearAllocator.hpp>
#include <Session/MatchingEngineSession.hpp>
#include <EncoderDecoder.hpp>

namespace marketdata {

     /**
    * Implementation of a generic TCP Trading server allowing implement functional layer
    */
    class TradingServer : public core::TCPServer{
        using Server = core::TCPServer;

        public :
            
            TradingServer(uint64_t aServerId,const std::string& aAddress,uint16_t aPort):Server(aServerId,aAddress,aPort){
            }
            
           /**
            * @brief Callback before starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreStart () {
                INFO(MARKETDATA,"MARKETDATA Server ::onPreStart");
            }; 
            
            /**
            * @brief  Callback after starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onStart (){
                INFO(MARKETDATA,"MARKETDATA Server ::onStart");
            }; 
            
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreClose () {
                INFO(MARKETDATA,"MARKETDATA Server ::onPreClose");
            }; 
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onClose () {
                INFO(MARKETDATA,"MARKETDATA Server ::onClose");
            }; 
            
            /**
            * @brief Callback when a client connect to the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onConnection (int aConnection) {
                INFO(MARKETDATA,"MARKETDATA Server ::onConnection Connection Id ="<<aConnection);
            }; 

            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            virtual void onDisconnect (int aConnection){
                INFO(MARKETDATA,"MARKETDATA Server ::onDisconnect Connection Id ="<<aConnection);
            };

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                DEBUGG(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(MARKETDATA,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(MARKETDATA,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(MARKETDATA,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(MARKETDATA,"::onCloseThread "<<aMessage);
            };
            
    };
};



#endif

