#pragma once

#include "implementation/Simulation.h"
#include "implementation/TrucksManager.h"
#include "common/Time.h"

#include <gtest/gtest.h>
#include <memory>
#include <functional>

namespace Helium3::Testing {

    
struct SimulationParams {
    unsigned int truckCount;
    unsigned int stationCount;
    Duration simulationLength;
    TruckFactory factory;
};

class SimulationTest {
protected:
    void init(const SimulationParams& params) {
        m_simulation = std::make_unique<Simulation>(params.factory);
        m_simulation->initialize(params.truckCount, params.stationCount);
        m_length = params.simulationLength;
    }

    void run() {
        m_simulation->run(m_length);
    }

    std::unique_ptr<ISimulation> m_simulation;
    Duration m_length;
};

/*
This test is used with non-random mining / predicatable mining duratons 
*/
struct PredictedParams {
    SimulationParams simulatonParams;
    std::vector<std::string> expectedLogs;
};


class PredictedTest : public SimulationTest, public ::testing::TestWithParam<PredictedParams> {
protected:
    void SetUp() override {
        init(GetParam().simulatonParams);
    }
};



} // namespace Helium3::Testing
