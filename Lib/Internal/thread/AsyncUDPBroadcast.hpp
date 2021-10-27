#ifndef ASYNC_UDP_BERADCAST_HPP_
#define ASYNC_UDP_BERADCAST_HPP_

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
#include <poll.h>
#include <netdb.h> 
#include <vector>
#include <netinet/in.h>
#include <cstring>
#include <boost/circular_buffer.hpp>
#include <server/Defines.hpp>
#include <thread/Runnable.hpp>

namespace core {
    
    
    /**
    * Construct an Aync Treat task
    *
    * @param aName The connection Id to wish we want to route the message .
    * @param aSuc A pointer to the data we want to push.
    * @param aBuffer A Buffer to treat.
    * @param aFa The size of the data we want to push .
    * @return constructor
    */
    template <class SUCCESS = std::function<void(size_t)>, class FAILURE = std::function<void(size_t)> ,class DATA = StructData* >
    class ASYNC_UDP_Broadcast : public ASYNC_Task<SUCCESS,FAILURE>
    {
        /// \Task An alias to base task class
        using Task = ASYNC_Task<SUCCESS,FAILURE>;
        private :
            /// Queue to Treat
           boost::circular_buffer<DATA>&mRingBuffer;
           
           /// description of the network
           sockaddr_in& mAdrin;
           
            //Traget socket
           int mSocket{0};
           
           ///
           std::mutex mMutex;
           
           std::condition_variable &mCond;
        public :
            /**
            * @brief Construct an Aync Send task
            *
            * @param aName The name of the task .
            * @param aSock The Traget descriptor.
            * @param aAdrin a reference to network description sockaddr_in&
            * @param aSuc A pointer to the data we want to push.
            * @param aCond A pointer to the data we want to push.
            * @param aBuffer A Buffer to treat.
            * @param aFa callcback in case of a failure .
            * @return constructor
            */
            ASYNC_UDP_Broadcast(std::string aName,boost::circular_buffer<StructData *> &aBuffer ,int aSock,sockaddr_in&aAdrin,std::condition_variable &aCond,SUCCESS aSuc,FAILURE aFa ) ;
             /**
            *
            * @return true if thread should wait
            */
            bool halt (){return (Runnable::IsStop () || !mRingBuffer.empty());}
            
            /**
            * @return default destructor
            */
            ~ASYNC_UDP_Broadcast() = default ;
            
            /**
            * Runnable Implmentation
            *
            * @return void
            */
            void Run () override ;
    };
    
}   
     template <class SUCCESS, class FAILURE,class DATA >
    core::ASYNC_UDP_Broadcast<SUCCESS,FAILURE,DATA>::ASYNC_UDP_Broadcast(std::string aId,boost::circular_buffer<StructData *> &aBuffer,int aSocket,sockaddr_in&aAdrin,std::condition_variable &aCond,SUCCESS aSucess,FAILURE aFailure) : Task (aId,aSucess,aFailure),mRingBuffer(aBuffer),mAdrin(aAdrin),mSocket(aSocket),mCond(aCond) {}  ;
    
    template <class SUCCESS, class FAILURE,class DATA >
    void core::ASYNC_UDP_Broadcast<SUCCESS,FAILURE,DATA>::Run()  {
         while (!Runnable::IsStop()){
            std::unique_lock<std::mutex> lock(mMutex);
            mCond.wait(lock, std::bind(&ASYNC_UDP_Broadcast::halt, this));
            if (mRingBuffer.empty()) continue;
            StructData * lMessageToTreat = mRingBuffer.front();
            mRingBuffer.pop_front();
            if (!lMessageToTreat) continue;
            size_t lSentBytes = ::sendto(mSocket, lMessageToTreat->mData, lMessageToTreat->mLength, 0, (struct sockaddr *)&(mAdrin), sizeof ((mAdrin)));
            if (lSentBytes <=0 ){
                Task::mFailure(lSentBytes);
            }
            else{
                
                Task::mSuccess(lSentBytes);
            }
        }
    }  ;
    
    
       

#endif
