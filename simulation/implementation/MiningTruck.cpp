#include "MiningTruck.h"

#include <unordered_map>
#include <random>
#include <cassert> 

namespace Helium3 {

namespace {

constexpr Duration MINING_TIME_MIN = std::chrono::hours(1);
constexpr Duration MINING_TIME_MAX = std::chrono::hours(5);
constexpr Duration DRIVE_TIME = std::chrono::minutes(30);
constexpr Duration UNLOAD_TIME = std::chrono::minutes(5);

const std::unordered_map<MiningTruck::StateID, std::string> STATE_NAMES = {
    {MiningTruck::Mining,           "Mining"},
    {MiningTruck::MovingToStation,  "Moving to Unloading Station"},
    {MiningTruck::ArrivedToStation, "Arrived at Unloading Station"},
    {MiningTruck::WaitingToUnload,  "Waiting for Unloading"},
    {MiningTruck::Unloading,        "Unloading"},
    {MiningTruck::MovingToMining,   "Returning to Mining Site"}
};

} // end of anonymous namespace

const std::string& MiningTruck::stateName(MiningTruck::StateID id) {
    auto it = STATE_NAMES.find(id);
    static const std::string idle = "Idle";
    return (it != STATE_NAMES.end()) ? it->second : idle;
}

Event MiningTruck::startMining() {
    assert(m_state == Idle || m_state == MovingToMining);
    m_state = Mining;

    
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<double> dist(MINING_TIME_MIN.count(), MINING_TIME_MAX.count());
    auto duration = std::chrono::duration_cast<Duration>(std::chrono::duration<double>(dist(rng)));

    return Event(
        Time{}, // time to be set externally
        duration, 
        [this]() { return driveToStation(); }
    );
}

Event MiningTruck::driveToStation() {
    assert(m_state == Mining);
    m_state = MovingToStation;

    return Event(
        Time{}, 
        DRIVE_TIME, 
        [this]() { return checkinAtStation(); }
    );
}

Event MiningTruck::checkinAtStation() {
    assert(m_state == MovingToStation);
    m_state = ArrivedToStation;

    return Event(
        Time{}, 
        Duration{}, // zero-duration event
        {}          // callback to be overridden by Station
    );
}

Event MiningTruck::startWaiting() {
    assert(m_state == ArrivedToStation);
    m_state = WaitingToUnload;

    return Event(
        Time{}, 
        Duration{}, // Station will define duration
        [this]() { return unload(); } // can be wrapped/overridden by Station
    );
}

Event MiningTruck::unload() {
    assert(m_state == ArrivedToStation || m_state == WaitingToUnload);
    m_state = Unloading;

    return Event(
        Time{}, 
        UNLOAD_TIME, 
        [this]() { return driveToMining(); }
    );
}

Event MiningTruck::driveToMining() {
    assert(m_state == Unloading);
    m_state = MovingToMining;

    return Event(
        Time{}, 
        DRIVE_TIME, 
        [this]() { return startMining(); }
    );
}

} // namespace Helium3
