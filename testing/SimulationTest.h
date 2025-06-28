#pragma once

#include "implementation/Simulation.h"
#include "common/Time.h"

#include <gtest/gtest.h>
#include <memory>
#include <functional>

namespace Helium3::Testing {

/**
 * @brief Configuration parameters for a simulation test.
 *
 * Encapsulates the number of trucks, number of stations, total run length,
 * and the factory used to create truck instances.
 */  
struct SimulationParams {
    unsigned int truckCount;
    unsigned int stationCount;
    Duration simulationLength;
    TruckFactory factory;
};

/**
 * @brief Base fixture for simulation-based tests.
 *
 * Provides common setup (`init`), execution (`run`), and event–printing
 * functionality (`printEvent`) for derived test suites.
 */
class SimulationTest {
protected:
    void init(const SimulationParams& params)
    {
        m_params = params;
        m_simulation = std::make_unique<Simulation>(params.factory);
        m_simulation->initialize(params.truckCount, params.stationCount);
    }

    void run() 
    {
        m_simulation->run(m_params.simulationLength);
    }

    void printEvent(const Event& e, ILog::TaskState state) const;
    void printSummary() const;  
    void printAnalytics() const;

    SimulationParams m_params;
    std::unique_ptr<ISimulation> m_simulation;
};


} // namespace Helium3::Testing
