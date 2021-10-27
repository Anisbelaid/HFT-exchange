#ifndef _SEQUENCER_HPP_
#define _SEQUENCER_HPP_

#include <Trading/Order.hpp>
#include <Application/Defines.hpp>
#include <logger/OutputListner.hpp>
#include <server/Defines.hpp>
#include <thread/ThreadPool.hpp>
#include <pattern/Observer.hpp>

#include <container/LockFreeQueue.hpp>
#include <container/CustomArray.hpp>

#include <boost/core/noncopyable.hpp>

#include <mutex>
#include <map>

namespace  core
{
    /**
    * Implementation of a sequencer that will ensure that only the last element is treated from the all clients
    * @tparam MAX_CONN Maximum connection allowed in the sequencer 
    *
    */
    template <uint16_t CONNECTION_MAX_INDEX = 10>
    class Sequencer  : public core::ThreadPool , public core::Runnable , public OutputListner {
        
        /// \QueuesList_ A map holding pointers to message queues by clientIds
        typedef std::map<  uint64_t,core::LockWaitFreeQueue<1024*1024,StructData>*> QueuesList_;
        private :
            
            /// mutex 
            std::mutex mMutex; 
            
            /// Map holding queues
            QueuesList_ mQueues;
            
            ///This  indicatoe ti the next client to read
            QueuesList_::iterator mLookupIndex;
            
        public : 

            /**
            * Create a new sequencer .
            *
             * @return Constuctor
            */
            Sequencer (){
                mLookupIndex = mQueues.begin();
            }; 

            /**
            * Destroy a sequencer.
            *
             * @return Constuctor
            */
            virtual ~Sequencer (); 
            
            /**
            * Push data into the async server queue.
            *
            * @param aConnectionId The connection Id to wish we want to route the message .
            * @param aData A pointer to the data we want to push.
            * @return bool
            */
            bool push (  StructData *aData);
            
            /**
            * add a new Connection to the sequencer.
            *
            * @param aClientId Connection id .
            * @param aRatio
            * @return bool
            */
             bool subscribe (uint64_t aClientId,uint64_t aRatio);
             
             
            /**
            * add a new Connection to the sequencer.
            *
            * @param aClientId Connection id .
            * @return bool
            */
            bool unSubscribe (uint64_t aClientId);
            
            /**
            * Get all data in the queue in the sequencer data.
            * 
            * 
            * @return the oldest order
            */
            StructData * consume ();

            /**
             * @brief 
             * 
             */
            void Run () {
                  StructData * lStruct = nullptr;
                while (!Runnable::IsStop()){   
                    while  ((mLookupIndex != mQueues.end())&&mLookupIndex->second->CanPoll()) 
                    //while  ((mLookupIndex != mQueues.end())) 
                    {
                        if (mLookupIndex->second->poll(lStruct))
                            if (lStruct){
                                onMessage(lStruct->mData,lStruct->mLength);
                                lStruct = nullptr;
                            }
                    }
                    mMutex.lock();
                        if (mLookupIndex == mQueues.end()) 
                            mLookupIndex = mQueues.begin();
                        else
                            ++mLookupIndex;
                    mMutex.unlock(); 
                    
                }
            }

            /**
             * @brief 
             * 
             * @param aMessage 
             * @param aSize 
             */
            virtual void onMessage (void *aMessage,size_t aSize=0) = 0;
            
            /**
             * @brief 
             * 
             * @return true 
             * @return false 
             */
            void launch (){
                core::ThreadPool::addThread(this);
            }

            
    };
    
}  
    
    template <uint16_t CONNECTION_MAX_INDEX>
    core::Sequencer<CONNECTION_MAX_INDEX>::~Sequencer() {
        for (auto i = mQueues.begin(); i!=mQueues.end();i++)
        {
            if (i->second)
                delete i->second;
        }
    }  
        
    template <uint16_t CONNECTION_MAX_INDEX>
    bool core::Sequencer<CONNECTION_MAX_INDEX>::push (  StructData *aData) {
        if (mQueues.find(aData->mClientId)!= mQueues.end()){
            mQueues[aData->mClientId]->push(aData);
            return true;
        }
        return false;
    }
    
   
    template <uint16_t CONNECTION_MAX_INDEX>
    bool core::Sequencer<CONNECTION_MAX_INDEX>::subscribe(uint64_t aClientID,uint64_t aRatio) {
        if (mQueues.size() == CONNECTION_MAX_INDEX)
        {
            onDebug("Max Subscription index Reached");
            return false;
        }
        else if (mQueues.find (aClientID) == mQueues.end()){
            onDebug("New Subscription ");
            mMutex.lock();
                mQueues.insert(std::make_pair(aClientID,new core::LockWaitFreeQueue<1024*1024,StructData>(aRatio)));
                mLookupIndex = mQueues.begin ();
            mMutex.unlock();
            return true;
        }
        else
        {   
            onDebug("Subscription already registred");
            return false;
        }
    }
    
   
    template <uint16_t CONNECTION_MAX_INDEX>
    bool core::Sequencer<CONNECTION_MAX_INDEX>::unSubscribe(uint64_t aClientID) {
           if (mQueues.find(aClientID) != mQueues.end()){
                mMutex.lock();
                    delete mQueues[aClientID];
                    onDebug("Client subscription removed ");
                mMutex.unlock();
           }
           else
           {
               onDebug("Client not found ");
           }
        return true;
    }
#endif
