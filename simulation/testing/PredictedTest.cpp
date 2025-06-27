#include "PredictedTest.h"
#include "TestTrucks.h"

#include "common/State.h"

#include <iostream>


namespace Helium3::Testing {

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
        PredictedParams{  //test case 1
            {1, 1, std::chrono::hours(10) + std::chrono::minutes(10), oneHourTruckFactory}, // 1 truck, 1 station
            {{
                {"Truck_0", ILog::MachineSummary{
                    {{
                        // complete
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    }},
                    {{
                        // unfinished
                        {"Moving", {1, std::chrono::minutes(15)}}
                    }}
                }},
                {"Station_0", ILog::MachineSummary{
                    {{
                        // complete
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    }},
                    {
                        // unfinished
                    }
                }}
            }}
        },
        PredictedParams{  //test case 2
            {2, 2, std::chrono::hours(10), oneHourTruckFactory}, // 2 trucks, 2 stations
            {{
                {"Truck_0", ILog::MachineSummary{
                    {{
                        // complete
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    }},
                    {{
                        // unfinished
                        {"Moving", {1, std::chrono::minutes(5)}}
                    }}
                }},
                {"Truck_1", ILog::MachineSummary{
                    {{
                        // complete
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    }},
                    {{
                        // unfinished
                        {"Moving", {1, std::chrono::minutes(5)}}
                    }}
                }},
                {"Station_0", ILog::MachineSummary{
                    {{
                        // complete
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    }},
                    {
                        // unfinished
                    }
                }},
                {"Station_1", ILog::MachineSummary{
                    {{
                        // complete
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    }},
                    {
                        // unfinished
                    }
                }}
            }}
        },
        PredictedParams{  //test case 3
            {2, 1, std::chrono::hours(10), oneHourTruckFactory}, // 2 trucks, 1 station
            {{
                
                {"Truck_0", ILog::MachineSummary{
                    {{
                        // complete
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {4, std::chrono::minutes(20)}},
                        {"Waiting",   {1, std::chrono::minutes(5)}}
                    }},
                    {{
                        // unfinished
                        {"Unloading", {1, std::chrono::minutes(5)}}
                    }}
                }},
                {"Truck_1", ILog::MachineSummary{
                    {{
                        // complete
                        {"Mining",    {5, std::chrono::hours(5)}},
                        {"Moving",    {9, std::chrono::minutes(270)}},
                        {"Arrived",   {5, std::chrono::minutes(0)}},
                        {"Unloading", {5, std::chrono::minutes(25)}}
                    }},
                    {{
                        // unfinished
                        {"Moving", {1, std::chrono::minutes(5)}}
                    }}
                }},
                {"Station_0", ILog::MachineSummary{
                    {{
                        // complete
                        {"Unloading", {9, std::chrono::minutes(45)}}
                    }},
                    {{
                        // unfinished
                        {"Unloading", {1, std::chrono::minutes(5)}}
                    }}
                }}
            }}
        }
    )
);


/*
Check against predicted results for fixed time mining
*/
TEST_P(PredictedTest, BasicSimulationRuns) 
{
    ASSERT_NO_THROW(run());

    const auto& expected = GetParam().expectedSummary;
    for (const auto& [machineId, expectedSummary] : expected) 
    {
        const auto& actualSummary = m_simulation->log().summary(machineId);

        for (const auto& [state, expectedState] : expectedSummary.complete) 
        {
            const auto& actualState = actualSummary.complete.at(state);
            
            std::cout << "[Check] " << machineId << " - " << state
                << ": expected " << expectedState.occurrences << "x/" << toString(expectedState.totalDuration) << ", "
                << "actual " << actualState.occurrences << "x/" << toString(actualState.totalDuration) << std::endl;

            EXPECT_EQ(actualState.occurrences, expectedState.occurrences);
            EXPECT_EQ(actualState.totalDuration, expectedState.totalDuration);
        }
    }
}


inline void PrintTo(const Helium3::Testing::PredictedParams& params, std::ostream* os) {
    *os << "PredictedParams {\n"
        << "  trucks   = " << params.simulationParams.truckCount << ",\n"
        << "  stations = " << params.simulationParams.stationCount << ",\n"
        << "  duration = " 
        << std::chrono::duration_cast<std::chrono::hours>(params.simulationParams.simulationLength).count() << "h,\n"
        << "  expected = {\n";

    for (const auto& [machineId, summary] : params.expectedSummary) {
        *os << "    " << machineId << " => {\n";
        *os << "      complete = {\n";
        for (const auto& [state, stats] : summary.complete) {
            *os << "        " << state
                << ": (" << stats.occurrences << "x, "
                << std::chrono::duration_cast<std::chrono::minutes>(stats.totalDuration).count() << "m)\n";
        }
        *os << "      },\n";
        *os << "      unfinished = {\n";
        for (const auto& [state, stats] : summary.unfinished) {
            *os << "        " << state
                << ": (" << stats.occurrences << "x, "
                << std::chrono::duration_cast<std::chrono::minutes>(stats.totalDuration).count() << "m)\n";
        }
        *os << "      }\n";
        *os << "    },\n";
    }

    *os << "  }\n"
        << "}";
}


} // namespace Helium3::Testing

