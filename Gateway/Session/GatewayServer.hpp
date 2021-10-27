#ifndef _GATEWAY_SERVER_HPP_
#define _GATEWAY_SERVER_HPP_

#include <server/TCPServer.hpp>
#include <Application/Defines.hpp>

namespace gateway{

    /**
     * @brief a Tcp server implementation to monitor server tasks
     * 
     */
    class GatewayServer : public core::TCPServer{
        using Server = core::TCPServer;

        public :

            GatewayServer(uint64_t aServerId,const std::string& aAddress,uint16_t aPort):Server(aServerId,aAddress,aPort){
            }
            /**
            * @brief Callback before starting server.
            * It can be overloaded to implement some treatment
            */  
            virtual void onPreStart () {
                INFO(Gateway,"Gateway Server ::onPreStart");
            }; 
            
            /**
            * @brief  Callback after starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onStart (){
                INFO(Gateway,"Gateway Server ::onStart");
            }; 
            
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreClose () {
                INFO(Gateway,"Gateway Server ::onPreClose");
            }; 
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onClose () {
                INFO(Gateway,"Gateway Server ::onClose");
            }; 
            
            /**
            * @brief Callback when a client connect to the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onConnection (int aConnection) {
                INFO(Gateway,"Gateway Server ::onConnection Connection Id ="<<aConnection);
            }; 

            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            virtual void onDisconnect (int aConnection){
                INFO(Gateway,"Gateway Server ::onDisconnect Connection Id ="<<aConnection);
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
                INFO(Gateway,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Gateway,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Gateway,"::onCloseThread "<<aMessage);
            };

    };
}

#endif