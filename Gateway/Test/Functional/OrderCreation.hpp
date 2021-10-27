#ifndef _ORDER_CREATION_HPP_
#define _ORDER_CREATION_HPP_

#include <gtest/gtest.h> 
#include <MarketOrder/MarketOrder.hpp>
#include <Trading/Order.hpp>
#include <vector>

class OrderCreation : public testing::Test
{ 
     protected :
        
         static std::vector<void *> mReplyData;
         static std::vector<uint64_t> mReplyLengthData;
         static std::vector<void *> mMatchingData;
         static std::vector<uint64_t> mMatchingLengthData;
         trading::Order mOrder;
         trading::Order mOrder1;
     public : 
        
         void SetUp() { 
            mOrder.STOCK = 500;
            mOrder.ORDERID = 100;
            mOrder.SHARES =100;
            mOrder.PRICE = 100;
            mOrder.TIMEINFORCE = TimeInForce_::TWAP ;
            mOrder.SESSIONID = 16;
            mOrder.BUYSELL = 'B';
            strcpy(mOrder.CLIENTID,"CLIENTO");
            mOrder1.STOCK = 500;
            mOrder1.ORDERID = 100;
            mOrder1.SHARES =100;
            mOrder1.PRICE = 100;
            mOrder1.TIMEINFORCE = TimeInForce_::TWAP ;
            mOrder1.SESSIONID = 16;
            mOrder1.BUYSELL = 'S';
            strcpy(mOrder1.CLIENTID,"CLIENT1");

         }

         void TearDown() { 
             mReplyData.clear();
             mReplyLengthData.clear();
             mMatchingData.clear();
             mMatchingLengthData.clear();
            
         }
  
         static void pushMarketData(void * aMessage,size_t aLength){
             mReplyData.push_back(aMessage) ;
             mReplyLengthData.push_back(aLength);
         }
        
         static void pushTradingdata(int aId, void * aMessage,size_t aLength){
             mMatchingData.push_back(aMessage) ;
             mMatchingLengthData.push_back(aLength);
         }

         static void SetUpTestCase( ) { 
             auto lMC= [=](void*aMessage,size_t aLength){OrderCreation::pushMarketData(aMessage,aLength);};
             auto lTC= [=](int aId,void*aMessage,size_t aLength){OrderCreation::pushTradingdata(aId,aMessage,aLength);};
             gateway::MarketOrder<10000>::getInstance()->Initialize (lMC,lTC);
             gateway::InstrumentManager<>::getInstance ()->initialize("/home/anis/Desktop/workSpace/repo/core/Gateway/Test/ressource/Instrument.csv");
         }

         static void TearDownTestCase( ) {   
             mReplyData.clear();
             mReplyLengthData.clear();
             mMatchingData.clear();
             mMatchingLengthData.clear();
         }
 
 };

 std::vector<void *> OrderCreation::mReplyData;
 std::vector<uint64_t> OrderCreation::mReplyLengthData;
 std::vector<void *> OrderCreation::mMatchingData;
 std::vector<uint64_t> OrderCreation::mMatchingLengthData;

 TEST_F(OrderCreation, BuyTest) { 
    
    gateway::MarketOrder<10000>::getInstance()->onNewOrder (&mOrder);
    gateway::MarketOrder<10000>::getInstance()->onNewOrder (&mOrder1);
//ASSERT_EQ (mReplyData.size(),1);
    std::this_thread::sleep_for (std::chrono::seconds(5));
     
 } 


 #endif
