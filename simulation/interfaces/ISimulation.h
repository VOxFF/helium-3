#pragma once

#include "common/Time.h"

namespace Helium3 {

class ILog;

/**
 * @brief Interface for a mining simulation.
 *
 * Defines the lifecycle of a simulation, including setup, execution, and
 * access to its event log.
 */
class ISimulation {
public:
    virtual ~ISimulation() = default;

    /**
     * @brief Initialize the simulation environment.
     *
     * Sets up the specified number of trucks and stations before running.
     *
     * @param truckCount   Number of trucks to include in the simulation.
     * @param stationCount Number of stations to include in the simulation.
     */
    virtual void initialize(unsigned int truckCount, unsigned int stationCount) = 0;

    /**
     * @brief Run the simulation for the given duration.
     *
     * Processes all scheduled events until the simulation time elapses.
     *
     * @param simulationLength The total time span to simulate.
     */
    virtual void run(const Duration& simulationLength) = 0;

    /**
     * @brief Obtain the logger that records simulation events.
     *
     * @return ILog& Reference to the simulation's event logger.
     */
    virtual ILog& log() const = 0;
};

} // namespace Helium3