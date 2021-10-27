#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include <pattern/Observer.hpp>
#include <server/IServer.hpp>
#include <thread/AsyncAccept.hpp>
#include <server/TCPConnection.hpp>
#include <pattern/Observer.hpp>
#include <thread/CPUTimer.hpp>
#include <boost/circular_buffer.hpp>

#include <libexplain/bind.h>
#include <libexplain/listen.h>
#include <libexplain/close.h>

#include <iostream>
#include <mutex>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <atomic>

#define MAX_QUEUE_LISTEN_SIZE 100
namespace core 
{

    // TODO Handle connection errors
    /**
    * Implementation of a generic Async TCP server with multiple connection allowed 
    * @tparam BUFFER_SIZE Indcate the size of the buffer size of the server async queue.
    * @tparam MAX_CONNECTION Maximum allowed connection on the server 
    * @tparam Allocator Custom Allocator type.
    *
    */
    class TCPServer  : public IServer , public core::ThreadPool {
        ///\Connection an alias to the thread pool
        using Pool = core::ThreadPool; 
        private :
            //// Server address
            struct sockaddr_in mServerAddr;
        public : 
            /**
            * @brief  Create a TCP Server.
            *
            * @return Constuctor
            */
            TCPServer (uint64_t,const std::string&,uint16_t);
            
            /**
            * @brief  Destroy a TCP Server.
            *
            */
            virtual ~TCPServer () {}; 
               
            /**
            * @brief  Used to stop a server .
            * @return stop result
            */
            bool stop () override;
            
            /**
            * @brief Used to start a server .
            * @return start result
            */
            bool start () override;

            /**
            * @brief Used to configure a server .
            * @return setup result
            */
            bool setup () override;

            /**
            * @brief Callback when a client connect to the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onConnection (int) override {}; 

            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            virtual void onDisconnect (int) override{}; 

            /**
            * @brief Used to instantiate a new client connection .
            * 
            * @param aConnectionId The connection Id of the incomming connection .
            */
            virtual void addConnection (int aConnectionId);
            
            /**
            * @brief Callback used to signal a failure upon creatio of a tcp connection  .
            * 
            * @param aConnectionId The connection Id of the incomming connection .
            * @return TCPConnection 
            */
            virtual void addConnectionFailure (int aConnectionId){};

            void push (void * aData, size_t aLength) {}

            void update(Event,void* aData = 0){};
    };
    
    TCPServer::TCPServer(uint64_t aServerId,const std::string& aAddress,uint16_t aPort) {
        IServer::mServerId = aServerId;
        IServer::mAddress = aAddress;
        IServer::mPort = aPort;
    }  

    
    void TCPServer::addConnection(int aClientSocket){
        onInfo("Server detected new incomming connection");
        onConnection(aClientSocket);
        notifyObservers(Event::NewClient,aClientSocket) ;
    } 
    
    bool TCPServer::setup() {
        onPreStart ();
        mServerSocket = socket(AF_INET, SOCK_STREAM , 0);
        if (mServerSocket<0)
        {
            onError("Failed to create server socket");
            return false;
        }
        int reuse = 1;
        if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0){
            onError("setsockopt (SO_REUSEADDR) failed");
            return false;
        }
        mServerAddr.sin_family = AF_INET;
        mServerAddr.sin_port = htons(mPort);
        mServerAddr.sin_addr.s_addr = inet_addr(mAddress.c_str());
        memset(mServerAddr.sin_zero, '\0', sizeof mServerAddr.sin_zero);
        return true;
    }
    
    bool TCPServer::stop () {
        onPreClose ();
        notifyObservers(Event::ServerShutdown);
        Pool::shutdown ();
        Pool::mCond.notify_all();
        ::shutdown(mServerSocket,SHUT_RDWR);
        if (::close(mServerSocket)!= 0){
            onError (::explain_close(mServerSocket));
            Pool::mCond.notify_all();
            return false;
        }
        Pool::mCond.notify_all(); 
        onClose ();
        return true;
    }

    bool TCPServer::start() {
        onStart ();
        if (::bind(mServerSocket, (struct sockaddr *) &mServerAddr, sizeof(mServerAddr))<0)
        {
            onError(::explain_bind(mServerSocket, (struct sockaddr *) &mServerAddr, sizeof(mServerAddr)));
            return false;
        }
        if(::listen(mServerSocket,MAX_QUEUE_LISTEN_SIZE)!=0)
        {
            onError (::explain_listen(mServerSocket,MAX_QUEUE_LISTEN_SIZE));
            return false;
        }
        auto lFAS = [&](int aConnection){this->addConnection(aConnection);};
        auto lFAF = [&](int aConnection){this->addConnectionFailure(aConnection);};
        ASYNC_Accept<> * lAcceptTask = new ASYNC_Accept<> ("Server ["+std::to_string(mServerId)+"] Accept Task",mServerSocket,lFAS,lFAF);
        Pool::addThread(lAcceptTask);
        return true;
    }

    
}
#endif
