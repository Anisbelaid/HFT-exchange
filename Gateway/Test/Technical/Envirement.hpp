#ifndef _ENVIREMENT_TEST_HPP_
#define _ENVIREMENT_TEST_HPP_

#include "Application/Envirement.hpp"
#include <gtest/gtest.h> 


class EnvirementTest : public testing::Test
{ 
    protected :

        void SetUp() { 
             
            
        }

        void TearDown() { 
        }
    
        static void SetUpTestCase() { 
        }

        static void TearDownTestCase() {  
        }
};

TEST_F(EnvirementTest, ConfigurationFolder) {
    gateway::Envirement lEnv ;
    ASSERT_TRUE(lEnv.loadEnvirement()); 
    ASSERT_STREQ(lEnv.getConfigFolder().c_str(),"/home/anis/Desktop/workSpace/repo/core/Gateway/Test");
} 


#endif