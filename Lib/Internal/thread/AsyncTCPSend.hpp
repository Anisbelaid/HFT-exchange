
#ifndef ASYNC_TCP_SEND_HPP_
#define ASYNC_TCP_SEND_HPP_


#include <thread/AsyncTask.hpp>
#include <Trading/OrderReply.hpp>
namespace core {
    /**
    * Implementation of a base Async Treat Task to return every new message on a callback
    * @tparam SUCCESS Type of function called in case of a success
    * @tparam FAILURE Type of function called in case of a failure 
    * @tparam DATA Type of Data to transfer in the buffer  
    * @tparam Buffer Type of the buffer to use 
    */
    template <class SUCCESS = std::function <void(size_t)>,class FAILURE = std::function <void(size_t)> ,typename DATA = StructData* >
    class ASYNC_TCP_Send : public ASYNC_Task<SUCCESS,FAILURE>{
            /// \Task An alias to base task class
            using Task =  ASYNC_Task<SUCCESS,FAILURE>;
        private :
            /// Queue to Treat
           boost::circular_buffer<DATA> &mRingBuffer;
           //Traget socket
           int mSocket{0};
           
           /// mutex to prtotect access to the buffer
           std::mutex &mMutex;
           
           /// condition variable 
           //std::condition_variable &mCond;
        public :
            /**
            * Construct an Aync Send task
            *
            * @param aName The name of the task .
            * @param aSock The Traget descriptor.
            * @param aSuc A pointer to the data we want to push.
            * @param aCond
            * @param aBuffer A Buffer to treat.
            * @param aFa callcback in case of a failure .
            * @return constructor
            */
            ASYNC_TCP_Send (std::string aName,int aSock,boost::circular_buffer<DATA> &aBuffer,std::mutex &aCond,SUCCESS aSuc,FAILURE aFa) ;
            
            /**
            *
            * @return true if threat should wait
            */
            bool halt (){return (!Runnable::IsStop () || !mRingBuffer.full());}
            
            /**
            * @return default destructor
            */
            ~ASYNC_TCP_Send() = default ;
            
            /**
            * Runnable Implmentation
            *
            * @return void
            */
            void Run () override ;
    };
}
    
    
    template <class SUCCESS ,class FAILURE,typename DATA >
    core::ASYNC_TCP_Send<SUCCESS,FAILURE,DATA >::ASYNC_TCP_Send (std::string aId,int aSocket,boost::circular_buffer<DATA> &aBuffer,std::mutex &aCond, SUCCESS aSuccess,FAILURE aFailure) : Task(aId,aSuccess,aFailure),mRingBuffer(aBuffer),mSocket (aSocket),mMutex(aCond){} ;
    
    template <class SUCCESS ,class FAILURE,typename DATA  >
    void core::ASYNC_TCP_Send<SUCCESS,FAILURE,DATA>::Run (){ 
        signal(SIGPIPE, SIG_IGN);
         while (!Runnable::IsStop()){
            StructData * lMessageToSend = nullptr;
            mMutex.lock();
            if (mRingBuffer.empty()) 
            { 
                mMutex.unlock();
                continue;
            }
            else
            {
                lMessageToSend = mRingBuffer.front();
                mRingBuffer.pop_front();
                if (!lMessageToSend) 
                {
                    mMutex.unlock();
                    continue;
                }
                if (!lMessageToSend->mData) 
                {
                    mMutex.unlock();
                    continue;
                }
                if (!lMessageToSend->mLength) 
                {
                    mMutex.unlock();
                    continue;
                }
            }
            
            int64_t lSentBytes = ::send (mSocket,(const void *)lMessageToSend->mData,lMessageToSend->mLength,0);
            if (lSentBytes <=0 ){
                Task::mFailure(lSentBytes);
            }
            else{
                Task::mSuccess(lSentBytes);
            }
            mMutex.unlock();
        }
    } ;
    


#endif

