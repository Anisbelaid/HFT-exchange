#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP


#include <thread/ThreadPool.hpp>
#include <thread/AsyncUDPRead.hpp>
#include <server/Defines.hpp>

#include <boost/circular_buffer.hpp>
#include <libexplain/bind.h>
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
#include <vector>
#include <thread>
#include <cstring>


namespace core {
    
    
    template <uint32_t BUFFER_SIZE = 1024*1024,class Allocator = core::LinearAllocator>
    class UDPClient : public core::ThreadPool , public OutputListner 
    {
        using Pool = core::ThreadPool;
        protected:
            Allocator * mAllocator;
            int mSock{-1};
            std::string mInterface;
            std::string mAddress{""};
            int mPort{0};
            struct sockaddr_in server;
            boost::circular_buffer<StructData *> mRecvRing;
            std::mutex mMutexRead;

        public:
            UDPClient(std::string aInterface,std::string aIp,int aSock);
            
            ~UDPClient() {delete mAllocator;};

            bool setup();

            bool stop(){
                Pool::shutdown();
                ::shutdown(mSock,SHUT_RDWR);
                ::close (mSock);
                return true;
            };
            
            virtual void start(std::string);
            
            void onRead(int aId,void*aMessage,ssize_t aBytes){
                StructData * aData = (StructData *)mAllocator->Allocate (sizeof (StructData));
                aData->mClientId = aId;
                aData->mData = mAllocator->Allocate (aBytes); 
                std::memcpy (aData->mData,aMessage,aBytes);
                aData->mLength = aBytes;
                mMutexRead.lock();
                    mRecvRing.push_back (aData);
                mMutexRead.unlock();
            };
            
            void onReadFailure(ssize_t aBytes){
                if (aBytes == 0)
                    Pool::shutdown();
                else
                    ;
            };
            
            virtual void onPacket(int Id,void * aMessage,size_t aBytes){
            };
            
            void onPacketFailure(ssize_t aBytes){
            };
            
            
};


template <uint32_t BUFFER_SIZE,class Allocator >
UDPClient< BUFFER_SIZE, Allocator >::UDPClient(std::string aInterface,std::string aIp,int aPort) :mInterface(aInterface),mAddress(aIp),mPort(aPort),mRecvRing(BUFFER_SIZE)
{
    mAllocator = new Allocator();
    mAllocator->Init(BUFFER_SIZE,64);
}

template <uint32_t BUFFER_SIZE,class Allocator >
bool UDPClient< BUFFER_SIZE, Allocator >::setup()
{
   struct ip_mreq mreq;
   
   char message[50];

   /* set up socket */
   mSock = socket(AF_INET, SOCK_DGRAM, 0);
   if (mSock < 0) {
     onError("Cannot create socket datagram");
     return false;
   }
   
   bzero((char *)&server, sizeof(server));
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = inet_addr(mAddress.c_str());
   server.sin_port = htons(mPort);
    if (bind(mSock, (struct sockaddr *) &server, sizeof(server)) < 0) {        
        onError (::explain_bind(mSock , (struct sockaddr *)&server , sizeof(server)));
        return false;
    }    
    mreq.imr_multiaddr.s_addr = inet_addr(mAddress.c_str());         
    mreq.imr_interface.s_addr = htonl(mPort);         
    if (setsockopt(mSock, SOL_SOCKET, SO_REUSEADDR,&mreq, sizeof(mreq)) < 0) {
        onError ("Cannot set socket options");
        return false;
    }  
      return true;
}

template <uint32_t BUFFER_SIZE,class Allocator >
void UDPClient< BUFFER_SIZE, Allocator >::start (std::string aJobName)
{
    
    auto lFRS = [&](int aId,void * aMessage,size_t aLength){this->onRead(aId,aMessage,aLength);};
    auto lFRF = [&](size_t aLength){this->onReadFailure(aLength);};
    Pool::addThread(new ASYNC_UDP_Read<>(aJobName+std::string("UDP Client Read"),mSock,server,lFRS,lFRF));
    
    auto lFTS = [&](int aId,void * aMessage,size_t aLength){this->onPacket(aId,aMessage,aLength);};
    auto lFTF = [&](size_t aLength){this->onPacketFailure(aLength);};
    
    Pool::addThread(new ASYNC_Treat<>(aJobName+std::string("UDP Client Treat"),mRecvRing,mMutexRead,lFTS,lFTF));
} 






}
#endif
