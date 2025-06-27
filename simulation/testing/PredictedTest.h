#pragma once

#include "SimulationTest.h"

namespace Helium3::Testing {

/*
This test is used with non-random mining / predictable mining durations 
*/
struct PredictedParams {
    SimulationParams simulationParams;
    std::unordered_map<std::string, ILog::MachineSummary> expectedSummary;
};


class PredictedTest : public SimulationTest, public ::testing::TestWithParam<PredictedParams> {
protected:
    void SetUp() override {
        init(GetParam().simulationParams);
    }
};

} // namespace Helium3::Testing
