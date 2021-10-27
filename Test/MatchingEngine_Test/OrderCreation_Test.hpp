#ifndef _ORDER_CREATION_TEST_HPP_
#define _ORDER_CREATION_TEST_HPP_

#include <gtest/gtest.h> 
#include <Application/MessageHandler.hpp>
#include <vector>
#include <Basket.hpp>

class OrderCreation : public testing::Test
{ 
    protected :
        
        static std::vector<int> mId;
        static std::vector<void *> mMessageMarketData;
        static std::vector<uint64_t> mLengthMarketData;
        static std::vector<void *> mMessageTrading;
        static std::vector<uint64_t> mLengthTrading;
        matching::MessageHandler mMessageHandler;

    public : 
        OrderCreation( ) { 
        } 
        
        // You can define per-test set-up logic as usual.
        void SetUp() { 
        }

        // You can define per-test tear-down logic as usual.
        void TearDown() { 
            matching::OrderBook<core::LinearAllocator>::getInstance()->flush();
            mId.clear();
            mMessageMarketData.clear();
            mLengthMarketData.clear();
            mMessageTrading.clear();
            mLengthTrading.clear();
            
        }
  
        static void pushMarketData(void * aMessage,size_t aLength){
            mMessageMarketData.push_back(aMessage) ;
            mLengthMarketData.push_back(aLength);
        }
        
        static void DebugBasket(void * aMessage,size_t aLength){
        
        }
        static void pushTradingdata(int aId, void * aMessage,size_t aLength){
            mId.push_back(aId);
            mMessageTrading.push_back(aMessage) ;
            mLengthTrading.push_back(aLength);
        }
        static void SetUpTestCase( ) { 
            auto lMC= [=](void*aMessage,size_t aLength){OrderCreation::pushMarketData(aMessage,aLength);};
            auto lTC= [=](int aId,void*aMessage,size_t aLength){OrderCreation::pushTradingdata(aId,aMessage,aLength);};
            matching::OrderBook<core::LinearAllocator>::getInstance(lMC,lTC);
        }

        static void TearDownTestCase( ) {            
        }

        ~OrderCreation( )  { 
            mId.clear();
            mMessageMarketData.clear();
            mLengthMarketData.clear();
            mMessageTrading.clear();
            mLengthTrading.clear();
            
        }
 
};

std::vector<int> OrderCreation::mId{0};
std::vector<void *> OrderCreation::mMessageMarketData;
std::vector<uint64_t> OrderCreation::mLengthMarketData;
std::vector<void *> OrderCreation::mMessageTrading;
std::vector<uint64_t> OrderCreation::mLengthTrading;

TEST_F(OrderCreation, BuyTest) { 
    Basket<100> mBasket ("/home/anis/Desktop/workSpace/MatchingEngine/Test/MatchingEngine_Test/ressource/OrderCreationBuyTest",[&](void*aData,size_t aSize){OrderCreation::DebugBasket(aData,aSize);});
    EnterOrder * lOrder = mBasket.next();
    mMessageHandler.onMessage((void*)lOrder,sizeof (EnterOrder));
    AddOrder * lAddOrder = (AddOrder*) mMessageMarketData[0];
    ASSERT_EQ (mLengthMarketData[0],sizeof (AddOrder));
    ASSERT_EQ (lAddOrder->STOCK,1111);
    ASSERT_EQ (lAddOrder->BUYSELL,'B');
    ASSERT_EQ (lAddOrder->PRICE,10);
    ASSERT_EQ (lAddOrder->SHARES,20);
} 

TEST_F(OrderCreation, SellTest) { 
    Basket<100> mBasket ("/home/anis/Desktop/workSpace/MatchingEngine/Test/MatchingEngine_Test/ressource/OrderCreationSellTest",[&](void*aData,size_t aSize){OrderCreation::DebugBasket(aData,aSize);});
    EnterOrder * lOrder = mBasket.next();
    mMessageHandler.onMessage((void*)lOrder,sizeof (EnterOrder));
    AddOrder * lAddOrder = (AddOrder*) mMessageMarketData[0];
    ASSERT_EQ (mLengthMarketData[0],sizeof (AddOrder));
    ASSERT_EQ (lAddOrder->STOCK,1111);
    ASSERT_EQ (lAddOrder->BUYSELL,'S');
    ASSERT_EQ (lAddOrder->PRICE,10);
    ASSERT_EQ (lAddOrder->SHARES,20);
} 

TEST_F(OrderCreation, DifferentLevel) { 
    Basket<100> mBasket ("/home/anis/Desktop/workSpace/MatchingEngine/Test/MatchingEngine_Test/ressource/OrderCreationDifferentLevelTest",[&](void*aData,size_t aSize){OrderCreation::DebugBasket(aData,aSize);});
    EnterOrder * lOrder = mBasket.next();
    mMessageHandler.onMessage((void*)lOrder,sizeof (EnterOrder));
    AddOrder * lAddOrder = (AddOrder*) mMessageMarketData[0];
    ASSERT_EQ (mLengthMarketData[0],sizeof (AddOrder));
    ASSERT_EQ (lAddOrder->STOCK,1111);
    ASSERT_EQ (lAddOrder->BUYSELL,'S');
    ASSERT_EQ (lAddOrder->PRICE,10);
    ASSERT_EQ (lAddOrder->SHARES,20);
    
    lOrder = mBasket.next();
    
    mMessageHandler.onMessage((void*)lOrder,sizeof (EnterOrder));
    
    ASSERT_EQ (mLengthMarketData[1],sizeof (AddOrder));
    lOrder = mBasket.next();
    
    mMessageHandler.onMessage((void*)lOrder,sizeof (EnterOrder));
    ASSERT_EQ (mLengthMarketData[2],sizeof (AddOrder));
    mMessageHandler.onMessage((void*)lOrder,sizeof (EnterOrder));
    
    lOrder = mBasket.next();
    ASSERT_EQ (mLengthMarketData[3],sizeof (AddOrder));
    mMessageHandler.onMessage((void*)lOrder,sizeof (EnterOrder));
} 




#endif
