#ifndef _OBSERVER_HPP_
#define _OBSERVER_HPP_


#include <cstdint>
#include <vector>
#include <algorithm>
#include <server/Defines.hpp>

namespace core
{

    enum class Event  {
        StartServer, 
        ServerShutdown,
        ToMatchingEngine,
        ToDispatcher,
        ToGateway,
        ToLogger,
        ToMarketData,
        ToNode,
        FromMatchingEngine,
        FromGateway,
        FromDispatcher,
        FromMarketData,
        NewClient,
        Subscription,
        UnSubscription,
        ClientDisconnection,
        FromServerSession,
        ServerDisconnection,
        Broadcast,
        NewWindow,
        DeleteWindow,
        WindowClosed,
        WindowOpened,
        NewOrder,
        CancelOrder,
    };

     /**
     * @brief Interface for the Observer
     */
    class Observer {

    public:
        
        virtual ~Observer() {} ;

        /**
         * Update the state of this observer
         */
        virtual void update(Event,  StructData * aData = nullptr) = 0;

        /**
         * Update the state of this observer
         */
        virtual void update(Event,void* aData = 0) = 0;

        /**
         * Update the state of this observer
         */
        virtual void update(Event,uint64_t aData = 0) = 0;

    };

    /**
     * @brief an interface for subject that will observed
     * 
     */
    class Subject {
        protected:
             typedef std::vector <Observer*> Observers_; 
            ///list of all session observers
            Observers_ mObservers;
        public:
            virtual ~Subject() {} ;
            /**
             * Register an observer
             * @param observer the observer object to be registered
             */
            void registerObserver(Observer *aObserver) {
                mObservers.push_back (aObserver);
            };

            /**
             * Unregister an observer
             * @param observer the observer object to be unregistered
             */
            void removeObserver(Observer *aObserver) {
                Observers_::iterator lIt = std::find (mObservers.begin(),mObservers.end(),aObserver);
                if (lIt != mObservers.end())
                    mObservers.erase (lIt);
            };

            /**
             * Notify all the registered observers when a change happens
             */
            void notifyObservers(Event aEvent,  StructData * aData = nullptr) {
                for (auto lObserver : mObservers){
                    lObserver->update(aEvent,aData);
                }
            }


             /**
             * Notify all the registered observers when a change happens
             */
            void notifyObservers(Event aEvent,void* aElement) {
                for (auto lObserver : mObservers){
                    lObserver->update(aEvent,aElement);
                }
            }

             /**
             * Notify all the registered observers when a change happens
             */
            void notifyObservers(Event aEvent,uint64_t aId) {
                for (auto lObserver : mObservers){
                    lObserver->update(aEvent,aId);
                }
            }
    };


   
}

#endif