
#ifndef _ORDER_EXECUTION_TEST_HPP_
#define _ORDER_EXECUTION_TEST_HPP_

#include <gtest/gtest.h> 
#include <Application/MessageHandler.hpp>
#include <vector>

class OrderExecution : public testing::Test
{ 
    protected :
        
        static std::vector<int> mId;
        static std::vector<void *> mMessageMarketData;
        static std::vector<uint64_t> mLengthMarketData;
        static std::vector<void *> mMessageTrading;
        static std::vector<uint64_t> mLengthTrading;
        EnterOrder  mOrder;
        matching::MessageHandler mMessageHandler;

    public : 
        OrderExecution( ) { 
        } 
        
        // You can define per-test set-up logic as usual.
        void SetUp() { 
                mOrder.TYPE='O';
                mOrder.BUYSELL='B';
                mOrder.STOCK=1111;
                mOrder.PRICE=10;
                mOrder.SHARES=20;
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
        static void pushTradingdata(int aId, void * aMessage,size_t aLength){
            mId.push_back(aId);
            mMessageTrading.push_back(aMessage) ;
            mLengthTrading.push_back(aLength);
        }
        
        static void SetUpTestCase( ) { 
            auto lMC= [=](void*aMessage,size_t aLength){OrderExecution::pushMarketData(aMessage,aLength);};
            auto lTC= [=](int aId,void*aMessage,size_t aLength){OrderExecution::pushTradingdata(aId,aMessage,aLength);};
            matching::OrderBook<core::LinearAllocator>::getInstance()->setCallback(lMC,lTC);
            matching::OrderBook<core::LinearAllocator>::getInstance()->flush();

        }

        static void TearDownTestCase( ) {  
            matching::OrderBook<core::LinearAllocator>::getInstance()->flush();
        }

        ~OrderExecution( )  { 
            mId.clear();
            mMessageMarketData.clear();
            mLengthMarketData.clear();
            mMessageTrading.clear();
            mLengthTrading.clear();
        }
 
};

std::vector<int> OrderExecution::mId{0};
std::vector<void *> OrderExecution::mMessageMarketData;
std::vector<uint64_t> OrderExecution::mLengthMarketData;
std::vector<void *> OrderExecution::mMessageTrading;
std::vector<uint64_t> OrderExecution::mLengthTrading;


TEST_F(OrderExecution, TotalExecution) { 
     
    mOrder.BUYSELL='S';
    mMessageHandler.onMessage((void*)&mOrder,sizeof (EnterOrder));
    AddOrder * lOrder = (AddOrder*) mMessageMarketData[0];
    ASSERT_EQ (mLengthMarketData[0],sizeof (AddOrder));
    ASSERT_EQ (lOrder->STOCK,1111);
    ASSERT_EQ (lOrder->BUYSELL,'S');
    ASSERT_EQ (lOrder->PRICE,10);
    ASSERT_EQ (lOrder->SHARES,20);
    
    
    mOrder.BUYSELL='B';
    mMessageHandler.onMessage((void*)&mOrder,sizeof (EnterOrder));
    
    EXECUTED *lExec  = (EXECUTED*) mMessageTrading[0];
    ASSERT_EQ (mLengthTrading[0],sizeof (EXECUTED));
    ASSERT_EQ (lExec->EXECUTEDSHARES,20);
    ASSERT_EQ (lExec->EXECUTEDPRICE,10);
    ASSERT_EQ (lExec->TYPE,'E');
    
    lExec  = (EXECUTED*) mMessageTrading[1];
    ASSERT_EQ (mLengthTrading[1],sizeof (EXECUTED));
    ASSERT_EQ (lExec->EXECUTEDSHARES,20);
    ASSERT_EQ (lExec->EXECUTEDPRICE,10);
    ASSERT_EQ (lExec->TYPE,'E');
    
    Trade * lTrade  = (Trade*) mMessageMarketData[1];
    ASSERT_EQ (mLengthMarketData[1],sizeof (Trade));
    ASSERT_EQ (lTrade->STOCK,1111);
    ASSERT_EQ (lTrade->SHARES,20);
    ASSERT_EQ (lTrade->PRICE,10);
    ASSERT_EQ (lTrade->TYPE,'P');
    
    lTrade  = (Trade*) mMessageMarketData[2];
    ASSERT_EQ (mLengthMarketData[2],sizeof (Trade));
    ASSERT_EQ (lTrade->STOCK,1111);
    ASSERT_EQ (lTrade->SHARES,20);
    ASSERT_EQ (lTrade->PRICE,10);
    ASSERT_EQ (lTrade->TYPE,'P');
    
} 


TEST_F(OrderExecution, MultipleExecution) { 
    
    EnterOrder  lOrderOne;
    lOrderOne.TYPE='O';
    lOrderOne.BUYSELL='S';
    lOrderOne.STOCK=1111;
    lOrderOne.PRICE=10;
    lOrderOne.SHARES=20;
    mMessageHandler.onMessage((void*)&lOrderOne,sizeof (EnterOrder));
    AddOrder * lOrder = (AddOrder*) mMessageMarketData[0];
    ASSERT_EQ (mLengthMarketData[0],sizeof (AddOrder));
    ASSERT_EQ (lOrder->STOCK,1111);
    ASSERT_EQ (lOrder->BUYSELL,'S');
    ASSERT_EQ (lOrder->PRICE,10);
    ASSERT_EQ (lOrder->SHARES,20);
    
    EnterOrder  lOrdertwo;
    lOrdertwo.TYPE='O';
    lOrdertwo.BUYSELL='S';
    lOrdertwo.STOCK=1111;
    lOrdertwo.PRICE=10;
    lOrdertwo.SHARES=20;
    
    mMessageHandler.onMessage((void*)&lOrdertwo,sizeof (EnterOrder));
    lOrder = (AddOrder*) mMessageMarketData[1];
    ASSERT_EQ (mLengthMarketData[1],sizeof (AddOrder));
    ASSERT_EQ (lOrder->STOCK,1111);
    ASSERT_EQ (lOrder->BUYSELL,'S');
    ASSERT_EQ (lOrder->PRICE,10);
    ASSERT_EQ (lOrder->SHARES,20);
    
    
    EnterOrder  lOrderthree;
    lOrderthree.TYPE='O';
    lOrderthree.BUYSELL='B';
    lOrderthree.STOCK=1111;
    lOrderthree.PRICE=10;
    lOrderthree.SHARES=100;
    mMessageHandler.onMessage((void*)&lOrderthree,sizeof (EnterOrder));
    
    EXECUTED *lExec  = (EXECUTED*) mMessageTrading[0];
    ASSERT_EQ (mLengthTrading[0],sizeof (EXECUTED));
    ASSERT_EQ (lExec->EXECUTEDSHARES,20);
    ASSERT_EQ (lExec->EXECUTEDPRICE,10);
    ASSERT_EQ (lExec->TYPE,'E');
    
    lExec  = (EXECUTED*) mMessageTrading[1];
    ASSERT_EQ (mLengthTrading[1],sizeof (EXECUTED));
    ASSERT_EQ (lExec->EXECUTEDSHARES,20);
    ASSERT_EQ (lExec->EXECUTEDPRICE,10);
    ASSERT_EQ (lExec->TYPE,'E');
    
    lExec  = (EXECUTED*) mMessageTrading[2];
    ASSERT_EQ (mLengthTrading[2],sizeof (EXECUTED));
    ASSERT_EQ (lExec->EXECUTEDSHARES,20);
    ASSERT_EQ (lExec->EXECUTEDPRICE,10);
    ASSERT_EQ (lExec->TYPE,'E');
    
    lExec  = (EXECUTED*) mMessageTrading[3];
    ASSERT_EQ (mLengthTrading[3],sizeof (EXECUTED));
    ASSERT_EQ (lExec->EXECUTEDSHARES,20);
    ASSERT_EQ (lExec->EXECUTEDPRICE,10);
    ASSERT_EQ (lExec->TYPE,'E');
    
    Trade * lTrade  = (Trade*) mMessageMarketData[2];
    ASSERT_EQ (mLengthMarketData[2],sizeof (Trade));
    ASSERT_EQ (lTrade->STOCK,1111);
    ASSERT_EQ (lTrade->SHARES,20);
    ASSERT_EQ (lTrade->PRICE,10);
    ASSERT_EQ (lTrade->TYPE,'P');
    
    lTrade  = (Trade*) mMessageMarketData[3];
    ASSERT_EQ (mLengthMarketData[3],sizeof (Trade));
    ASSERT_EQ (lTrade->STOCK,1111);
    ASSERT_EQ (lTrade->SHARES,20);
    ASSERT_EQ (lTrade->PRICE,10);
    ASSERT_EQ (lTrade->TYPE,'P');
    
     lTrade  = (Trade*) mMessageMarketData[4];
    ASSERT_EQ (mLengthMarketData[4],sizeof (Trade));
    ASSERT_EQ (lTrade->STOCK,1111);
    ASSERT_EQ (lTrade->SHARES,20);
    ASSERT_EQ (lTrade->PRICE,10);
    ASSERT_EQ (lTrade->TYPE,'P');
    
     lTrade  = (Trade*) mMessageMarketData[5];
    ASSERT_EQ (mLengthMarketData[5],sizeof (Trade));
    ASSERT_EQ (lTrade->STOCK,1111);
    ASSERT_EQ (lTrade->SHARES,20);
    ASSERT_EQ (lTrade->PRICE,10);
    ASSERT_EQ (lTrade->TYPE,'P');
    
    
    lOrder = (AddOrder*) mMessageMarketData[6];
    ASSERT_EQ (mLengthMarketData[6],sizeof (AddOrder));
    ASSERT_EQ (lOrder->STOCK,1111);
    ASSERT_EQ (lOrder->BUYSELL,'B');
    ASSERT_EQ (lOrder->PRICE,10);
    ASSERT_EQ (lOrder->SHARES,60);
} 


#endif
