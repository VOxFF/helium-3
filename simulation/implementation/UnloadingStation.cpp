#include "UnloadingStation.h"
#include "interfaces/ITruck.h"
#include <cassert>

namespace Helium3 {

namespace {

constexpr Duration UNLOAD_TIME = std::chrono::minutes(5);

const std::unordered_map<UnloadingStation::StateID, std::string> STATE_NAMES = {
    {UnloadingStation::Unloading,   "Unloading"},
    {UnloadingStation::Waiting,     "Waiting"}
};

} // end of anonymous namespace

const std::string& UnloadingStation::stateName(UnloadingStation::StateID id) 
{
    auto it = STATE_NAMES.find(id);
    static const std::string idle = "Idle";
    return (it != STATE_NAMES.end()) ? it->second : idle;
}

Events UnloadingStation::enqueue(ITruck* truck) 
{
    assert(truck);

    if (m_unloading == nullptr) 
        return startUnloading(truck);

    // Truck needs to wait 
    auto events = truck->startWaiting();
    auto& waitEvent = events.front();
    
    waitEvent.duration = getWaitTime(waitEvent.start);
    waitEvent.message += " at " + id();
    waitEvent.onExpiration = [this]() { return dequeue(); };
    m_queue.push(truck);

    return events;
}

Events UnloadingStation::dequeue() 
{
    m_unloading = nullptr;

    if (m_queue.empty()) {
        m_state = Waiting;
        return {};         // No Event
    }

    auto truck = m_queue.front();
    m_queue.pop();

    return startUnloading(truck);
}

Events UnloadingStation::startUnloading(ITruck* truck) 
{
    assert(truck && "startUnloading called with nullptr truck");

    m_unloading = truck;
    m_state = Unloading;

    auto events = truck->unload();
    auto& unloadEvent = events.front();
    unloadEvent.message += " at " + id();
    
    m_currentUnloadingEnd = unloadEvent.start + UNLOAD_TIME;
    
    auto truckCbk = unloadEvent.onExpiration;
    unloadEvent.onExpiration = [this, truckCbk]() -> Events {
        Events events = truckCbk ? truckCbk() : Events{};     
        events += dequeue();
        return events;
    };

    return events;
}


Duration UnloadingStation::getWaitTime(Time currentTime) const 
{
    Duration waitTime = Duration::zero();
    
    // If there's a truck currently unloading, add remaining time
    if (m_unloading != nullptr && m_currentUnloadingEnd > currentTime) 
        waitTime += m_currentUnloadingEnd - currentTime;
    
    // Add time for all trucks in queue (5 minutes each)
    waitTime += UNLOAD_TIME * m_queue.size();
    
    return waitTime;
}

} // namespace Helium3