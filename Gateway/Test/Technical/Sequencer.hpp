#ifndef _SEQUENCER_TEST_HPP_
#define _SEQUENCER_TEST_HPP_

#include "sequencer/Sequencer.hpp"
#include <gtest/gtest.h> 


class SequencerTest : public testing::Test
{ 
    protected :
        core::Sequencer<>       mSequencer;
        StructData             *mDataOne;
        StructData             *mDataTwo;
    public :
        SequencerTest (){}
        ~SequencerTest(){}
        void SetUp() {
            mDataOne = new StructData();
            mDataOne->mLength = 10;
            mDataOne->mData = (void*) "first";
            mDataOne->mClientId = 5;
            mDataTwo = new StructData();
            mDataTwo->mLength = 10;
            mDataTwo->mData = (void*) "second";
            mDataTwo->mClientId = 6;
        }
        void TearDown() {
            delete mDataOne;
            delete mDataTwo;
        }
};

TEST_F(SequencerTest, Subscription) {
    ASSERT_TRUE(mSequencer.subscribe (10,1));
    ASSERT_TRUE(mSequencer.unSubscribe (10));
} 

TEST_F(SequencerTest, SubcritonFailed) {
    for (int i = 1;i<=10;++i)
        mSequencer.subscribe (i,1);
    ASSERT_FALSE(mSequencer.subscribe (11,1));
    ASSERT_FALSE(mSequencer.unSubscribe (11));
}

TEST_F(SequencerTest, pushConsumeSignleClient) {
    ASSERT_TRUE(mSequencer.subscribe (5,1));
    ASSERT_TRUE(mSequencer.push(mDataOne));
    StructData * lTemp = mSequencer.consume ();
    ASSERT_STREQ("first",(char *)lTemp->mData);
    ASSERT_TRUE(mSequencer.unSubscribe (5));
}


TEST_F(SequencerTest, pushConsumeMultipleClient) {
    ASSERT_TRUE(mSequencer.subscribe (5,1));
    ASSERT_TRUE(mSequencer.subscribe (6,1));
    ASSERT_TRUE(mSequencer.push(mDataOne));
    ASSERT_TRUE(mSequencer.push(mDataTwo));
    StructData * lTemp = mSequencer.consume ();
    ASSERT_TRUE(lTemp);
    ASSERT_STREQ("second",(char *)lTemp->mData);
    lTemp = mSequencer.consume ();
    ASSERT_TRUE(lTemp);
    ASSERT_STREQ("first",(char *)lTemp->mData);
    ASSERT_TRUE(mSequencer.unSubscribe (5));
    ASSERT_TRUE(mSequencer.unSubscribe (6));
}

TEST_F(SequencerTest, pushConsumeMultipleClientDifferentRatio) {
    ASSERT_TRUE(mSequencer.subscribe (5,1));
    ASSERT_TRUE(mSequencer.subscribe (6,2));
    ASSERT_TRUE(mSequencer.push(mDataOne));
    ASSERT_TRUE(mSequencer.push(mDataTwo));
    StructData * lTemp = mSequencer.consume ();
    ASSERT_TRUE(lTemp);
    ASSERT_STREQ("second",(char *)lTemp->mData);
    lTemp = mSequencer.consume ();
    ASSERT_FALSE(lTemp);
    lTemp = mSequencer.consume ();
    ASSERT_TRUE(lTemp);
    ASSERT_STREQ("first",(char *)lTemp->mData);
    ASSERT_TRUE(mSequencer.unSubscribe (5));
    ASSERT_TRUE(mSequencer.unSubscribe (6));
}


#endif