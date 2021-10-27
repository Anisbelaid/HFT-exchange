#ifndef _PERSISTANCE_SERVER_HPP_
#define _PERSISTANCE_SERVER_HPP_

#include <boost/core/noncopyable.hpp>
#include <TCPServer.hpp> 
#include <Server/PersistenceSession.hpp>

using namespace std::placeholders;


namespace server {
    
       
    /**
    * Implementation of a generic Logging server allowing implement functional layer
    */
    class PersistenceServer : public core::TCPServer< >   {
            ///\Pool an alias to the thread pool context
            using Pool = core::ThreadPool;
            ///\Pool an alias to the TCP Server
            using Server = core::TCPServer;
        private :
            
            ///A flag used to check if there only one instance in the program of the trading server
            static std::once_flag mFlag;
            
            ///A pointer to the istance of the trading server
            static PersistenceServer * mInstance;
            /**
            * Create a Trading Server to acess matching engine.
            * @param aPort The port that will be used.
            * @param aAllocator aPointer to a custom allocator to allocate directly packet received from the exchange.
            * @return Private constuctor
            */
            PersistenceServer (int aPort) ;

        public :
            
            /**
            * @brief return an Instance of the trading server.
            * @param aPort The port that will be used.
            * @return static TradingServer*
            */
            static PersistenceServer* getInstance (uint16_t aPort=0)
            {
                std::call_once(mFlag,
                            [&](PersistenceServer *&aIntance) {
                                aIntance = new PersistenceServer(aPort);
                            },
                            mInstance);
                return mInstance;
            }
            
            
            /**
            * @brief an overload of the callback of connection addition
            * @param aConnectionId a ConnectionId
            * @return static TradingServer*
            */
            void onConnection  (int aConnectionId) {
                //std::cout<< "on Connection "<< aConnectionId<<std::endl;
            }; 
            
            /**
            * @brief Used to instantiate a new client connection .
            * It can be overloaded to implement some treatment
            * @param aSessionId The connection Id of the incomming message .
            * @return TCPConnection 
            */
            core::TCPConnection<> * createNewConnection(int aSessionId);
            
            /**
            * Destroy a Trading Server.
            *
            * @return Destuctor
            */
            ~PersistenceServer () = default;
            
            
            void onError (const char * aError) override {
                INFO(PERSISTANCE,"Persistance Server ServerId =["<<Server::mServerId<<"]  Reported an error "<<aError);
            };
            
            void onInfo (const char * aError) override{
                INFO(PERSISTANCE,"Persistance Server ServerId =["<<Server::mServerId<<"]  Reported an event "<<aError);
            };
            
            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            * @return void
            */
            void onDisconnect (int aConId);
            
            
    };
   
}

    server::PersistenceServer *server::PersistenceServer::mInstance{0x0};
    std::once_flag server::PersistenceServer::mFlag;
    

     server::PersistenceServer::PersistenceServer(int aPort) : Server (aPort){
                Server::setup();
                Server::start ();
    };
    
    
    core::TCPConnection<> * server::PersistenceServer::createNewConnection(int aSessionId) {
                server::PersistenceSession * lSession =  new server::PersistenceSession (aSessionId,aSessionId,*this);
                return lSession;
    };
    
     void server::PersistenceServer::onDisconnect (int aConId) {
        if (mConnection.find (aConId) != mConnection.end()){
            INFO(PERSISTANCE, "Shutding down Client  ... "<<aConId);
            delete mConnection[aConId];
            mConnection.erase(aConId);
        }
        else
            DEBUGG(PERSISTANCE, "Connection not found ... "<<aConId);
    }



#endif

