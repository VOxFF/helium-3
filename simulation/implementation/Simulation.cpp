#include "Simulation.h"
#include "TrucksManager.h"
#include "StationsManager.h"
#include "MiningTruck.h"

#include "common/Time.h"

namespace Helium3 {

Simulation::Simulation() 
: m_trucksManager(std::make_unique<TrucksManager>())
, m_stationsManager(std::make_unique<StationsManager>()) {}

void Simulation::initialize(unsigned int truckCount, unsigned int stationCount) 
{
    m_trucksManager->initialize(truckCount);
    m_stationsManager->initialize(stationCount);
}

void Simulation::run(const Duration& simulationLength) 
{
    auto startTime = Clock::now();
    auto endTime = startTime + simulationLength;
    auto curTime = startTime;

    for(auto track : m_trucksManager->trucks()){
        auto event = track->startMining();
        event.start = curTime; 
        m_eventQueue.push(event);
    }
    while(!m_eventQueue.empty() && m_eventQueue.top().end() < endTime)
    {
        auto event = m_eventQueue.top();
        m_eventQueue.pop();

        curTime = event.end();

        if(!event.onExpiration)
            continue; // no action to perform

        if (auto nextOpt = event.onExpiration(); nextOpt.has_value()) {
                auto next = nextOpt.value();
                next.start = curTime;
                m_eventQueue.push(next);
            }
    
    }
}

} // namespace Helium3