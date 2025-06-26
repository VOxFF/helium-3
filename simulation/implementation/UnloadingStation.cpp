#include "UnloadingStation.h"
#include "interfaces/ITruck.h"
#include <cassert>

namespace Helium3 {

namespace {

constexpr Duration UNLOAD_TIME = std::chrono::minutes(5);

const std::unordered_map<UnloadingStation::StateID, std::string> STATE_NAMES = {
    {UnloadingStation::Idle,        "Idle"},
    {UnloadingStation::Unloading,   "Unloading"},
    {UnloadingStation::Waiting,     "Waiting"}
};

const std::string& stateName(UnloadingStation::StateID id) 
{
    auto it = STATE_NAMES.find(id);
    static const std::string undefined = "Undefined";
    return (it != STATE_NAMES.end()) ? it->second : undefined;
}

} // end of anonymous namespace



Events UnloadingStation::enqueue(ITruck* truck) 
{
    assert(truck && "enqueue called with nullptr truck");

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
        return {};  // No Event
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
    unloadEvent.onExpiration = [this, truckCbk = unloadEvent.onExpiration]() {
        Events events = truckCbk ? truckCbk() : Events{};     
        events += dequeue();
        return events;
    };

    // Add event for logging Station activity
    auto stateID = static_cast<StateID>(state());
    events.push_back(Event(id(), state(), stateName(stateID), {}, UNLOAD_TIME, {}, stateName(stateID) + " of " + truck->id()));

    m_currentUnloadingEnd = unloadEvent.start + UNLOAD_TIME;
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