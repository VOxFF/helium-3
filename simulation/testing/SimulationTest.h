#pragma once

#include "implementation/Simulation.h"
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


} // namespace Helium3::Testing
