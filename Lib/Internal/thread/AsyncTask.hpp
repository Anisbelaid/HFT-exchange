
#ifndef ASYNC_TCP_TASK_HPP_
#define ASYNC_TCP_TASK_HPP_

#include <server/Defines.hpp>
#include <thread/ThreadPool.hpp>
#include <thread/Runnable.hpp>

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
#include <poll.h>
#include <netdb.h> 
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MIN_MESSAGE_SIZE 176

namespace core {

    
    /**
    * Implementation of a base Async task with two callbeck function /Success and failure inherit from Runnbale class to be launched in single thread
    * @tparam SUCCESS Type of function called in case of a success
    * @tparam FAILURE Type of function called in case of a failure 
    *
    */
    template <class SUCCESS ,class FAILURE >
    class ASYNC_Task : public Runnable{
        protected:
            /// callback of success
            SUCCESS mSuccess;
            /// Callback of a failure
            FAILURE mFailure;
            
        public :
            /**
            * Construct an Aync Task
            *
            * @param aName The connection Id to wish we want to route the message .
            * @param aSuc A pointer to the data we want to push.
            * @param aFa The size of the data we want to push .
            * @return constructor
            */
            ASYNC_Task(std::string aName, SUCCESS aSuc, FAILURE aFa) ;
            
            /**
            * Runnable Implmentation
            *
            * @return void
            */
            virtual void Run () override {};
            
            /**
            * @return default destructor
            */
            ~ASYNC_Task() = default ;
    };

    
    template <class SUCCESS ,class FAILURE >
    ASYNC_Task<SUCCESS,FAILURE>::ASYNC_Task(std::string aName,SUCCESS aSucess,FAILURE aFailure) : Runnable(aName),mSuccess(aSucess),mFailure(aFailure){} ;
    
}

#endif
