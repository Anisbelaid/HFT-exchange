#ifndef _CONFIGURATION_TEST_HPP_
#define _CONFIGURATION_TEST_HPP_

#include "Application/Configuration.hpp"
#include <gtest/gtest.h> 
#include <unistd.h>

class ConfigurationTest : public testing::Test
{ 
    public:
        static std::string gRessPath ;
    protected :

        void SetUp() { 
             
            
        }

        void TearDown() { 
        }
    
        static void SetUpTestCase() { 
            
            if (const char * lResult = std::getenv("GATEWAY_TEST"))
                gRessPath = lResult;
            else
            {   
                gRessPath += __FILE__ ;
                gRessPath =  gRessPath.substr(0, gRessPath.rfind("/"));
                gRessPath += "/.." ;
            }
        }

        static void TearDownTestCase() {  
        }
};

std::string ConfigurationTest::gRessPath ="";

TEST_F(ConfigurationTest, FullyFilled) {
    gateway::Configuration lConf (gRessPath,"ressource/Conf1.ini");
    ASSERT_TRUE(lConf.loadConfigurationFile());
    ASSERT_TRUE(lConf.load());
    ASSERT_EQ(lConf.getGatewayId(),1000);
    ASSERT_STREQ(lConf.getGatewayAddress().c_str(),"127.0.0.1");
    ASSERT_STREQ(lConf.getMarKetDataServerAddress().c_str(),"127.0.0.1");
    ASSERT_STREQ(lConf.getReferentialFilePath().c_str(),"/home/anis/Desktop/instrument.csv");
    ASSERT_STREQ(lConf.getMatchingEngineAddress().c_str(),"127.0.0.1");
    ASSERT_EQ(lConf.getGatewayPort(),5556);
    ASSERT_EQ(lConf.getMarketDataServerPort(),5556);
    ASSERT_EQ(lConf.getMatchingEnginePort(),5556);
    ASSERT_STREQ(lConf.getLogginServerAddress().c_str(),"127.0.0.1");
    ASSERT_EQ(lConf.getLoggigServerPort(),5556);
} 

TEST_F(ConfigurationTest, wrongPath) {
    gateway::Configuration lConf ("Trash","ressource/Conf1.ini");
    ASSERT_FALSE(lConf.loadConfigurationFile());
} 


TEST_F(ConfigurationTest, emptyAddress) {
    gateway::Configuration lConf (gRessPath,"ressource/Conf2.ini");
    ASSERT_TRUE(lConf.loadConfigurationFile());
    ASSERT_TRUE(lConf.load());
    ASSERT_STREQ(lConf.getGatewayAddress().c_str(),"");
} 

#endif