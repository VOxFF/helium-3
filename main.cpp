
#include "common/State.h"
#include "testing/SimulationTest.h"

#include <iostream>

using namespace Helium3;
using namespace Helium3::Testing;


INSTANTIATE_TEST_SUITE_P(
    SimpleScenarios,        // Test suite group (prefix for test names)
    SimulationTest,         // Test class
    ::testing::Values(      // Test parameters
        SimulationTestParams{2, 1, std::chrono::hours(10), {}},
        SimulationTestParams{5, 2, std::chrono::hours(12), {}})
);

TEST_P(SimulationTest, BasicSimulationRuns) {
    ASSERT_NO_THROW(run());
}

int main(int argc, char** argv) 
{
    std::cout << "Hello from Helium-3 mining!" << std::endl << std::endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
