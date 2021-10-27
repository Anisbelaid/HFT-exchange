#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <logger/OutputListner.hpp>
#include <allocator/LinearAllocator.hpp>
#include <thread/ThreadPool.hpp>
#include <thread/AsyncTCPSend.hpp>
#include <thread/AsyncTCPRead.hpp>
#include <thread/AsyncTreat.hpp>

#include <libexplain/connect.h>
#include <libexplain/close.h>

#include <boost/circular_buffer.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h> 

namespace core {
    
    /**
    * Implementation of a simple asynchronous TCP Client  
    * @tparam BUFFER_SIZE Maximum allowed data in the queues of the connection 
    * @tparam Allocator Custom Allocator type.
    */
    template <uint32_t BUFFER_SIZE = 1024*1024*1024,class Allocator = core::LinearAllocator >
    class TCPClient : public core::ThreadPool ,public OutputListner
    {
        ///\Pool an alias of thread pool context
        using Pool = core::ThreadPool ;
        protected:
            
            /// Custom allocator pointer that will adress all the incoming orders.
            Allocator * mAllocator;
            /// Socket Id.
            int mSock{-1};

            /// check if the session is connected or not
            bool mIsConnected {false};
            
            /// Destination IP Adress
            std::string mInterface{"127.0.0.1"};

            /// Destination IP Adress
            std::string mAddress{"127.0.0.1"};
            
            /// Destination port
            uint16_t mPort{0};
            
            /// old mesHsages to send to the destination 
            boost::circular_buffer<StructData *> mSendRing;
            
             /// Hold messages received from to the destination 
            boost::circular_buffer<StructData *> mRecvRing;
            
            /// Dedicated to protect the access to shared ring buffer
            std::mutex mMutexRead;
            
            /// Dedicated to protect the access to shared ring buffer
            std::mutex mSendMutex;

            struct sockaddr_in mServer;

        public:
            
            /**
            * @brief Create a TCP Client.
            * @param aIp The adress of the destination.
            * @param aPort athe port of the destinaation.
            * @return Constuctor
            */
            TCPClient();
            
            /**
            * Destroy the Connection.
            * @return Destructor
            */
            virtual ~TCPClient() { delete mAllocator;};
            
            /**
            * Configure the Client.
            * @return bool upon a success
            */
            bool setup();
            
            /**
            * Configure the Client.
            * @return bool upon a success
            */
            bool stop();
            
            
            /**
            * Push a message to the async sending queue  .
            * @param aMessage The message to send.
            * @param aLength the length of the message.
            * 
            * @return 
            */
            void push(void * aMessage,size_t aLength);
            
            /**
            * Start the Connection.
            * 
            * @return 
            */
            bool start(const std::string&);
            
            /**
            * @brief Push a message to the async reading queue .
            * @param aId aConnection Id.
            * @param aMessage The message to read.
            * @param aBytes the length of the message.
            * 
            */
           void onRead(int aId,void * aMessage,ssize_t aBytes);
            
            /**
            * @brief  Called upon a read failure  .
            * @param aBytes Hold the error code.
            * 
            */
            void onReadFailure(ssize_t aBytes);
            
            /**
            * @brief Called upon a sending of a message
            * @param aBytes Hold the size of wrote message.
            * 
            * @return 
            */
            void onSend (ssize_t aBytes){};
            
            /**
            * @brief  Called upon a sending failure  .
            * @param aBytes Hold the error code.
            * 
            * @return 
            */
            void onSendFailure (ssize_t aBytes) {} ;
            
            
            /**
            * @brief Ensure that we received the full message to process.
            *
            * @param aConnectionId The ConnectionId.
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return StructData * pointer to a full message
            */
            virtual StructData * ensure (int aConnectionId,void * aData = nullptr,size_t aLength = 0) {return nullptr;}; 
            
            
            /**
            * @brief This method should process the new message to a peer.
            * @param aId 
            * @param aData The final data to process.
            * @return void
            */
            virtual void processMessage (int aId,  StructData * aData)  {};
            
            /**
             * @brief will be called when a peer diiconnnect
             * 
             */
            virtual void onDisconnect () {
                mIsConnected = false;
                Pool::shutdown ();
            };

};


template <uint32_t BUFFER_SIZE,class Allocator >
TCPClient< BUFFER_SIZE, Allocator >::TCPClient(): mSendRing(BUFFER_SIZE),mRecvRing(BUFFER_SIZE)
{
    mAllocator = new Allocator (),
    mAllocator->Init (BUFFER_SIZE,64);
}

template <uint32_t BUFFER_SIZE,class Allocator >
bool TCPClient< BUFFER_SIZE, Allocator >::setup()
{

  	if(mSock == -1)
	{
		mSock = socket(AF_INET , SOCK_STREAM , 0);
		if (mSock == -1)
		{
      		onError ("Could not create socket");
            return false;
        }
    }

  	if((signed)inet_addr(mAddress.c_str()) == -1)
  	{
    		struct hostent *he;
    		struct in_addr **addr_list;
    		if ( (he = gethostbyname( mAddress.c_str() ) ) == NULL)
    		{
                char lMsg[100];
                sprintf (lMsg,"Failed to resolve hostname %s",mAddress.c_str());
                onError (lMsg);
		        return false;
    		}
	   	    addr_list = (struct in_addr **) he->h_addr_list;
    		for(int i = 0; addr_list[i] != NULL; i++)
    		{
      		    mServer.sin_addr = *addr_list[i];
		        break;
    		}
  	}
  	else
  	{
    		mServer.sin_addr.s_addr = inet_addr( mAddress.c_str() );
  	}
  	mServer.sin_family = AF_INET;
  	mServer.sin_port = htons( mPort );
  	return true;
}

template <uint32_t BUFFER_SIZE,class Allocator >
bool TCPClient< BUFFER_SIZE, Allocator >::start (const std::string &aJobName)
{
    if (::connect(mSock , (struct sockaddr *)&mServer , sizeof(mServer)) < 0)
  	{
        onError (::explain_connect(mSock , (struct sockaddr *)&mServer , sizeof(mServer)));
        return false;
  	}
    auto lFSS  = [&](ssize_t aBytes){this->onSend(aBytes);};
    auto lFSF  = [&](ssize_t aBytes){this->onSendFailure(aBytes);};
	Pool::addThread(new ASYNC_TCP_Send<>(aJobName+std::string(" Sending Thread"),mSock,mSendRing,mSendMutex,lFSS,lFSF));
    
    auto lFRS = [&](int Id,void * aMessage,ssize_t aBytes){this->onRead(Id,aMessage,aBytes);}; 
    auto lFRF = [&](ssize_t aBytes){this->onReadFailure(aBytes);};
    Pool::addThread(new ASYNC_TCP_Read<>(aJobName+std::string("Recieving Thread"),mSock,lFRS,lFRF)); 
    
   /* auto lFTS = [&](int aId,void*aMessage,size_t aLength){this->onPacket(aId,aMessage,aLength);};
    auto lFTF = [&](ssize_t aBytes){this->onPacketFailure(aBytes);}; 
    Pool::addThread(new ASYNC_Treat<>(aJobName+std::string("Treating Thread"),mRecvRing,mMutexRead,lFTS,lFTF));*/

    mIsConnected = true;
    
    return true;
} 

template <uint32_t BUFFER_SIZE,class Allocator >
bool TCPClient< BUFFER_SIZE, Allocator >::stop ()
{
    Pool::shutdown();
    ::shutdown(mSock,SHUT_RDWR);
    if (::close(mSock)!= 0)
        onError (::explain_close(mSock));
    return true;
};

template <uint32_t BUFFER_SIZE,class Allocator >
void TCPClient< BUFFER_SIZE, Allocator >::push (void * aMessage,size_t aLength)
{
    mSendMutex.lock();
        if (!mIsConnected)
        {
            mSendMutex.unlock();
            return;
        }
        StructData * lData = (StructData * )mAllocator->Allocate(sizeof(StructData));
        lData->mData     = aMessage;
        lData->mLength   = aLength ;
        lData->mClientId = mSock ;
        mSendRing.push_back (lData);
        Pool::mCond.notify_all();
    mSendMutex.unlock();
}


template <uint32_t BUFFER_SIZE,class Allocator >
void TCPClient< BUFFER_SIZE, Allocator >::onRead(int aId,void * aMessage,ssize_t aBytes)
{
    StructData * lMessage = ensure(aId,aMessage,aBytes);
    while (lMessage) {
        processMessage(aId,lMessage);
        lMessage = ensure(aId);
    }
};



template <uint32_t BUFFER_SIZE,class Allocator >
void TCPClient< BUFFER_SIZE, Allocator >::onReadFailure(ssize_t aBytes){
    if (aBytes == 0)
    {
        onInfo ("Peer Disconnected");
        //mSendMutex.lock();
            onDisconnect();
        //mSendMutex.unlock();
    }
};
    

}
#endif
