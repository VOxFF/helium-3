
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
    PredictedScenarios,
    PredictedTest,
    ::testing::Values(
        PredictedParams{
            {1, 1, std::chrono::hours(10), oneHourTruckFactory}, // 1 truck, 1 station
            std::unordered_map<std::string, ILog::MachineSummary>{
                {"Truck_0", ILog::MachineSummary{
                    std::map<std::string, ILog::StateSummary>{
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    },
                    {} // unfinished
                }}
            }
        },
        PredictedParams{
            {2, 2, std::chrono::hours(10), oneHourTruckFactory}, // 2 trucks, 2 stations
            std::unordered_map<std::string, ILog::MachineSummary>{
                {"Truck_0", ILog::MachineSummary{
                    std::map<std::string, ILog::StateSummary>{
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    },
                    {}
                }},
                {"Truck_1", ILog::MachineSummary{
                    std::map<std::string, ILog::StateSummary>{
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    },
                    {}
                }}
            }
        }
    )
);


/*
Check against predicted resuslts for fixed time mining
*/
TEST_P(PredictedTest, BasicSimulationRuns) 
{
    ASSERT_NO_THROW(run());

    const auto& expected = GetParam().expectedSummary;
    for (const auto& [machineId, expectedSummary] : expected) 
    {
        const auto& actualSummary = m_simulation->log().summary(machineId);

        for (const auto& [state, expectedState] : expectedSummary.complette) 
        {
            const auto& actualState = actualSummary.complette.at(state);
            
            std::cout << "[Check] " << machineId << " - " << state
                << ": expected " << expectedState.occurrences << "x/" << toString(expectedState.totalDuration) << ", "
                << "actual " << actualState.occurrences << "x/" << toString(actualState.totalDuration) << std::endl;

            EXPECT_EQ(actualState.occurrences, expectedState.occurrences);
            EXPECT_EQ(actualState.totalDuration, expectedState.totalDuration);
        }
    }
}

int main(int argc, char** argv) 
{
    std::cout << "Hello from Helium-3 mining!" << std::endl << std::endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
