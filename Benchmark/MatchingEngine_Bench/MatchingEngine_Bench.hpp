#ifndef _BENCHMARK_Test_HPP_
#define _BENCHMARK_Test_HPP_


#include <benchmark/benchmark.h> 
#include <Application/MessageHandler.hpp>
#include <vector>
#include <numeric>  
#include <Sequencer.hpp>
#include <Application/Defines.hpp>
#include <vDSO.hpp>
#include <random>

class OrderBookBench :  public ::benchmark::Fixture 
{ 
    protected:
        using Sequ = sequencer::Sequencer<gMaxCnnection>;
        using OrderBook = matching::OrderBook<core::LinearAllocator>;
    protected:
       StructData  * mData;
       
       EnterOrder  * mOrder;
       
       static std::condition_variable mCond;
       
       static std::mutex mMutex;
       
       static std::once_flag mFlag;
       
       static matching::MessageHandler mMessageHandler;
       
       static std::atomic_uint_fast64_t mNbOrders ;
       
       static std::atomic_uint_fast64_t mNbOrdersSent ;
       
    public :  
        OrderBookBench( ) {
            std::call_once(mFlag,
                        [&]() {
                           // mMessageHandler.Start();
                            auto lMC= [=](void*aMessage,size_t aLength){OrderBookBench::pushMarketData(aMessage,aLength);};
                            auto lTC= [=](int aId,void*aMessage,size_t aLength){OrderBookBench::pushTradingdata(aId,aMessage,aLength);};
                            OrderBook::getInstance(lMC,lTC);
                            Sequ::getInstance()->subscribe (2);
                            Sequ::getInstance()->subscribe (3);
                            Sequ::getInstance()->subscribe (4);
                            Sequ::getInstance()->subscribe (5);
                        });
            
             
        } 
        
        void initOrder (){
            mNbOrders = 0;
            mNbOrdersSent = 0;
            mOrder= new EnterOrder ();
            mOrder->TYPE='O';
            mOrder->BUYSELL='B';
            mOrder->STOCK=1111;
            mOrder->PRICE=10;
            mOrder->SHARES=20;
            mData = new StructData();
            mData->mData =mOrder;
            mData->mLength = sizeof (EnterOrder);
            mData->mClientId = 1;
            
        }

        void SetUp(const ::benchmark::State& state) { 
            initOrder ();
        }
        
        uint64_t RandomPrice (int aSpread){
            return rand() % aSpread + 1;
            }
        
         uint64_t RandomSide (){
            return rand() % 2 + 1;
        }
        
        static void pushMarketData(void * aMessage,size_t aLength){
            mNbOrdersSent--;
        }
        
        static void pushTradingdata(int aId, void * aMessage,size_t aLength){
            mNbOrders++;
        }
        
        static void SetUpTestCase( ) { 
        }

        void TearDown(const ::benchmark::State& state) {  
            delete mOrder;
            delete mData;
            OrderBook::getInstance()->flush();
        }

        ~OrderBookBench( )  { 
           
        }
 
};
std::atomic_uint_fast64_t OrderBookBench::mNbOrders ;
std::atomic_uint_fast64_t OrderBookBench::mNbOrdersSent ;
std::condition_variable OrderBookBench::mCond;
std::mutex OrderBookBench::mMutex;
std::once_flag OrderBookBench::mFlag;

matching::MessageHandler OrderBookBench::mMessageHandler;

BENCHMARK_DEFINE_F(OrderBookBench, OneConnectionSameLevelSameSide)(benchmark::State& state) {
    
    struct timespec lTime;
    StructData * lData=  0;
     while (state.KeepRunning()) {
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        mNbOrdersSent++;
        
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);
        
        core::gClockTime(CLOCK_REALTIME, &lTime);
        
        uint64_t begin =lTime.tv_nsec;
        lData = sequencer::Sequencer<gMaxCnnection>::getInstance()->get ();
        
        if (!lData) continue;  
        
        OrderBook::getInstance()->insertOrder((EnterOrder*)lData->mData);
        
        core::gClockTime(CLOCK_REALTIME, &lTime);
        
        uint64_t end =lTime.tv_nsec;
        state.SetIterationTime(end-begin);
        
    }

  
};

BENCHMARK_REGISTER_F(OrderBookBench, OneConnectionSameLevelSameSide);

BENCHMARK_DEFINE_F(OrderBookBench, OneConnectionincrementalLevelSameSide)(benchmark::State& state) {
    
    struct timespec lTime;
    StructData * lData=  0;
     while (state.KeepRunning()) {
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        ((EnterOrder*)mData->mData)->PRICE++;
        mNbOrdersSent++;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);
        
        uint64_t begin =core::gClockTime(CLOCK_REALTIME, &lTime);;
        lData = sequencer::Sequencer<gMaxCnnection>::getInstance()->get ();
        
        if (!lData) continue;  
        
        OrderBook::getInstance()->insertOrder((EnterOrder*)lData->mData);
        
        uint64_t end =core::gClockTime(CLOCK_REALTIME, &lTime);;
        
        state.SetIterationTime(end-begin);
        
    }

  
};

BENCHMARK_REGISTER_F(OrderBookBench, OneConnectionincrementalLevelSameSide);



BENCHMARK_DEFINE_F(OrderBookBench, OneConnectionSameLeveldifferentSide)(benchmark::State& state) {
    
    struct timespec lTime;
    StructData * lData=  0;
     while (state.KeepRunning()) {
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        ((EnterOrder*)mData->mData)->BUYSELL = (((EnterOrder*)mData->mData)->BUYSELL == 'B'?'S':'B');
        mNbOrdersSent++;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);
        
        uint64_t begin =core::gClockTime(CLOCK_REALTIME, &lTime);;
        lData = sequencer::Sequencer<gMaxCnnection>::getInstance()->get ();
        
        if (!lData) continue;  
        
        OrderBook::getInstance()->insertOrder((EnterOrder*)lData->mData);
        uint64_t end =core::gClockTime(CLOCK_REALTIME, &lTime);;
        state.SetIterationTime(end-begin);
        
    }  
};

BENCHMARK_REGISTER_F(OrderBookBench, OneConnectionSameLeveldifferentSide);

BENCHMARK_DEFINE_F(OrderBookBench, OneConnectionRandomSideRadomPrice)(benchmark::State& state) {
    
    struct timespec lTime;
    StructData * lData=  0;
     while (state.KeepRunning()) {
       // core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        ((EnterOrder*)mData->mData)->BUYSELL = (RandomSide() == 1 ?'S':'B');
        
        ((EnterOrder*)mData->mData)->PRICE = RandomPrice(state.range(0)); 
       // mNbOrdersSent++;
        
        //sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);

        //core::gClockTime(CLOCK_REALTIME, &lTime);
        //((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        //((EnterOrder*)mData->mData)->BUYSELL = (RandomSide() == 1 ?'S':'B');
        //((EnterOrder*)mData->mData)->PRICE = RandomPrice(state.range(0)); 
        //mNbOrdersSent++;
        
       // using namespace std::chrono; 
        //sequencer::Sequencer<gMaxCnnection>::getInstance()->push (3,mData);
        //auto start = std::chrono::high_resolution_clock::now();


       // core::gClockTime(CLOCK_REALTIME, &lTime);
        
        //uint64_t begin =lTime.tv_nsec;
        
       // lData = sequencer::Sequencer<gMaxCnnection>::getInstance()->get ();
        
        //if (!lData) continue;  
        
        OrderBook::getInstance()->insertOrder((EnterOrder*)mData->mData);
        
        //lData = sequencer::Sequencer<gMaxCnnection>::getInstance()->get ();
        
        //if (!lData) continue;  
        
        //OrderBook::getInstance()->insertOrder((EnterOrder*)lData->mData);
        //auto finish = std::chrono::high_resolution_clock::now();
        //core::gClockTime(CLOCK_REALTIME, &lTime);
       
      //  uint64_t end =lTime.tv_nsec;
        //state.SetIterationTime(end-begin);
        
    }  
};

BENCHMARK_REGISTER_F(OrderBookBench, OneConnectionRandomSideRadomPrice)->Arg(10)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

#endif
