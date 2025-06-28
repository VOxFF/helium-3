#include "Simulation.h"
#include "TrucksManager.h"
#include "StationManager.h"
#include "MiningTruck.h"
#include "Log.h"

#include "common/Time.h"

#include <cassert>

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

void Simulation::start(const Time& t) 
{
    for (auto* truck : m_trucksManager->trucks())
        for (auto event : truck->startMining()) 
        {
            event.start = t;
            m_eventQueue.push(event);
        }
}

void Simulation::run(const Duration& simulationLength) 
{
    auto startTime = Clock::now();
    auto endTime = startTime + simulationLength;
    auto curTime = startTime;

    // Initialize the event loop
    start(curTime);

    // Process events until time expires or the event queue becomes empty
    while (!m_eventQueue.empty() && m_eventQueue.top().end() < endTime)
    {
        auto event = m_eventQueue.top();
        m_eventQueue.pop();

        log().add(event);

        curTime = event.end();
        Events nextEvents;

        if (event.state == MiningTruck::StateID::ArrivedToStation)
        {
            auto station = m_stationsManager->getOptimalStation(curTime); assert(station);
            auto truck = m_trucksManager->truck(event.machineId); assert(truck);
            nextEvents = station->enqueue(truck);
        }
        else if (event.onExpiration)
        {
            nextEvents = event.onExpiration();
        }

        for (auto next : nextEvents) 
        {
            next.start = curTime;
            m_eventQueue.push(next);
        }
    }

    // It may happen that the time has expired, but there are still unfinished operations in the queue.
    finish(endTime);
}

void Simulation::finish(const Time& t) 
{
    while (!m_eventQueue.empty())
    {
        auto event = m_eventQueue.top();
        m_eventQueue.pop();

        event.duration = t - event.start;
        log().add(event, ILog::TaskState::Unfinished);
    }
}

} // namespace Helium3
