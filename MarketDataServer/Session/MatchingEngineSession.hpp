
#ifndef _TRADING_SESSION_HPP_
#define _TRADING_SESSION_HPP_


#include <server/TCPConnection.hpp>


namespace marketdata {

   /**
    * Implementation of the TCPConnection class to add functionnal layer  
    */
    class TradingSession : public core::TCPServerSession<> {

        using TCPSession = core::TCPServerSession<>;
        
        public :
            
            /**
            * @brief Create a new session.
            * @param aConnectionId a Connection Id.
            * @param aSockId 
            * @param aContext 
            * @return Constuctor
            */
            TradingSession (int aSockId,uint64_t aSessionName) : TCPSession(aSockId,aSessionName){
            };

            void TreatNewOrder (void*aMessage) {
               INFO(MARKETDATA,"MARKETDATA Server Session ::TreatNewOrder New order received ");
                StructData * lData = (StructData*) mAllocator->Allocate(sizeof(StructData));
                lData->mData    = aMessage;
                lData->mLength  = sizeof(feed::Order);
                notifyObservers(core::Event::Broadcast,lData);
            };

            void TreatAddOrder (void*aMessage) {
               INFO(MARKETDATA,"MARKETDATA Server Session ::TreatAddOrder New order received ");
                StructData * lData = (StructData*) mAllocator->Allocate(sizeof(StructData));
                lData->mData    = aMessage;
                lData->mLength  = sizeof(feed::Order);
                notifyObservers(core::Event::Broadcast,lData);
            };

            void TreatExecution (void *aTrade)
            {
                INFO(MARKETDATA,"Matching Engine Session ::TreatExecution Session Id <"<<mSessionName<<">");
                trading::Exec* lExec=(trading::Exec*)aTrade;
                feed::Trade *lTrade = (feed::Trade*) mAllocator->Allocate(sizeof(feed::Trade));
                lTrade->TYPE='P';
                lTrade->STOCK=lExec->STOCK;
                lTrade->MATCHNUMBER=lExec->MATCHNUMBER;
                lTrade->SHARES=lExec->EXECUTEDSHARES;
                lTrade->PRICE=lExec->EXECUTEDPRICE;
                StructData * lData = (StructData*) mAllocator->Allocate(sizeof(StructData));
                lData->mData    = (void*) lTrade;
                lData->mLength  = sizeof(feed::Trade);
                notifyObservers(core::Event::Broadcast,lData);
            }

            void TreatNewCancel (void*aMessage) {
                INFO(MARKETDATA,"MARKETDATA Server Session ::TreatNewCancel New cancel received ");
            };

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                 DEBUGG(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(MARKETDATA,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(MARKETDATA,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(MARKETDATA,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(MARKETDATA,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(MARKETDATA,"::onCloseThread "<<aMessage);
            };
            
    };
    
}

#endif


