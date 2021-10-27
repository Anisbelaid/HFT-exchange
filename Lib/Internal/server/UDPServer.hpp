#ifndef _UDP_SERVER_HPP_
#define _UDP_SERVER_HPP_

#include <server/IServer.hpp>
#include <thread/AsyncTask.hpp>
#include <thread/ThreadPool.hpp>
#include <thread/CPUTimer.hpp>
#include <thread/AsyncUDPBroadcast.hpp>


#include <boost/circular_buffer.hpp>

#include <iostream>
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
#include <stdio.h>
#include <errno.h>
#include <string.h>


#define LOCALHOST "127.0.0.1"

using namespace std::placeholders;
namespace core {
    
    // TODO Handle connection errors
    /**
    * Implementation of a generic Async TCP server with multiple connection allowed 
    * @tparam BUFFER_SIZE Indcate the size of the buffer size of the server async queue.
    * @tparam Allocator Custom Allocator type.
    *
    */
    template <uint32_t BUFFER_SIZE = 1024 *1024 ,class Allocator = core::LinearAllocator>
    class UDPServer  : public core::ThreadPool , public Routine ,public IServer {
            ///\Pool an alias to a pool
            using Pool = core::ThreadPool;
        protected:
            
            /// Server Adresse.
            std::string mAddess {LOCALHOST};
            
            /// Server Port.
            uint16_t mPort {0};
            
            /// Server Socket.
            int mServerSocket{-1};
            
            /// Main Server Queue.
            boost::circular_buffer<StructData *> mMainRing;
            
            /// Network information.
            struct sockaddr_in mBroadcastAddr; 

            ///
            Allocator  * mAllocator{nullptr};

            /// this the heartbeat message that will be trasnfered between Sessions 
            intern::HeartBeat         mHeartBeat;
            
        public : 
            
            /**
            * @brief Create a UDP Server.
            *
            * @param aPort the server Port.
            */
            UDPServer (int aPort);
            
            /**
            * Destroy a MarketData Server.
            *
            * @return Destuctor
            */
             ~UDPServer () { delete mAllocator;};

            /**
            * @brief Push data into the async server queue.
            *
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return void
            */
            void push (void * aData, size_t aLength);
            
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual void onPreStart () {}; 
            
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual bool setup () ; 
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual void onStart (){}; 
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual void onConnection (int aConnectionId) {};
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual void onPreClose () {};
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual void onClose () {};  
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual void onSend (size_t aLength ) {
            };
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            virtual void onSendFailure (size_t aLength ) {
            }; 
            
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            bool start ();
            /**
            * Callback from the server thread to process prestart action.
            *
            * @return void
            */
            bool stop ();

            /**
             * @brief 
             * 
             */
            virtual bool Task () { 
                struct timespec lTime;
                core::gClockTime(CLOCK_REALTIME, &lTime);
                mHeartBeat.SENDINGTIME = lTime.tv_nsec;
                push ((void*)&mHeartBeat,sizeof(intern::HeartBeat));
                return true;     
            }

            /**
             * @brief 
             * 
             */
            virtual bool SubTask ()
            { 
                return true;
            }

            void update(Event,void* aData = 0){};
    };
    
    template <uint32_t BUFFER_SIZE ,class Allocator>
    UDPServer<BUFFER_SIZE,Allocator >::UDPServer(int aPort) :Routine(LOCALHOST,1000,true),mMainRing(BUFFER_SIZE),mPort(aPort){
        mAllocator =  new Allocator ();
        mAllocator->Init (1024*1024,64);
        mHeartBeat.TYPE = 'H';
        mHeartBeat.PARENTSESSIONID = mServerId;
        mHeartBeat.SESSIONID = mServerId;
    }  
    
    template <uint32_t BUFFER_SIZE ,class Allocator>
    bool UDPServer<BUFFER_SIZE,Allocator >::setup() {
            mServerSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if (mServerSocket < 0) {
                onError("Cannot create DataGram socket");
                return false;
            }

            memset(&mBroadcastAddr, 0, sizeof(mBroadcastAddr));
            mBroadcastAddr.sin_family = AF_INET;
            mBroadcastAddr.sin_addr.s_addr = inet_addr(LOCALHOST);
            mBroadcastAddr.sin_port = htons(mPort);
            return true;

    }
    
    template <uint32_t BUFFER_SIZE ,class Allocator>
    bool UDPServer<BUFFER_SIZE,Allocator >::stop () {
            onPreClose ();
            Pool::shutdown();
            ::shutdown (mServerSocket,SHUT_RDWR);
            ::close(mServerSocket);
            onClose ();
            return true;
    }
    
    template <uint32_t BUFFER_SIZE ,class Allocator>
    bool UDPServer<BUFFER_SIZE,Allocator >::start() {
        onPreStart ();
        
        auto lFSS = [&](size_t aLength){this->onSend(aLength);};
        auto lFSF = [&](size_t aLength){this->onSendFailure(aLength);};
        Pool::addThread(new core::ASYNC_UDP_Broadcast<> (std::string("Multicast Task"),mMainRing,mServerSocket,mBroadcastAddr,Pool::mCond,lFSS,lFSF));
        
        onStart ();
        return true;
    }
    
    
    template <uint32_t BUFFER_SIZE ,class Allocator>
    void UDPServer<BUFFER_SIZE,Allocator >::push (void * aMessage, size_t aLength) {
         mMutex.lock();
          StructData * lData =(StructData*) mAllocator->Allocate (sizeof(StructData));
          lData->mData = aMessage;
          lData->mLength=aLength;
            mMainRing.push_back(lData);
            Pool::mCond.notify_all();
        mMutex.unlock();
    }

}
#endif
