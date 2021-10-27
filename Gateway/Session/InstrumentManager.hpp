#ifndef  GATEWAY_INSTRUMENT_MANAGER_HPP_
#define  GATEWAY_INSTRUMENT_MANAGER_HPP_

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

namespace gateway {
    
    template <class Allocator = core::LinearAllocator>
    class InstrumentManager : public core::InstrumentManager<Allocator> {
        public: 
            using BaseInstrument= core::InstrumentManager<Allocator>; 
            /**
             * @brief Construct a new Instrument Manager object
             * 
             */
            InstrumentManager ():core::InstrumentManager<Allocator>(){ 
             };
             
            ~InstrumentManager ()
            {
            }

             void initialize (){
                Configuration * lConf = dynamic_cast<gateway::Configuration*> (BaseInstrument::mConfiguration);
                BaseInstrument::mDoc = new rapidcsv::Document(lConf->getReferentialFilePath());
                BaseInstrument::mAllocator =  new Allocator ();
                BaseInstrument::mAllocator->Init(1024*1024,64);
                BaseInstrument::InitManager ();
             }

           
            void onTrade (void * aTrade) {
                feed::Trade * lTrade = ((feed::Trade*)aTrade);
                core::InstrumentInfo * lInstrumentInfo = getInstrument(lTrade->STOCK) ;
                lInstrumentInfo->setLast(lTrade->PRICE);
            }
            
           core::InstrumentInfo * getInstrument (uint64_t &aKey){
               return BaseInstrument::mInstrumentList[aKey];
            };
    };


}

#endif
