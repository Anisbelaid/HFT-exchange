 
#ifndef ASYNC_UDP_READ_HPP_
#define ASYNC_UDP_READ_HPP_

#include <thread/AsyncTask.hpp>

#include <sys/socket.h>

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
 
    template <class SUCCESS = std::function <void(int , void *,size_t)>, class FAILURE = std::function <void(size_t)> >
    class ASYNC_UDP_Read : public ASYNC_Task<SUCCESS,FAILURE>{
            /// \Task An alias to base task class
            using Task = ASYNC_Task<SUCCESS,FAILURE>;
        private:
            /// Read Buffer
            char mBuffer[1024*1024];
            //target socket descriptor
            int mSock {0};
            /// Network Decription
            sockaddr_in& mAdrin;
        public :
            
            /**
            * @brief Construct an Aync Send task
            *
            * @param aName The name of the task .
            * @param aSock The Traget descriptor.
            * @param aAddrin a reference to network description sockaddr_in&
            * @param aSuc A pointer to the data we want to push.
            * @param aFa callcback in case of a failure .
            * @return constructor
            */
            ASYNC_UDP_Read(std::string aName,int aSock,sockaddr_in&aAddrin, SUCCESS aSuc,FAILURE aFa) ;
            
            /**
            * @return default destructor
            */
            ~ASYNC_UDP_Read() = default ;
            
            /**
            * Runnable Implmentation
            *
            * @return void
            */
            void Run () override;
    };
}
    template <class SUCCESS , class FAILURE>
    core::ASYNC_UDP_Read<SUCCESS,FAILURE>::ASYNC_UDP_Read(std::string aId,int aSocket,sockaddr_in& aAdrin, SUCCESS aSuccess,FAILURE aFailure) :Task (aId,aSuccess,aFailure),mSock(aSocket),mAdrin(aAdrin) {
        
    }  ;
    
    template <class SUCCESS , class FAILURE>
    void core::ASYNC_UDP_Read<SUCCESS,FAILURE>::Run()  {
        int len= 0;
        while (!Runnable::IsStop()) {
                int lRecvBytes = ::recvfrom(mSock, mBuffer, 1024*1024,  MSG_WAITALL, ( struct sockaddr *) &(mAdrin),(socklen_t*)&len); 
              
                if (lRecvBytes>0)
                {
                    Task::mSuccess (mSock,mBuffer,lRecvBytes);
                }
                else 
                {
                    Task::mFailure (lRecvBytes);
                }
        }
    }  ;
#endif
