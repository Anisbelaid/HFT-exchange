#ifndef  _ORDER_MANAGER_HPP_
#define  _ORDER_MANAGER_HPP_

#include <functionnal/OrderManager.hpp>

#include <container/CustomArray.hpp>

#include <Trading/Order.hpp>

#include <cstdint>


namespace matching {
    
    class OrderManager : public core::OrderManager <> , Decoder<> 
    {                    
        public :
            
            OrderManager () {}

            virtual void onMessage (void *aMessage,size_t aSize=0) {
                Decoder<>::Decode (aMessage,aSize);
            };

            void TreatNewOrder (void*aMessage) {
                INFO(Matching,"Order Book ::TreatNewOrder "<<*(trading::Order *)aMessage);
                onNewOrder((trading::Order *)aMessage);
            };
            
            void TreatNewCancel (void*aMessage) {
                INFO(Matching,"Market Order ::TreatNewCancel ");
                onCancelOrder((trading::Cancel *)aMessage);
            };

            virtual void onThreadAdded (const char *aMessage) {
                 INFO(Matching,"Market Order ::onThreadId "<<aMessage);
            };
            virtual void onThreadError (const char *aMessage) {
                 ERRORR(Matching,"Market Order  ::onThreadError " <<aMessage);
            };
            virtual void onCloseThread (const char *aMessage) {
                 INFO(Matching,"Market Order  ::onCloseThread "<<aMessage);
            };
            
            virtual void onNewOrder (trading::Order *aOrder);

            virtual void onCancelOrder (trading::Cancel *);

            virtual void onTwap (trading::Order *) {};
            
    };

    void OrderManager::onCancelOrder (trading::Cancel *aOrder)
    {
        trading::Order *lOrder= mOrderList[aOrder->ORDERID];
        TradeHandlerList_::iterator lIt = mTradeHandlerList.find(aOrder->STOCK);
        if (lIt!=mTradeHandlerList.end())
        {
            if (lOrder->BUYSELL=='B')
                mTradeHandlerList[aOrder->STOCK]->onRemoveBuyOrder (lOrder);
            else
                mTradeHandlerList[aOrder->STOCK]->onRemoveSellOrder (lOrder);
        }
        
        mOrderList.erase (aOrder->ORDERID);
    }
     void OrderManager::onNewOrder (trading::Order *aOrder)
     {
        TradeHandlerList_::iterator lIt = mTradeHandlerList.find(aOrder->STOCK);
        core::TradeHandler<>* lHandler= nullptr;
        if (lIt==mTradeHandlerList.end())
        {
            lHandler = new  core::TradeHandler<> ();
            lHandler->registerObserver(this);
            mTradeHandlerList[aOrder->STOCK] = lHandler;
        }
        else
        {
            lHandler = lIt->second;
        }

        if (((trading::Order *)aOrder)->BUYSELL == 'B')
        {
            mOrderList.insert(std::make_pair(aOrder->ORDERID,aOrder));
            aOrder->REMAININGSHARES = aOrder->SHARES;
            lHandler->Match (aOrder);
            if (aOrder->REMAININGSHARES!=0) 
                lHandler->onBuyOrder(aOrder);
            return ;
        }
        else if (((trading::Order *)aOrder)->BUYSELL == 'S')
        {
                mOrderList.insert(std::make_pair(aOrder->ORDERID,aOrder));
                aOrder->REMAININGSHARES = aOrder->SHARES;
                lHandler->Match (aOrder);
                if (aOrder->REMAININGSHARES!=0) 
                    lHandler->onSellOrder(aOrder);
                return ;
        }
    }

}
#endif


