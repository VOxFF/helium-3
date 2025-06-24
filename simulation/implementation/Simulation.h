#pragma once

#include "interfaces/ISimulation.h"
#include "common/Event.h"

#include <vector>
#include <memory>
#include <queue>

namespace Helium3 {

class ITrucksManager;
class IStationsManager;

class Simulation : public ISimulation {
public:
    Simulation();
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