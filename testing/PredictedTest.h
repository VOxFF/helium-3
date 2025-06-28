#pragma once

#include "SimulationTest.h"

namespace Helium3::Testing {

/**
 * @brief Holds parameters and expected outcomes for predictable‐duration simulation tests.
 *
 * Each instance specifies the number of trucks, stations, total simulation length,
 * and the expected summary of state occurrences and durations.
 */
struct PredictedParams {
    SimulationParams simulationParams;
    std::unordered_map<std::string, ILog::MachineSummary> expectedSummary;
};

/**
 * @brief Parameterized test using fixed‐time mining to generate predictable results.
 *
 * Runs SimulationTest::run() with non‐random mining durations so that the
 * outputs can be verified against the provided expectations.
 */
class PredictedTest : public SimulationTest, public ::testing::TestWithParam<PredictedParams> {
protected:
    void SetUp() override {
        init(GetParam().simulationParams);
    }
};

} // namespace Helium3::Testing
