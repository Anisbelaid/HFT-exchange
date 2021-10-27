#include "gtest/gtest.h"
//#include "OrderCreation_Test.hpp"
//#include "OrderExecution_Test.hpp"
//#include "TradingServer_Test.hpp"
//#include "Sequencer_Test.hpp"
#include "Session.hpp"

int main(int argc, const char * argv[])
{
    testing::InitGoogleTest(&argc, (char**)argv);
    return RUN_ALL_TESTS();
}
