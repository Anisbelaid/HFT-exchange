#ifndef _TRADING_SERVER_LINNE_HPP_
#define _TRADING_SERVER_LINNE_HPP_

#include <server/TCPServer.hpp>
#include <Application/Defines.hpp>


namespace matching {
    
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
                INFO(Matching,"Matching Server ::onPreStart");
            }; 
            
            /**
            * @brief  Callback after starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onStart (){
                INFO(Matching,"Matching Server ::onStart");
            }; 
            
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreClose () {
                INFO(Matching,"Matching Server ::onPreClose");
            }; 
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onClose () {
                INFO(Matching,"Matching Server ::onClose");
            }; 
            
            /**
            * @brief Callback when a client connect to the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onConnection (int aConnection) {
                INFO(Matching,"Matching Server ::onConnection Connection Id ="<<aConnection);
            }; 

            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            virtual void onDisconnect (int aConnection){
                INFO(Matching,"Matching Server ::onDisconnect Connection Id ="<<aConnection);
            };

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Matching,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                 DEBUGG(Matching,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(Matching,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(Matching,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(Matching,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Matching,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Matching,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Matching,"::onCloseThread "<<aMessage);
            };
            
    };
   
}


#endif
