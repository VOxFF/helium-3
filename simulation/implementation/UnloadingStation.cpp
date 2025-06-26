#include "UnloadingStation.h"
#include "interfaces/ITruck.h"
#include <cassert>

namespace Helium3 {

namespace {

constexpr Duration WAIT_TIME = std::chrono::minutes(5);

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
    auto& unloadEvent = events.front();
    // Account for the truck currently unloading by adding 1
    // to the number of vehicles already waiting in the queue.
    unloadEvent.duration = WAIT_TIME * (m_queue.size() + 1);
    m_queue.push(truck);

    if (m_callback) 
        m_callback();  // Notify manager of queue size change

    auto truckCbk = unloadEvent.onExpiration;
    // Need to chain these events
    unloadEvent.onExpiration = [this, truckCbk]() -> Events {
        Events events = truckCbk ? truckCbk() : Events{};     
        events += dequeue();
        return events;
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
    auto truckCbk = moveEvent.onExpiration;
    //need to chain these events
    moveEvent.onExpiration = [this, truckCbk]() -> Events {
        Events events = truckCbk ? truckCbk() : Events{};     
        events += dequeue();
        return events;
    };

    return events;
}

} // namespace Helium3
