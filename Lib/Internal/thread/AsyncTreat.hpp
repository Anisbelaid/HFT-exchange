
#ifndef ASYNC_TREAT_TASK_HPP_
#define ASYNC_TREAT_TASK_HPP_

#include <thread/AsyncTask.hpp>

namespace core {
  /**
    * Implementation of a base Async Treat Task to return every new message on a callback
    * @tparam SUCCESS Type of function called in case of a success
    * @tparam FAILURE Type of function called in case of a failure 
    * @tparam DATA Type of Data to transfer in the buffer  
    * @tparam Buffer Type of the buffer to use 
    *
    */
    template <class SUCCESS = std::function <void(int,void*,size_t)>,class FAILURE = std::function <void(size_t)> ,class DATA = StructData* >
    class ASYNC_Treat : public ASYNC_Task<SUCCESS,FAILURE>{
            /// \Task An alias to base task class
            using Task =  ASYNC_Task<SUCCESS,FAILURE>;
        private :
            /// Queue to Treat
           boost::circular_buffer<DATA>&mRingBuffer;
           
           ///Mustex used to protect treating thread
           std::mutex &mMutex;
           
           ///
          // std::condition_variable &mCond;
        public :
            /**
            * Construct an Aync Treat task
            *
            * @param aName The connection Id to wish we want to route the message .
            * @param aSuc A pointer to the data we want to push.
            * @param aCond 
            * @param aBuffer A Buffer to treat.
            * @param aSuc The size of the data we want to push .
            * @param aFa The size of the data we want to push .
            * @return constructor
            */
            ASYNC_Treat (std::string aName,boost::circular_buffer<DATA> &aBuffer, std::mutex &aCond,SUCCESS aSuc,FAILURE aFa) ;
            
            /**
            *
            * @return true if threat should wait
            */
            bool halt (){return (Runnable::IsStop () || !mRingBuffer.empty());}
            
            /**
            * @return default destructor
            */
            ~ASYNC_Treat() = default ;
            
            /**
            * Runnable Implmentation
            *
            * @return void
            */
            void Run () override ;
    };
}
    
    
    template <class SUCCESS ,class FAILURE,class DATA >
    core::ASYNC_Treat<SUCCESS,FAILURE,DATA>::ASYNC_Treat (std::string aId,boost::circular_buffer<DATA> &aBuffer, std::mutex &aCond, SUCCESS aSuccess,FAILURE aFailure) : Task(aId,aSuccess,aFailure),mRingBuffer(aBuffer),mMutex(aCond){} ;
    
    template <class SUCCESS ,class FAILURE,class DATA >
    void core::ASYNC_Treat<SUCCESS,FAILURE,DATA>::Run (){ 
          while (!Runnable::IsStop()){
                    mMutex.lock();
                    StructData * lMessageToTreat =  nullptr;
                    if (mRingBuffer.empty()) 
                    {
                        mMutex.unlock();
                        continue;
                    }
                    lMessageToTreat = mRingBuffer.front();
                    mRingBuffer.pop_front();
                    
                    if (lMessageToTreat)
                    {
                        Task::mSuccess(lMessageToTreat->mClientId,lMessageToTreat->mData,lMessageToTreat->mLength);
                    }
                    else
                    {
                        Task::mFailure((size_t)lMessageToTreat);
                    }
                    mMutex.unlock();
                }
    } ;
    


#endif
