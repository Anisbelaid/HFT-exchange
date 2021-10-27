#ifndef  _INSTRUMENT_MANAGER_HPP_
#define  _INSTRUMENT_MANAGER_HPP_

#include <container/CustomArray.hpp>
#include <pattern/Singleton.hpp>

#include <Feed/Instrument.hpp>
#include <Feed/Trade.hpp>

#include <skeleton/InstrumentInfo.hpp>
#include <skeleton/Configuration.hpp>
#include <rapidcsv.h>

#include <boost/core/noncopyable.hpp>
#include <cstring>
#include <map>


namespace core {
    
    template <class Allocator = core::LinearAllocator>
    class InstrumentManager : public Subject {

        protected:
            typedef std::map<uint64_t,InstrumentInfo*> InstrumentList_;
            
            /// Instrument container 
            InstrumentList_ mInstrumentList;    

            ///Allocator
            Allocator  *mAllocator {nullptr};
            
            /// Document Parser
            rapidcsv::Document * mDoc{nullptr};

            /// a pointer to the general configuration
            Configuration *         mConfiguration{nullptr};

            
        public: 
             
            /**
             * @brief Construct a new Instrument Manager object
             * 
             */
            InstrumentManager ():mDoc(nullptr){
                 
             };

            /**
             * @brief 
             * 
             */
            void setConfiguration (Configuration * aConfiguration)
            {
                mConfiguration=aConfiguration;
            }
            /**
             * @brief Destroy the Instrument Manager object
             * 
             */
            virtual ~InstrumentManager (){
                 delete mAllocator;
                 delete mDoc;
             };

            /**
             * @brief 
             * 
             * @param aReferentialPath 
             */
             virtual void initialize (){
             }

             
            /**
             * Initilize manager with the specified file 
             * */
            virtual void InitManager (){
                std::vector<uint64_t> lIstrumentKeys =  mDoc->GetColumn<uint64_t>("INSTRUMENTKEY");
                std::vector<std::string> lIstrumentName =  mDoc->GetColumn<std::string>("INSTRUMENTNAME");
                for (int i=0;i<lIstrumentKeys.size();i++){
                    addInstrument (lIstrumentKeys[i],lIstrumentName[i]);
                }
            }
           
            virtual void onTrade (void * aTrade) {
                feed::Trade * lTrade = ((feed::Trade*)aTrade);
                core::InstrumentInfo * lInstrumentInfo = getInstrument(lTrade->STOCK) ;
                lInstrumentInfo->setLast(lTrade->PRICE);
            }
            
            virtual void onNewOrder (void *aOrder){
                
            }
            
            
            virtual void addInstrument (const uint64_t &aKey,const std::string &aName){
                core::InstrumentInfo * lInstrumentInfo =(core::InstrumentInfo *) mAllocator->Allocate (sizeof (core::InstrumentInfo ));
                lInstrumentInfo->setKey(aKey);
                lInstrumentInfo->setSymbol(aName);
                mInstrumentList.insert(std::make_pair(aKey,lInstrumentInfo));
            }
            
           core::InstrumentInfo * getInstrument (uint64_t &aKey){
               return mInstrumentList[aKey];
            };
    };


}

#endif
