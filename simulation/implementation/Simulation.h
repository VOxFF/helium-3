#pragma once

#include "interfaces/ISimulation.h"
#include "interfaces/ITrucksManager.h"
#include "interfaces/IStationManager.h"
#include "interfaces/ITruck.h"
#include "interfaces/ILog.h"

#include "common/Event.h"

#include <vector>
#include <memory>
#include <queue>

namespace Helium3 {

class Simulation : public ISimulation {
public:
    /**
     * @brief Constructs the Simulation with a custom truck factory.
     *
     * Supplies a custom truck factory to the Simulation, allowing override
     * of default `MiningTruck` creation. This is particularly useful for
     * injecting customized truck classes in testing scenarios.
     *
     * @param factory Optional factory function used to create truck instances.
     */
    Simulation(const TruckFactory& factory = {});
    virtual ~Simulation() = default;

    // --- From ISimulation ---
    void initialize(unsigned int truckCount, unsigned int stationCount) override;
    void run(const Duration& simulationLength) override;
    ILog& log() const override { return *m_log; }

protected:
    // --- From Simulation ---
    /**
     * @brief Starts the simulation at the given time.
     *
     * Initializes internal state to begin processing from the specified start time.
     *
     * @param t The start time of the simulation.
     */
    void start(const Time& t);

    /**
     * @brief Processes any unfinished jobs that remain in the queue.
     *
     * This is typically called after the main simulation loop to ensure all remaining
     * events are accounted for, even if they were not completed within the simulation duration.
     */
    void finish(const Time& t);

private:
    std::unique_ptr<IStationManager> m_stationsManager;
    std::unique_ptr<ITrucksManager> m_trucksManager;
    std::unique_ptr<ILog> m_log;

    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> m_eventQueue;
};

} // namespace Helium3