#include "SimpleTest.h"
#include <chrono>

using namespace Helium3;
using namespace Helium3::Testing;

/**
 * @brief Runs the simulation for 72 h with random mining durations and
 *        prints all completed and unfinished event logs.
 */
INSTANTIATE_TEST_SUITE_P(
    BasicSimulations,   // Instance name
    SimpleTest,         // Test suit name
    ::testing::Values(
        SimulationParams{ 1, 1, std::chrono::hours(72), {} },  // 1 truck, 1 station
        SimulationParams{ 3, 2, std::chrono::hours(72), {} },  // 3 trucks, 2 stations
        SimulationParams{ 10, 1, std::chrono::hours(72), {} }, // 5 trucks, 1 station
        SimulationParams{ 3, 3, std::chrono::hours(72), {} }   // 3 trucks, 3 stations
    )
);

TEST_P(SimpleTest, SimulationRunsWithoutThrow) {
    ASSERT_NO_THROW(run());

    auto& log = m_simulation->log();

    std::cout << std::endl << "--- Completed Events ---" << std::endl;
    for (const auto& e : log.events(ILog::TaskState::Complete)) {
        printEvent(e, ILog::TaskState::Complete);
    }

    std::cout << std::endl << "--- Unfinished Events ---" << std::endl;
    for (const auto& e : log.events(ILog::TaskState::Unfinished)) {
        printEvent(e, ILog::TaskState::Unfinished);
    }

    std::cout << std::endl << "--- Summary for All Machines ---" << std::endl;
    printSummary();

    std::cout << std::endl << "--- Analytics for All Machines ---" << std::endl;
    printAnalytics();
}
