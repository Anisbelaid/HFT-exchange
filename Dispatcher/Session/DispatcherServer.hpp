#ifndef _TRADING_SERVER_LINNE_HPP_
#define _TRADING_SERVER_LINNE_HPP_

#include <server/TCPServer.hpp>
#include <Application/Defines.hpp>


namespace dispatcher {
    
    /**
    * Implementation of a generic TCP Trading server allowing implement functional layer
    */
    class DispatcherServer : public core::TCPServer{
        using Server = core::TCPServer;

        public :
            
            DispatcherServer(uint64_t aServerId,const std::string& aAddress,uint16_t aPort):Server(aServerId,aAddress,aPort){
            }
            
           /**
            * @brief Callback before starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreStart () {
                INFO(Dispatcher,"Dispatcher Server ::onPreStart");
            }; 
            
            /**
            * @brief  Callback after starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onStart (){
                INFO(Dispatcher,"Dispatcher Server ::onStart");
            }; 
            
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreClose () {
                INFO(Dispatcher,"Dispatcher Server ::onPreClose");
            }; 
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onClose () {
                INFO(Dispatcher,"Dispatcher Server ::onClose");
            }; 
            
            /**
            * @brief Callback when a client connect to the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onConnection (int aConnection) {
                INFO(Dispatcher,"Dispatcher Server ::onConnection Connection Id ="<<aConnection);
            }; 

            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            virtual void onDisconnect (int aConnection){
                INFO(Dispatcher,"Dispatcher Server ::onDisconnect Connection Id ="<<aConnection);
            };

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
   
}


#endif
