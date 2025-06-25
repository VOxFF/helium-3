#include "Simulation.h"
#include "TrucksManager.h"
#include "StationManager.h"
#include "Log.h"

#include "common/Time.h"

#include <iostream> //temp

namespace Helium3 {

Simulation::Simulation(const TruckFactory& factory) 
: m_stationsManager(std::make_unique<StationManager>())
, m_trucksManager(std::make_unique<TrucksManager>(factory))
, m_log(std::make_unique<Log>()) {}

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

        m_log->add(event);

        curTime = event.end();
        if(!event.onExpiration)
            continue; // no action to perform

        if (auto nextOpt = event.onExpiration(); nextOpt.has_value()) 
        {
            auto next = nextOpt.value();
            next.start = curTime;
            m_eventQueue.push(next);
        }
    }
    std::cout << "queue size: " << m_eventQueue.size() << std::endl; //some items can still remain in queue after expiraton, uncomplette tasks
}

ILog& Simulation::log() const 
{
    return *m_log;
}

} // namespace Helium3