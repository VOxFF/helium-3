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


Event UnloadingStation::enqueue(ITruck* truck) {
    assert(truck);

    if (m_unloading == nullptr) 
        return startUnloading(truck);
    

    // Truck needs to wait 
    auto unloadEvent = truck->startWaiting();
    unloadEvent.duration = WAIT_TIME * m_queue.size();
    m_queue.push(truck);
    
    auto truckCbk = unloadEvent.onExpiration;
    unloadEvent.onExpiration = [this, truckCbk]() -> Event {
        if (truckCbk) 
            truckCbk();
        return this->dequeue();
    };

    return unloadEvent;
}

Event UnloadingStation::dequeue() {
    
    m_unloading = nullptr;
    if(m_queue.empty()){
        m_state = Waiting;
        return Event{{}, {}, {}};
    }
        
    auto truck = m_queue.front();
    m_queue.pop();

    return startUnloading(truck);
}

Event UnloadingStation::startUnloading(ITruck* truck) {
    m_unloading = truck;
    m_state = Unloading;

    auto moveEvent = truck->unload();
    auto truckCbk = moveEvent.onExpiration;

    moveEvent.onExpiration = [this, truckCbk]() -> Event {
        if (truckCbk) 
            truckCbk();
        return this->dequeue();
    };

    return moveEvent;
}

} // namespace Helium3