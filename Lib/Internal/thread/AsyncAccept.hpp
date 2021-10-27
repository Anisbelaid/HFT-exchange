
#ifndef ASYNC_ACCEPT_TASK_HPP_
#define ASYNC_ACCEPT_TASK_HPP_

#include <thread/AsyncTask.hpp>

namespace core {
    /**
    * Implementation of a base Async Accept Task to return a new connection id on every connection
    * @tparam SUCCESS Type of function called in case of a success
    * @tparam FAILURE Type of function called in case of a failure 
    *
    */
    template <class SUCCESS = std::function <void(size_t)>,class FAILURE = std::function <void(size_t)> >
    class ASYNC_Accept : public ASYNC_Task<SUCCESS,FAILURE>{
            /// \Task An alias to base task class
            using Task =  ASYNC_Task<SUCCESS,FAILURE>;
        private :
            /// Server Socket Id;
            int mServerSocket{-1};
        public :
            /**
            * Construct an Aync Task
            *
            * @param aName The connection Id to wish we want to route the message .
            * @param aSuc A pointer to the data we want to push.
            * @param aSock A descriptor id.
            * @param aFa The size of the data we want to push .
            * @return constructor
            */
            ASYNC_Accept (std::string aName, int aSock, SUCCESS aSuc, FAILURE aFa) ;
            
            /**
            * @return default destructor
            */
            ~ASYNC_Accept() = default ;
            
            /**
            * Runnable Implmentation
            *
            * @return void
            */
            void Run () override ;
    };
} 
    
    template <class SUCCESS, class FAILURE >
    core::ASYNC_Accept<SUCCESS,FAILURE>::ASYNC_Accept(std::string aId,int aSocket,SUCCESS aSuccess,FAILURE aFailure):Task(aId,aSuccess,aFailure),mServerSocket(aSocket){ } ;
    
    template <class SUCCESS, class FAILURE >
    void core::ASYNC_Accept<SUCCESS,FAILURE>::Run (){     
        socklen_t addr_size;
        
        while (!Runnable::IsStop()){
            struct sockaddr_storage serverStorage;
            addr_size = sizeof (serverStorage);
            int lClientSocket = accept(mServerSocket, (struct sockaddr *) &serverStorage, &addr_size);
            if (lClientSocket >=0)
            {
                Task::mSuccess(lClientSocket);
            }
            else
            {
                Task::mFailure(lClientSocket);
            }
        }
    } ;

#endif
