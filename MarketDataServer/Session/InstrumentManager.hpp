#ifndef  MARKETDATA_INSTRUMENT_MANAGER_HPP_
#define  MARKETDATA_INSTRUMENT_MANAGER_HPP_

#include <container/CustomArray.hpp>

#include <Feed/Instrument.hpp>
#include <Feed/Trade.hpp>

#include <skeleton/InstrumentManager.hpp>

#include <thread/CPUTimer.hpp>
#include <skeleton/Configuration.hpp>
#include <rapidcsv.h>

#include <boost/core/noncopyable.hpp>
#include <cstring>
#include <cstdlib>

namespace marketdata {
    
    template <class Allocator = core::LinearAllocator>
    class InstrumentManager : public core::InstrumentManager<Allocator> ,public core::Routine{
        private:
            core::CPUTimerChecker         mCycleTimer;
                        
            std::mutex mMutexInstrument;
        public: 
            using BaseInstrument= core::InstrumentManager<Allocator>; 
            /**
             * @brief Construct a new Instrument Manager object
             * 
             */
            InstrumentManager (const char *aName,uint64_t aDelay):core::InstrumentManager<Allocator>(),Routine(aName,aDelay,true){
                mCycleTimer.calibrate();
                mCycleTimer.pushRoutine(this);  
             };
             
            ~InstrumentManager ()
            {
                mCycleTimer.shutdown();
            }

             void initialize (){
                Configuration * lConf = dynamic_cast<marketdata::Configuration*> (BaseInstrument::mConfiguration);
                BaseInstrument::mDoc = new rapidcsv::Document(lConf->getReferentialPath());
                BaseInstrument::mAllocator =  new Allocator ();
                BaseInstrument::mAllocator->Init(1024*1024,64);
                BaseInstrument::InitManager ();
             }

           
            void onTrade (void * aTrade) {
                feed::Trade * lTrade = ((feed::Trade*)aTrade);
                core::InstrumentInfo * lInstrumentInfo = getInstrument(lTrade->STOCK) ;
                lInstrumentInfo->setLast(lTrade->PRICE);
            }
            
            void onNewOrder (void *aOrder){
                
            }
            
           core::InstrumentInfo * getInstrument (uint64_t &aKey){
               return BaseInstrument::mInstrumentList[aKey];
            };

                       /**
             * @brief 
             * 
             */
            virtual bool Task () {
                for  (auto lIt : BaseInstrument::mInstrumentList )
                {
                    feed::Instrument * lInstrument =(feed::Instrument * ) BaseInstrument::mAllocator->Allocate (sizeof (feed::Instrument));
                    lInstrument->TYPE='I';
                    lInstrument->STOCK=lIt.second->getKey();
                    strcpy (lInstrument->SYMBOL,lIt.second->getSymbol().c_str());
                    StructData * lData = (StructData*) BaseInstrument::mAllocator->Allocate(sizeof(StructData));
                    lData->mData    = (void *) lInstrument;
                    lData->mLength  = sizeof(feed::Instrument );
                    BaseInstrument::notifyObservers(core::Event::Broadcast,lData);
                }
                return true;
            };

            /**
             * @brief 
             * 
             */
            virtual bool SubTask () {
                
                return true;
            };
    };


}

#endif
