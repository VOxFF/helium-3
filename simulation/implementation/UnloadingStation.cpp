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

const std::string& UnloadingStation::stateName(UnloadingStation::StateID id) {
    auto it = STATE_NAMES.find(id);
    static const std::string idle = "Idle";
    return (it != STATE_NAMES.end()) ? it->second : idle;
}

Events UnloadingStation::enqueue(ITruck* truck) {
    assert(truck);

    if (m_unloading == nullptr) {
        auto events = startUnloading(truck);
        if (m_callback) 
            m_callback();  // Notify manager

        return events;
    }

    // Truck needs to wait 
    auto events = truck->startWaiting();
    auto& waitEvent = events.front();
    

    Time currentTime = waitEvent.start;  // This will be set by simulation
    waitEvent.duration = getWaitTime(currentTime);
    
    m_queue.push(truck);

    if (m_callback) 
        m_callback();  // Notify manager of queue size change

    waitEvent.onExpiration = [this]() -> Events {
        return dequeue();
    };

    return events;
}

Events UnloadingStation::dequeue() {
    m_unloading = nullptr;

    if (m_queue.empty()) {
        m_state = Waiting;
        if (m_callback) 
            m_callback();  // Notify manager: now idle
        return {};         // We do not know wait duration
    }

    auto truck = m_queue.front();
    m_queue.pop();

    auto events = startUnloading(truck);
    if (m_callback) 
        m_callback();  // Notify manager: new truck started
    return events;
}

Events UnloadingStation::startUnloading(ITruck* truck) {
    assert(truck && "startUnloading called with nullptr truck");

    m_unloading = truck;
    m_state = Unloading;

    auto events = truck->unload();
    auto& moveEvent = events.front();
    

    m_currentUnloadingEnd = moveEvent.start + UNLOAD_TIME;
    
    auto truckCbk = moveEvent.onExpiration;
    //need to chain these events
    moveEvent.onExpiration = [this, truckCbk]() -> Events {
        Events events = truckCbk ? truckCbk() : Events{};     
        events += dequeue();
        return events;
    };

    return events;
}


Duration UnloadingStation::getWaitTime(Time currentTime) const {
    Duration waitTime = Duration::zero();
    
    // If there's a truck currently unloading, add remaining time
    if (m_unloading != nullptr && m_currentUnloadingEnd > currentTime) {
        waitTime += m_currentUnloadingEnd - currentTime;
    }
    
    // Add time for all trucks in queue (5 minutes each)
    waitTime += UNLOAD_TIME * m_queue.size();
    
    return waitTime;
}

} // namespace Helium3