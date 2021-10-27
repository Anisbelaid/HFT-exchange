
#ifndef _TRADING_SERVER_TEST_HPP_
#define _TRADING_SERVER_TEST_HPP_

#include <gtest/gtest.h> 
#include <vector>
#include <TradingServer.hpp>
#include <TCPClient.hpp>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class TradingServerTest : public testing::Test
{ 
    protected :
        static std::vector<int> mId;
        static std::vector<void *> mMessageMarketData;
        static std::vector<uint64_t> mLengthMarketData;
        static std::vector<void *> mMessageTrading;
        static std::vector<uint64_t> mLengthTrading;
        static core::TCPClient<> * mClient ;
        EnterOrder  mOrder;

    public : 
        TradingServerTest( ) { 
        } 
        
        // You can define per-test set-up logic as usual.
        void SetUp() { 
                
                mOrder.TYPE='O';
                mOrder.BUYSELL='B';
                mOrder.STOCK=1111;
                mOrder.PRICE=10;
                mOrder.SHARES=20;
        }

        void TearDown() { 
            mId.clear();
            mMessageMarketData.clear();
            mLengthMarketData.clear();
            mMessageTrading.clear();
            mLengthTrading.clear();
            
        }
  
        static void pushMarketData(void * aMessage,size_t aLength){
            
        }
        static void pushTradingdata(int aId, void * aMessage,size_t aLength){
        }
        static void SetUpTestCase( ) {
             server::TradingServer::getInstance(5571) ;
             std::this_thread::sleep_for(2ms);
             mClient = new core::TCPClient<>("127.0.0.1",5571);
             mClient->setup();
             mClient->start();
        }

        static void TearDownTestCase( ) {  
            server::TradingServer::getInstance()->stop();
            server::TradingServer::getInstance()->stop();
            mClient->stop();
            delete mClient;
        }

        ~TradingServerTest( )  { 
            mId.clear();
            mMessageMarketData.clear();
            mLengthMarketData.clear();
            mMessageTrading.clear();
            mLengthTrading.clear();
            
        }
 
};

core::TCPClient<> * TradingServerTest::mClient =new core::TCPClient<>("127.0.0.1",5571) ;
std::vector<int> TradingServerTest::mId{0};
std::vector<void *> TradingServerTest::mMessageMarketData;
std::vector<uint64_t> TradingServerTest::mLengthMarketData;
std::vector<void *> TradingServerTest::mMessageTrading;
std::vector<uint64_t> TradingServerTest::mLengthTrading;

TEST_F(TradingServerTest, oneOrderProcess) { 
    
    mClient->push(&mOrder,sizeof (EnterOrder));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_EQ (1,server::TradingServer::getInstance()->Count()) ;
} 




#endif
