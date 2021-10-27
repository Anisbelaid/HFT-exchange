#ifndef _SEQUENCER_TEST_HPP_
#define _SEQUENCER_TEST_HPP_

#include <gtest/gtest.h> 
#include <vector>
#include <Sequencer.hpp>
#include <algorithm>
#include <chrono>
#include <thread>
#include "Defines.hpp"

using namespace std::chrono;

class SequencerTest : public testing::Test
{ 
    protected :
        static std::vector <StructData*>  mOrders;
        static std::vector <StructData*>  mResults;
    public : 
        SequencerTest( ) { 
        } 
        
        // You can define per-test set-up logic as usual.
        void SetUp() { 
            std::random_shuffle ( mOrders.begin(), mOrders.end() );
        }

        void TearDown() { 
        }
        
        static void SetUpTestCase( ) { 
            mOrders.reserve (5);
            mOrders.push_back(new StructData ());
            mOrders.push_back(new StructData ());
            mOrders.push_back(new StructData ());
            mOrders.push_back(new StructData ());
            mOrders.push_back(new StructData ());
            mOrders[0]->mData = new EnterOrder();
             nanoseconds lns = duration_cast< nanoseconds >(system_clock::now().time_since_epoch());
            ((EnterOrder*)mOrders[0]->mData)->TIMESTAMP = lns.count();
            ((EnterOrder*)mOrders[0]->mData)->TYPE='O';
            mOrders[1]->mData = new EnterOrder();
            lns = duration_cast< nanoseconds >(system_clock::now().time_since_epoch());
            ((EnterOrder*)mOrders[1]->mData)->TIMESTAMP = lns.count();
            ((EnterOrder*)mOrders[1]->mData)->TYPE='O';
            mOrders[2]->mData = new EnterOrder();
            lns = duration_cast< nanoseconds >(system_clock::now().time_since_epoch());
            ((EnterOrder*)mOrders[2]->mData)->TIMESTAMP = lns.count();
            ((EnterOrder*)mOrders[2]->mData)->TYPE='O';
            mOrders[3]->mData = new EnterOrder();
            lns = duration_cast< nanoseconds >(system_clock::now().time_since_epoch());
            ((EnterOrder*)mOrders[3]->mData)->TIMESTAMP = lns.count();
            ((EnterOrder*)mOrders[3]->mData)->TYPE='O';
            mOrders[4]->mData = new EnterOrder();
            lns = duration_cast< nanoseconds >(system_clock::now().time_since_epoch());
            ((EnterOrder*)mOrders[4]->mData)->TIMESTAMP = lns.count();
            ((EnterOrder*)mOrders[4]->mData)->TYPE='O';
            
            ((EnterOrder*)mOrders[0]->mData)->ORDERID=1;
            ((EnterOrder*)mOrders[1]->mData)->ORDERID=2;
            ((EnterOrder*)mOrders[2]->mData)->ORDERID=3;
            ((EnterOrder*)mOrders[3]->mData)->ORDERID=4;
            ((EnterOrder*)mOrders[4]->mData)->ORDERID=5;
            
            
            sequencer::Sequencer<5>::getInstance() ;
            std::this_thread::sleep_for(1ns);
            sequencer::Sequencer<5>::getInstance()->subscribe(1);
            sequencer::Sequencer<5>::getInstance()->subscribe(2);
            sequencer::Sequencer<5>::getInstance()->subscribe(3);
            sequencer::Sequencer<5>::getInstance()->subscribe(4);
            sequencer::Sequencer<5>::getInstance()->subscribe(5);
        }

        static void TearDownTestCase( ) {  
            sequencer::Sequencer<5>::getInstance()->unSubscribe(1);
            sequencer::Sequencer<5>::getInstance()->unSubscribe(2);
            sequencer::Sequencer<5>::getInstance()->unSubscribe(3);
            sequencer::Sequencer<5>::getInstance()->unSubscribe(4);
            sequencer::Sequencer<5>::getInstance()->unSubscribe(5);
            for (auto lIt : mOrders){
                delete ((EnterOrder*)lIt->mData);
                delete lIt;
            }
            mOrders.clear();
        }

        ~SequencerTest( )  {
            mOrders.clear();
        }
 
};

std::vector<StructData*> SequencerTest::mOrders ;

std::vector<StructData*> SequencerTest::mResults ;

TEST_F(SequencerTest, randomTest) { 
  for (auto lIt : mOrders){
        sequencer::Sequencer<5>::getInstance()->push(((EnterOrder*)lIt->mData)->ORDERID,lIt);
  }

  for (int i=0;i<mOrders.size();i++){
    StructData *lOrder = nullptr;
    while (!lOrder)
        lOrder = sequencer::Sequencer<5>::getInstance()->get();
    	mResults.push_back(lOrder);
  }
  
  
  ASSERT_EQ (5,(((EnterOrder*)mResults[4]->mData)->ORDERID));
  ASSERT_EQ (4,(((EnterOrder*)mResults[3]->mData)->ORDERID));
  ASSERT_EQ (3,(((EnterOrder*)mResults[2]->mData)->ORDERID));
  ASSERT_EQ (2,(((EnterOrder*)mResults[1]->mData)->ORDERID));
  ASSERT_EQ (1,(((EnterOrder*)mResults[0]->mData)->ORDERID));

    
} 




#endif
