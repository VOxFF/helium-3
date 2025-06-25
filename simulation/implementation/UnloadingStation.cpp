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

Event UnloadingStation::enqueue(ITruck* truck) {
    assert(truck);

    if (m_unloading == nullptr) {
        auto event = startUnloading(truck);
        if (m_callback) 
            m_callback();  // Notify manager
        return event;
    }

    // Truck needs to wait 
    auto unloadEvent = truck->startWaiting();
    unloadEvent.duration = WAIT_TIME * m_queue.size();
    m_queue.push(truck);

    if (m_callback) 
        m_callback();  // Notify manager of queue size change

    auto truckCbk = unloadEvent.onExpiration;
    unloadEvent.onExpiration = [this, truckCbk]() -> Event {
        if (truckCbk) truckCbk();
        return this->dequeue();
    };

    return unloadEvent;
}

Event UnloadingStation::dequeue() {
    m_unloading = nullptr;

    if (m_queue.empty()) {
        m_state = Waiting;
        if (m_callback) 
            m_callback();  // Notify manager: now idle
        return {};
    }

    auto truck = m_queue.front();
    m_queue.pop();

    auto event = startUnloading(truck);
    if (m_callback) 
        m_callback();  // Notify manager: new truck started
    return event;
}

Event UnloadingStation::startUnloading(ITruck* truck) {
    assert(truck && "startUnloading called with nullptr truck");

    m_unloading = truck;
    m_state = Unloading;

    auto moveEvent = truck->unload();
    auto truckCbk = moveEvent.onExpiration;

    moveEvent.onExpiration = [this, truckCbk]() -> Event {
        if (truckCbk) 
            return truckCbk().value();
            
        return this->dequeue();
    };

    return moveEvent;
}

} // namespace Helium3
