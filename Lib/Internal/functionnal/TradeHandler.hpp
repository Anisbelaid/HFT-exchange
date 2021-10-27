#ifndef _TRADE_HANDLER_HPP_
#define _TRADE_HANDLER_HPP_

#include <Application/Defines.hpp>

#include <Trading/Trade.hpp>
#include <Trading/Order.hpp>
#include <Feed/Trade.hpp>
#include <Intern/Limit.hpp>
#include <server/Defines.hpp>
#include <pattern/Observer.hpp>

#include <functional>


namespace core
{

    template <class Allocator = core::LinearAllocator>
    class TradeHandler : public Subject
    {

        typedef core::CustomArray<intern::Limit, 1, 1000000> DepthList_;

        typedef std::unordered_map<uint64_t,feed::Trade*> TradeMarketList_;

    protected:
        DepthList_ mBuyLimitSide;

        DepthList_ mSellLimitSide;

        TradeMarketList_ mTrades;

        Allocator *mAllocator{nullptr};

    public:
        /**
             * @brief Construct a new Trade Handler object
             * 
             * @param aInstrument 
             */
        TradeHandler();

        /**
             * @brief Destroy the Trade Handler object
             * 
             */
        virtual ~TradeHandler();

        /**
             * @brief Get the Next Id object
             * 
             * @return uint64_t 
             */
        uint64_t getNextId();

        /**
             * @brief 
             * 
             * @param aOrder 
             */
        virtual void Match(trading::Order *aOrder);

        /**
             * @brief 
             * 
             * @param aOrderOne 
             * @param aOrderTwo 
             * @param aPrice 
             * @return true 
             * @return false 
             */
        bool Execute(trading::Order *aOrderOne, trading::Order *aOrderTwo, uint64_t aPrice);
        /**
             * @brief 
             * 
             * @param aSessionId 
             * @param aStock 
             * @param aSide 
             * @param aPrice 
             * @param aQty 
             */
        void GenerateTrade(uint64_t aParentSessionId, uint64_t aSessionId, uint64_t aStock, char aSide, uint64_t aPrice, uint64_t aQty,uint64_t aOrderId);

        void onCancelOrder(trading::Order *);
        /**
         * @brief 
         * 
         */
        void onBuyOrder(trading::Order *);

        /**
             * @brief 
             * 
             */
        void onSellOrder(trading::Order *);

        /**
             * @brief 
             * 
             * @return true 
             * @return false 
             */
        bool onRemoveBuyOrder(trading::Order *);

        /**
             * @brief 
             * 
             * @return true 
             * @return false 
             */
        bool onRemoveSellOrder(trading::Order *);

        /**
             * @brief 
             * 
             */
        void PublishOrder(trading::Order *);
    };

}

template <class Allocator>
core::TradeHandler<Allocator>::TradeHandler() : mBuyLimitSide(nullptr), mSellLimitSide(nullptr)
{
    mAllocator = new Allocator();
    mAllocator->Init(1024 * 1024, 64);
};

template <class Allocator>
core::TradeHandler<Allocator>::~TradeHandler()
{
    delete mAllocator;
};

template <class Allocator>
bool core::TradeHandler<Allocator>::Execute(trading::Order *aOrderOne, trading::Order *aOrderTwo, uint64_t aPrice)
{

    if (aOrderOne->REMAININGSHARES > aOrderTwo->REMAININGSHARES)
    {
        aOrderOne->REMAININGSHARES -= aOrderTwo->REMAININGSHARES;
        GenerateTrade(aOrderOne->PARENTSESSIONID, aOrderOne->SESSIONID, aOrderOne->STOCK, aOrderOne->BUYSELL, aPrice, aOrderTwo->REMAININGSHARES,aOrderOne->ORDERID);
        GenerateTrade(aOrderOne->PARENTSESSIONID, aOrderTwo->SESSIONID, aOrderOne->STOCK, aOrderTwo->BUYSELL, aPrice, aOrderTwo->REMAININGSHARES,aOrderTwo->ORDERID);
        aOrderTwo->REMAININGSHARES = 0;
    }
    else if (aOrderOne->REMAININGSHARES < aOrderTwo->REMAININGSHARES)
    {
        aOrderTwo->REMAININGSHARES -= aOrderOne->REMAININGSHARES;
        GenerateTrade(aOrderOne->PARENTSESSIONID, aOrderOne->SESSIONID, aOrderOne->STOCK, aOrderOne->BUYSELL, aPrice, aOrderOne->REMAININGSHARES,aOrderOne->ORDERID);
        GenerateTrade(aOrderOne->PARENTSESSIONID, aOrderTwo->SESSIONID, aOrderOne->STOCK, aOrderTwo->BUYSELL, aPrice, aOrderOne->REMAININGSHARES,aOrderTwo->ORDERID);
        aOrderOne->REMAININGSHARES = 0;
    }
    else
    {
        GenerateTrade(aOrderOne->PARENTSESSIONID, aOrderOne->SESSIONID, aOrderOne->STOCK, aOrderOne->BUYSELL, aPrice, aOrderTwo->REMAININGSHARES,aOrderOne->ORDERID);
        GenerateTrade(aOrderOne->PARENTSESSIONID, aOrderTwo->SESSIONID, aOrderOne->STOCK, aOrderTwo->BUYSELL, aPrice, aOrderTwo->REMAININGSHARES,aOrderTwo->ORDERID);
        aOrderTwo->REMAININGSHARES = 0;
        aOrderOne->REMAININGSHARES = 0;
    }

    if (aOrderOne->REMAININGSHARES)
        return false;
    else
        return true;
}

template <class Allocator>
void core::TradeHandler<Allocator>::GenerateTrade(uint64_t aParentSessionId, uint64_t aSessionId, uint64_t aStock, char aSide, uint64_t aPrice, uint64_t aQty,uint64_t aOrderId)
{

    StructData *lStructExec = (StructData *)mAllocator->Allocate(sizeof(StructData));
    trading::Exec *lTradeTrading = (trading::Exec *)mAllocator->Allocate(sizeof(trading::Exec));
    lTradeTrading->TYPE = 'E';
    lTradeTrading->STOCK = aStock;
    lTradeTrading->ORDERID = aOrderId;
    lTradeTrading->MATCHNUMBER = getNextId();
    lTradeTrading->EXECUTEDSHARES = aQty;
    lTradeTrading->EXECUTEDPRICE = aPrice;
    lTradeTrading->SESSIONID = aSessionId;
    lTradeTrading->PARENTSESSIONID = aParentSessionId;
    lStructExec->mClientId = aParentSessionId;
    lStructExec->mData = (void *)lTradeTrading;
    lStructExec->mLength = sizeof(trading::Exec);
    notifyObservers(core::Event::FromMatchingEngine, lStructExec);
    notifyObservers(core::Event::ToMarketData, lStructExec);
}

template <class Allocator>
uint64_t core::TradeHandler<Allocator>::getNextId()
{
    static uint64_t gId = 0;
    return ++gId;
}

template <class Allocator>
void core::TradeHandler<Allocator>::PublishOrder(trading::Order *aOrder)
{
    StructData *lStructOrder = (StructData *)mAllocator->Allocate(sizeof(StructData));
    feed::Order *lAddOrder = (feed::Order *)mAllocator->Allocate(sizeof(feed::Order));

    lAddOrder->TYPE = 'X';
    //lAddOrder->ORDERID=aOrder->ORDERID;
    lAddOrder->BUYSELL = aOrder->BUYSELL;
    lAddOrder->SHARES = aOrder->REMAININGSHARES;
    lAddOrder->STOCK = aOrder->STOCK;
    lAddOrder->PRICE = aOrder->PRICE;

    lStructOrder->mClientId = aOrder->PARENTSESSIONID;
    lStructOrder->mData = (void *)lAddOrder;
    lStructOrder->mLength = sizeof(feed::Order);
    notifyObservers(core::Event::ToMarketData, lStructOrder);
}

template <class Allocator>
void core::TradeHandler<Allocator>::onBuyOrder(trading::Order *aOrder)
{
    intern::Limit *lNewLimit = mBuyLimitSide[aOrder->PRICE];
    if (lNewLimit != nullptr)
    {
        lNewLimit->QUANTITY += aOrder->REMAININGSHARES;
        lNewLimit->push(aOrder, mAllocator);
    }
    else
    {
        lNewLimit = (intern::Limit *)mAllocator->Allocate(sizeof(intern::Limit));
        lNewLimit->SIDE = aOrder->BUYSELL;
        lNewLimit->PRICE = aOrder->PRICE;
        lNewLimit->QUANTITY = aOrder->REMAININGSHARES;
        lNewLimit->push(aOrder, mAllocator, true);
    }
    mBuyLimitSide.add(lNewLimit->PRICE, lNewLimit);
    PublishOrder(aOrder);
};

template <class Allocator>
void core::TradeHandler<Allocator>::onSellOrder(trading::Order *aOrder)
{

    intern::Limit *lLimit = mSellLimitSide[aOrder->PRICE];
    if (lLimit != nullptr)
    {
        lLimit->QUANTITY += aOrder->REMAININGSHARES;
        lLimit->push(aOrder, mAllocator);
    }
    else
    {
        lLimit = (intern::Limit *)mAllocator->Allocate(sizeof(intern::Limit));
        lLimit->SIDE = aOrder->BUYSELL;
        lLimit->PRICE = aOrder->PRICE;
        lLimit->QUANTITY = aOrder->REMAININGSHARES;
        lLimit->push(aOrder, mAllocator, true);
    }
    mSellLimitSide.add(lLimit->PRICE, lLimit);
    PublishOrder(aOrder);
};

template <class Allocator>
bool core::TradeHandler<Allocator>::onRemoveBuyOrder(trading::Order *aOrder)
{
    intern::Limit *lLimit = mBuyLimitSide[aOrder->PRICE];
    if (lLimit != nullptr)
    {
        intern::OrderListElement *lTemp = lLimit->mOrderList->Head;
        while (lTemp != nullptr)
        {
            if (aOrder->ORDERID == lTemp->elem->ORDERID)
            {
                lTemp->elem->REMAININGSHARES = 0;
                return true;
            }
            lTemp=lTemp->Next;
        }
    }
    return false;
};

template <class Allocator>
bool core::TradeHandler<Allocator>::onRemoveSellOrder(trading::Order *aOrder)
{
    intern::Limit *lLimit = mSellLimitSide[aOrder->PRICE];
    if (lLimit != nullptr)
    {
        intern::OrderListElement *lTemp = lLimit->mOrderList->Head;
        while (lTemp != nullptr)
        {
            if (aOrder->ORDERID == lTemp->elem->ORDERID)
            {
                lTemp->elem->REMAININGSHARES = 0;
                return true;
            }
            lTemp=lTemp->Next;
        }
    }
    return false;
};

template <class Allocator>
void core::TradeHandler<Allocator>::Match(trading::Order *aOrder)
{
    if (aOrder->BUYSELL == 'B')
    {
        if (mSellLimitSide.empty())
            return;
        uint64_t llimit = mSellLimitSide.firstIndex();
        while ((llimit <= aOrder->PRICE) && (aOrder->REMAININGSHARES != 0))
        {
            // if (mSellLimitSide.empty()) return;
            //if (mSellLimitSide[llimit] == nullptr) {llimit++; continue;}
            intern::OrderListElement *lTemp = mSellLimitSide[llimit]->mOrderList->Head;
            while (lTemp != nullptr && (aOrder->REMAININGSHARES != 0))
            {
                if (Execute(lTemp->elem, aOrder, mSellLimitSide[llimit]->PRICE))
                {
                    lTemp = lTemp->Next;
                }
                else
                {
                    break;
                }
            }
            mSellLimitSide[llimit]->mOrderList->Head = lTemp;
            if (aOrder->REMAININGSHARES != 0)
            {
                //llimit++;//
                llimit = mSellLimitSide.next();
            }
            else
                break;
        }
    }
    else if (aOrder->BUYSELL == 'S')
    {
        if (mBuyLimitSide.empty())
            return;
        uint64_t llimit = mBuyLimitSide.lastIndex();
        while ((llimit >= aOrder->PRICE) && (aOrder->REMAININGSHARES != 0))
        {
            //if (mBuyLimitSide.empty()) return;
            //if (mBuyLimitSide[llimit] == nullptr) {llimit--; continue;}
            intern::OrderListElement *lTemp = mBuyLimitSide[llimit]->mOrderList->Head;
            while (lTemp != nullptr && (aOrder->REMAININGSHARES != 0))
            {
                if (Execute(lTemp->elem, aOrder, mBuyLimitSide[llimit]->PRICE))
                {
                    lTemp = lTemp->Next;
                }
                else
                {
                    break;
                }
            }
            mBuyLimitSide[llimit]->mOrderList->Head = lTemp;
            if (aOrder->REMAININGSHARES != 0)
            {
                //llimit--;//
                llimit = mBuyLimitSide.previous();
            }
            else
                break;
        }
    }
}

#endif
