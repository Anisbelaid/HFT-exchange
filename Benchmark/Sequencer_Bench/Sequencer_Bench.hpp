#ifndef SEQUENCER_BENCHMARK_Test_HPP_
#define SEQUENCER_BENCHMARK_Test_HPP_


#include <benchmark/benchmark.h> 
#include <EngineMessageHandler.hpp>
#include <vector>
#include <numeric>  
#include <Sequencer.hpp>
#include <Defines.hpp>
#include <vDSO.hpp>

using namespace std::chrono;

class SequencerBench :  public ::benchmark::Fixture 
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
        SequencerBench( ) {
            std::call_once(mFlag,
                        [&]() {
                            mMessageHandler.Start();
                            auto lMC= [=](void*aMessage,size_t aLength){SequencerBench::pushMarketData(aMessage,aLength);};
                            auto lTC= [=](int aId,void*aMessage,size_t aLength){SequencerBench::pushTradingdata(aId,aMessage,aLength);};
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
            mData =new StructData();
            mData->mData =mOrder;
            mData->mLength = sizeof (EnterOrder);
            mData->mClientId = 1;
            
        }

        void SetUp(const ::benchmark::State& state) { 
            initOrder ();
        }
        
        static void pushMarketData(void * aMessage,size_t aLength){
            std::unique_lock<std::mutex> lk(mMutex);
            mNbOrders++;
            mCond.notify_all();
        }
        
        static void pushTradingdata(int aId, void * aMessage,size_t aLength){
            std::unique_lock<std::mutex> lk(mMutex);
             mNbOrders++;
             mCond.notify_all();
           
        }
        
        static void SetUpTestCase( ) { 
        }

        void TearDown(const ::benchmark::State& state) {  
            delete mOrder;
            delete mData;
            OrderBook::getInstance()->flush();
        }

        ~SequencerBench( )  { 
           
        }
 
};
std::atomic_uint_fast64_t SequencerBench::mNbOrders ;
std::atomic_uint_fast64_t SequencerBench::mNbOrdersSent ;
std::condition_variable SequencerBench::mCond;
std::mutex SequencerBench::mMutex;
std::once_flag SequencerBench::mFlag;

matching::MessageHandler SequencerBench::mMessageHandler;

BENCHMARK_DEFINE_F(SequencerBench, OneConnection)(benchmark::State& state) {
    
	struct timespec lTime;
    while (state.KeepRunning()) {
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);
        mNbOrdersSent++;
    }
    
    std::unique_lock<std::mutex> lk(mMutex);
    mCond.wait(lk, [](){return mNbOrders == mNbOrdersSent;});
  
};

BENCHMARK_REGISTER_F(SequencerBench, OneConnection);


BENCHMARK_DEFINE_F(SequencerBench, TwoConnection)(benchmark::State& state) {
    
	struct timespec lTime;
    while (state.KeepRunning()) {
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);
        mNbOrdersSent++;
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (3,mData);
        mNbOrdersSent++;
    }
    
    std::unique_lock<std::mutex> lk(mMutex);
    mCond.wait(lk, [](){return mNbOrders == mNbOrdersSent;});
  
};

BENCHMARK_REGISTER_F(SequencerBench, TwoConnection);


BENCHMARK_DEFINE_F(SequencerBench, ThreeConnection)(benchmark::State& state) {
    
	struct timespec lTime;
    while (state.KeepRunning()) {
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);
        mNbOrdersSent++;
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (3,mData);
        mNbOrdersSent++;
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (4,mData);
        mNbOrdersSent++;
    }
    
    std::unique_lock<std::mutex> lk(mMutex);
    mCond.wait(lk, [](){return mNbOrders == mNbOrdersSent;});
  
};

BENCHMARK_REGISTER_F(SequencerBench, ThreeConnection);

BENCHMARK_DEFINE_F(SequencerBench, FourConnection)(benchmark::State& state) {
    
	struct timespec lTime;
    while (state.KeepRunning()) {
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (2,mData);
        mNbOrdersSent++;
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (3,mData);
        mNbOrdersSent++;
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (4,mData);
        mNbOrdersSent++;
        core::gClockTime(CLOCK_REALTIME, &lTime);
        ((EnterOrder*)mData->mData)->TIMESTAMP = lTime.tv_nsec;
        sequencer::Sequencer<gMaxCnnection>::getInstance()->push (5,mData);
        mNbOrdersSent++;
    }
    
    std::unique_lock<std::mutex> lk(mMutex);
    mCond.wait(lk, [](){return mNbOrders == mNbOrdersSent;});
  
};

BENCHMARK_REGISTER_F(SequencerBench, FourConnection);

#endif
