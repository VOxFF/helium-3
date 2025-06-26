#pragma once

#include "SimulationTest.h"

namespace Helium3::Testing {

/*
This test is used with non-random mining / predicatable mining duratons 
*/
struct PredictedParams {
    SimulationParams simulatonParams;
    std::unordered_map<std::string, ILog::MachineSummary> expectedSummary;
};


class PredictedTest : public SimulationTest, public ::testing::TestWithParam<PredictedParams> {
protected:
    void SetUp() override {
        init(GetParam().simulatonParams);
    }
};

} // namespace Helium3::Testing
