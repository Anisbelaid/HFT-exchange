#ifndef  MARKET_ORDER_HPP_
#define  MARKET_ORDER_HPP_

#include <functionnal/OrderManager.hpp>

#include <container/CustomArray.hpp>

#include <Trading/Order.hpp>

#include <cstdint>


namespace marketdata {
    
    class OrderManager : public core::OrderManager <> , Decoder<> 
    {                    
        public :
            
            OrderManager () {}

            virtual void onMessage (void *aMessage,size_t aSize=0) {
                Decoder<>::Decode (aMessage,aSize);
            };

            void TreatNewOrder (void*aMessage) {
                INFO(MARKETDATA,"Order Book ::TreatNewOrder "<<*(trading::Order *)aMessage);
                onNewOrder((trading::Order *)aMessage);
            };
            
            void TreatNewCancel (void*aMessage) {
                //INFO(Gateway,"Market Order ::TreatNewCancel "<<*(trading::Cancel *)aMessage);
                onCancelOrder((trading::Cancel *)aMessage);
            };

            virtual void onThreadAdded (const char *aMessage) {
                 INFO(MARKETDATA,"Market Order ::onThreadId "<<aMessage);
            };
            virtual void onThreadError (const char *aMessage) {
                 ERRORR(MARKETDATA,"Market Order  ::onThreadError " <<aMessage);
            };
            virtual void onCloseThread (const char *aMessage) {
                 INFO(MARKETDATA,"Market Order  ::onCloseThread "<<aMessage);
            };
            
            virtual void onNewOrder (trading::Order *aOrder){};

            virtual void onCancelOrder (trading::Cancel *aCancel) 
            {
                feed::Cancel *lCancel = (feed::Cancel *)mAllocator->Allocate(sizeof(feed::Order));

                lCancel->TYPE = 'Y';
                lCancel->ORDERID   = aCancel->ORDERID;
                lCancel->BUYSELL   = aCancel->BUYSELL;
                lCancel->STOCK     = aCancel->STOCK;
                StructData * lData = (StructData*) mAllocator->Allocate(sizeof(StructData));
                lData->mData    = (void*) lCancel;
                lData->mLength  = sizeof(feed::Cancel);
                notifyObservers(core::Event::Broadcast,lData);
            };

            virtual void onTwap (trading::Order *) {};
            
    };

}
#endif


