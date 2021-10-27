
#ifndef ASYNC_TCP_READ_HPP_
#define ASYNC_TCP_READ_HPP_

#include <thread/AsyncTask.hpp>


namespace core {
  /**
    * Implementation of a base Async Read Task to return every new message on a callback
    * @tparam SUCCESS Type of function called in case of a success
    * @tparam FAILURE Type of function called in case of a failure 
    *
    */
    template <class SUCCESS = std::function <void(int,void*,size_t)>,class FAILURE = std::function <void(size_t)> >
    class ASYNC_TCP_Read : public ASYNC_Task<SUCCESS,FAILURE>{
            /// \Task An alias to base task class
            using Task =  ASYNC_Task<SUCCESS,FAILURE>;
        private :
            /// Socket Buffer
            char mBuffer[64*1024];
            
           //Traget socket
           int mSocket{0};
        public :
            /**
            * @brief Construct an Aync Treat task
            *
            * @param aName The connection Id to wish we want to route the message .
            * @param aSock A pointer to the data we want to push.
            * @param aSuc A pointer to the data we want to push.
            * @param aFa The size of the data we want to push .
            */
            ASYNC_TCP_Read (std::string aName,int aSock,SUCCESS aSuc,FAILURE aFa) ;
            
            /**
            * @return default destructor
            */
            ~ASYNC_TCP_Read() = default ;
            
            /**
            * Runnable Implmentation
            *
            * @return void
            */
            void Run () override ;
    };
}
    
    
    template <class SUCCESS ,class FAILURE>
    core::ASYNC_TCP_Read<SUCCESS,FAILURE>::ASYNC_TCP_Read (std::string aId,int aSocket, SUCCESS aSuccess,FAILURE aFailure) : Task(aId,aSuccess,aFailure),mSocket (aSocket){} ;
    
    template <class SUCCESS ,class FAILURE>
    void core::ASYNC_TCP_Read<SUCCESS,FAILURE>::Run (){ 
       // signal(SIGPIPE, SIG_IGN);
       // struct pollfd pfd[1];
       // pfd[0].fd = mSocket;
       // pfd[0].events = POLLIN  | POLLPRI;
       // pfd[0].revents = 0;
        while (!Runnable::IsStop()) {
         //   int rv = ::poll(pfd,1, 1);
          //  if (rv == -1 )
          //  {
           //     Task::mFailure (rv);
           // }
           // else{
            //    if (pfd[0].revents == POLLIN) {
                    int64_t lRecvBytes = ::recv(mSocket , mBuffer , 64*1024, 0);
                    if (lRecvBytes>0)
                    {
                        Task::mSuccess (mSocket,mBuffer,lRecvBytes);
                    }
                    else
                    {
                        Task::mFailure (lRecvBytes);
                    }
              //  }
               // else if (pfd[0].revents == POLLPRI) {
                //    size_t lRecvBytes = ::recv(mSocket , mBuffer , 1024, 0);
                //    if (lRecvBytes>0)
                //    {
                 //       Task::mSuccess (mSocket,mBuffer,lRecvBytes);
                 //       std::cout <<"Pool one"<<mSocket<<" size "  <<lRecvBytes<<std::endl;
                 //   }
                 //   else
                 //   {
                  //      Task::mFailure (lRecvBytes+1);
                  //          std::cout <<"Pool Fail one"<<mSocket<<" size "  <<lRecvBytes<<std::endl;
                   // }
                        
                //}
                //else if (pfd[0].revents == POLLRDHUP){
                 //   Task::mFailure (0);
                 //       std::cout <<"Pool Fail two"<<mSocket<<" size "  <<std::endl;
               // }

         //   }
        }
    } ;
    


#endif


