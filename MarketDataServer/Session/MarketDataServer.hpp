#ifndef _MULTICAST_SERVER_HPP_
#define _MULTICAST_SERVER_HPP_

#include <boost/core/noncopyable.hpp>
#include <server/UDPServer.hpp> 
#include <Application/Defines.hpp>



//TODO Think of using a tcp connection for Internal information broadcast to avoid packet synchronisation 
namespace marketdata {

    /**
    * Implementation of a generic UDP multicast server allowing implement functional layer
    */
    class MarketDataServer : public core::UDPServer<> {
            /// \Server an alias to the server 
            using Server = core::UDPServer<>; 
            /// \Server an alias to the thread Pool 
            using Pool = core::ThreadPool;
        public :

            /**
            * Create a marketData Server to broadcat feed.
            * @param aPort The port that will be used.
            * @param aAllocator aPointer to a custom allocator to allocate directly packet sent to the client.
            * @return Private constuctor
            */
            MarketDataServer (int aPort) : Server (aPort){ };
           
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
