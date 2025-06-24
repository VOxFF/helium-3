#pragma once

#include "interfaces/ISimulation.h"
#include "interfaces/ITrucksManager.h"
#include "interfaces/IStationsManager.h"
#include "interfaces/ITruck.h"
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

private:
    std::unique_ptr<ITrucksManager> m_trucksManager;
    std::unique_ptr<IStationsManager> m_stationsManager;

    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> m_eventQueue;
};

} // namespace Helium3