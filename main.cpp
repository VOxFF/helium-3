
#include "common/State.h"
#include "testing/SimulationTest.h"
#include "testing/TestTrucks.h"

#include <iostream>

using namespace Helium3;
using namespace Helium3::Testing;

auto oneHourTruckFactory =
  [](int idx) -> std::shared_ptr<ITruck> {
    return std::make_shared<FixedTimeTruck>(
      MiningTruck::namePrefix() + std::to_string(idx),       
      std::chrono::hours(1)
    );
};


INSTANTIATE_TEST_SUITE_P(
    PredictedScenarios,         // Test suite group (prefix for test names)
    PredictedTest,              // Test class
    ::testing::Values(          // Test parameters
        PredictedParams{{1, 1, std::chrono::hours(10), oneHourTruckFactory}, {}}, //1 truck, 1 station
        PredictedParams{{2, 2, std::chrono::hours(10), oneHourTruckFactory}, {}}  //1 trucks, 1 stations
    )
);

/*
1 - one hour truck, 1 station, total time 10 hrs, anticipated
5 ming -> unloading cycles, simulation finishes when truck travesl back to Mining Site
*/
TEST_P(PredictedTest, BasicSimulationRuns) {
    ASSERT_NO_THROW(run());
}

int main(int argc, char** argv) 
{
    std::cout << "Hello from Helium-3 mining!" << std::endl << std::endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
