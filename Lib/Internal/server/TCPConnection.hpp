#ifndef _TCP_CONNECTION_HPP_
#define _TCP_CONNECTION_HPP_

#include <thread/ThreadPool.hpp>

#include <thread/AsyncTCPRead.hpp>
#include <thread/AsyncTCPSend.hpp>
#include <thread/AsyncTreat.hpp>
#include <allocator/LinearAllocator.hpp>
#include <logger/OutputListner.hpp>
#include <libexplain/close.h>

#include <boost/circular_buffer.hpp>

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

#include <Trading/OrderReply.hpp>
namespace core {
    
    // TODO Check a replacement of the priority queue
    /**
    * Implementation of a generic TCP Connection allowing to push and poll data asynchronously 
    * @tparam BUFFER_SIZE Maximum allowed data in the queues oof the connection 
    * @tparam Allocator Custom Allocator type.
    */
    template <uint32_t BUFFER_SIZE = 1024*1024 ,class Allocator = core::LinearAllocator >
    class TCPConnection : public core::ThreadPool,public core::OutputListner{
        
        /// \Pool alias to the containing thread pool 
        using Pool = core::ThreadPool;
    protected:
            /// Socket Id.
            int mClientSocket{0};

            /// Custom allocator pointer that will adress all the incoming orders.
            Allocator * mAllocator;

             /// check if the session is connected or not
            bool mIsConnected {false};

            /// Sending queue.
            boost::circular_buffer<StructData *>mSendRing;  
            
            /// will be used to protect sending buffer
            std::mutex mSendMutex;
            
            /// will be used to protect recv buffer
            std::mutex mRecvMutex;

        public :
            
            /**
            * @brief Create a TCP Connection.
            * @param aSock 
            * @return Constuctor
            */
            explicit TCPConnection(int) ;
            
            /**
             * @brief Destroy the TCPConnection object
             * 
             */
            virtual ~TCPConnection ();
            
            /**
            * @brief  poll data into from the connection and insert it into the treating async connection queue.
            *
            * @param aConnectionId The connection Id tof the incoming the message .
            * @param aData A pointer to the data we want to poll.
            * @param aLength The size of the data we want to poll .
            */
            void poll (int aConnectionId,void *aData, size_t aLength);
            
            /**
            * @brief  Callback received if we fail to read data from socket.
            *
            * @param aLength The size of the data we want to poll .
            */
            void pollFailure (size_t aLength);
            
            
            /**
            * @brief Callback triggered when we send data.
            *
            * @param aLength The size of the data we sent .
            */
            virtual void onSend (size_t aLength){ };
            
            /**
            * @brief Callback triggered when we fail to send send data.
            *
            * @param aLength The size of the data we sent .
            */
            virtual void onSendFailure (size_t aLength){};
        
            /**
            *  @brief Push data into the async connection queue.
            *
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            */
            virtual void push (void *aData,ssize_t aLength);
            
            /**
            * @brief Ensure that we received the full message to process.
            *
            * @param aConnectionId The ConnectionId.
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return StructData * pointer to a full message
            */
            virtual StructData * ensure (int aConnectionId,void * aData = nullptr,size_t aLength = 0) = 0; 
            
            
            /**
            * @brief This method should process the new message to a peer.
            * @param aId 
            * @param aData The final data to process.
            */
            virtual void processMessage (int aId,  StructData * aData)  = 0; 

            /**
             * @brief will be called when a peer diiconnnect
             * 
             */
            virtual void onDisconnect () {
                mIsConnected = false;
                Pool::shutdown ();
            };
            
            /**
            * @brief Used to start a server .
            */
            void start ();
            
            /**
            * @brief Used to stop a server .
            */
            bool stop ();

            
            
    };

    
    template <uint32_t BUFFER_SIZE,class Allocator >
    TCPConnection<BUFFER_SIZE,Allocator >::TCPConnection(int aSock) : mClientSocket(aSock),mSendRing(BUFFER_SIZE){
        mAllocator = new Allocator ();
        mAllocator->Init (BUFFER_SIZE,64);
    };
    
    template <uint32_t BUFFER_SIZE,class Allocator >
    TCPConnection<BUFFER_SIZE,Allocator >::~TCPConnection (){
        delete mAllocator;
    };
 
    
    template <uint32_t BUFFER_SIZE,class Allocator >
    void TCPConnection<BUFFER_SIZE,Allocator >::push (void *aData,ssize_t aLength) {
        mSendMutex.lock();
            if (!mIsConnected)
            {
                return mSendMutex.unlock();
            }
            StructData *lData  =  (StructData*)mAllocator->Allocate (sizeof (StructData));
            lData->mData = aData;
            lData->mLength = aLength;
            lData->mClientId = mClientSocket;
            mSendRing.push_back (lData); 
            Pool::mCond.notify_all();
        mSendMutex.unlock();  
    }
    
    
    template <uint32_t BUFFER_SIZE,class Allocator >
    void TCPConnection<BUFFER_SIZE,Allocator >::start()  {
        
        using namespace std::placeholders;
        
        auto lFRS =[&](int aConnection,void *aMessage,size_t aLength){this->poll(aConnection,aMessage,aLength);}; 
        
        auto lFRF =[&](size_t aLength){this->pollFailure(aLength);};
        
        Pool::addThread(new ASYNC_TCP_Read<>(std::string("From Client Reading Task"),mClientSocket,lFRS,lFRF));
            
        auto lFSS= [&](size_t aLength){this->onSend(aLength);};
        
        auto lFSF = [&](size_t aLength){this->onSendFailure(aLength);};
        
        Pool::addThread(new ASYNC_TCP_Send<std::function <void (size_t)> >(std::string("To Client Send Task"),mClientSocket,mSendRing,mSendMutex,lFSS,lFSF));
        
        mIsConnected = true;
    }
    
    template <uint32_t BUFFER_SIZE,class Allocator >
    bool TCPConnection<BUFFER_SIZE,Allocator >::stop() {
        onInfo ((std::string ("Closing connection <")+std::to_string(mClientSocket)+std::string(">")).c_str());
        ::shutdown(mClientSocket,SHUT_RDWR);
        if (::close(mClientSocket)!= 0)
        {
            onError (::explain_close(mClientSocket));
            return false;
        }
        onDisconnect();
        return true;
    }
      
    template <uint32_t BUFFER_SIZE,class Allocator >
    void TCPConnection<BUFFER_SIZE,Allocator >::poll (int aClientId,void *aData,size_t aLength ) {
          StructData * lMessage = ensure(aClientId,aData,aLength);
        while (lMessage) {
            processMessage(aClientId,lMessage);
            lMessage = ensure(aClientId);
        }
    }; 
    
    template <uint32_t BUFFER_SIZE,class Allocator >
    void TCPConnection<BUFFER_SIZE,Allocator >::pollFailure (size_t aCode)
    {    
        if (aCode == 0)
        {
            onInfo ("Peer Disconnected");
            onDisconnect();
        }
    }

}

      
#endif
