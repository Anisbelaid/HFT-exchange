#ifndef _LOCK_FREE_QUEUE
#define _LOCK_FREE_QUEUE

#include <boost/core/noncopyable.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <atomic>
namespace core {
    
    /**
    * Implementation of a generic Lock free wait free queue 
    * @tparam CAPACITY max capacity of the queue.
    * @tparam ELEMENT type of the elment that will be pushed/poped  in the queue 
    *
    */
    template <uint64_t CAPACITY, class ELEMENT>
    class LockWaitFreeQueue :private boost ::noncopyable {

        private:
            /// a lock free queue propsed by boost 
            boost::lockfree::spsc_queue<  ELEMENT*, boost::lockfree::capacity<CAPACITY> > mQueue;
            std::atomic_uint_fast64_t mRatio ;
            std::atomic_uint_fast64_t mCounter ;
        public :
            
            /**
            * Create a lock wait/free queue.
            *
            * @return default constuctor
            */
            explicit LockWaitFreeQueue (uint64_t aRatio){
                mRatio.store(aRatio,std::memory_order_acquire);
                mCounter.store (0,std::memory_order_acquire);
            }
            
            /**
            * Destroy a lock wait/free queue.
            *
            * @return default destuctor
            */
            ~LockWaitFreeQueue () = default;
            
            
            /**
            * Push a message to the async sending queue  .
            * @param aData The message to put in the queue.
            * 
            * @return 
            */
            bool push(  ELEMENT * aData);
            
            /**
            * Push a message to the async sending queue  .
            * @param aData The message to populate.
            * 
            * @return true if is not empty
            */
            bool poll(  ELEMENT *& aData);
            
            /**
             * @brief 
             * 
             * @return true 
             * @return false 
             */
            bool CanPoll (){
                if (mCounter.load()<=mRatio.load())
                {
                    mCounter.fetch_add(1,std::memory_order_relaxed);
                    return true;
                }
                else
                {
                    mCounter.store (0,std::memory_order_acquire);
                    return false;
                }
            }
           
    };
}

template <uint64_t CAPACITY, class ELEMENT>
bool core::LockWaitFreeQueue<CAPACITY,ELEMENT>::push (  ELEMENT *aData)
{
    //if (!spsc_queue.is_lock_free())
    // keep looping until element os pushed into the queue 
    // this loop should have one iteration
    while (mQueue.write_available() <= 0 || !mQueue.push(aData)){}
    return true;
}

template <uint64_t CAPACITY,class ELEMENT>
bool core::LockWaitFreeQueue<CAPACITY,ELEMENT>::poll (  ELEMENT *&aData) {
    //typename boost::lockfree::spsc_queue<ELEMENT*, boost::lockfree::capacity<CAPACITY> >::iterator lIt;
    //if (mQueue.read_available() > 0) 
    {
        return mQueue.pop(aData);

        //while (lIt!=mQueue.end())
       // {
       //     std::cout <<*lIt<<std::endl;
       // }
    }
    
    //return false;
}

#endif
