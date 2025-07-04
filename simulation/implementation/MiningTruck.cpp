#include "MiningTruck.h"

#include <unordered_map>
#include <random>
#include <cassert> 

namespace Helium3 {

namespace {

constexpr double MINING_TIME_MIN = 1.0;
constexpr double MINING_TIME_MAX = 5.0;
constexpr Duration DRIVE_TIME = std::chrono::minutes(30);
constexpr Duration UNLOAD_TIME = std::chrono::minutes(5);

const std::string PREFIX("Truck_");

const std::unordered_map<MiningTruck::StateID, std::string> STATE_NAMES = {
    {MiningTruck::Idle,             "Idle"},
    {MiningTruck::Mining,           "Mining"},
    {MiningTruck::MovingToStation,  "Moving"},
    {MiningTruck::ArrivedToStation, "Arrived"},
    {MiningTruck::WaitingToUnload,  "Waiting"},
    {MiningTruck::Unloading,        "Unloading"},
    {MiningTruck::MovingToMining,   "Moving"}
};


const std::unordered_map<MiningTruck::StateID, std::string> STATE_MESSAGES = {
    {MiningTruck::Mining,           "Mining in progress"},
    {MiningTruck::MovingToStation,  "Moving for unloading"},
    {MiningTruck::ArrivedToStation, "Arrived for unloading"},
    {MiningTruck::WaitingToUnload,  "Waiting for unloading"},
    {MiningTruck::Unloading,        "Unloading in progress"},
    {MiningTruck::MovingToMining,   "Returning for mining"}
};

const std::string& stateName(MiningTruck::StateID id) {
    auto it = STATE_NAMES.find(id);
    static const std::string undefined = "Undefined";
    return (it != STATE_NAMES.end()) ? it->second : undefined;
}

const std::string& stateMessage(MiningTruck::StateID id) {
    auto it = STATE_MESSAGES.find(id);
    static const std::string undefined = "Undefined";
    return (it != STATE_MESSAGES.end()) ? it->second : undefined;
}

} // end of anonymous namespace



const std::string& MiningTruck::namePrefix() {
    return PREFIX;
}

Duration MiningTruck::miningTime() const 
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<double> dist(MINING_TIME_MIN, MINING_TIME_MAX); // in hours
    double hours = dist(rng);

    return std::chrono::duration_cast<Duration>(std::chrono::duration<double, std::ratio<3600>>(hours)); //to duration
}

Event MiningTruck::makeEvent(Time start, Duration duration, EventCallback onExpirationCbk) const 
{
    auto stateID = static_cast<StateID>(state());
    return Event(id(), state(), stateName(stateID), start, duration, onExpirationCbk, stateMessage(stateID));
}

Events MiningTruck::startMining() {
    assert(m_state == Idle || m_state == MovingToMining);
    m_state = Mining;

    return {makeEvent(
        Time{}, // time to be set externally
        miningTime(), 
        [this]() { return driveToStation(); }
    )};
}

Events MiningTruck::driveToStation() {
    assert(m_state == Mining);
    m_state = MovingToStation;

    return { makeEvent(
        Time{}, 
        DRIVE_TIME, 
        [this]() { return checkinAtStation(); }
    )};
}

Events MiningTruck::checkinAtStation() {
    assert(m_state == MovingToStation);
    m_state = ArrivedToStation;

    return { makeEvent(
        Time{}, 
        Duration{}, // zero-duration event
        {}          // Handled by Station Manager
    )};
}

Events MiningTruck::startWaiting() {
    assert(m_state == ArrivedToStation);
    m_state = WaitingToUnload;

    return { makeEvent(
        Time{}, 
        Duration{}, // Station will define duration
        [this]() { return unload(); } // can be wrapped/overridden by Station
    )};
}

Events MiningTruck::unload() {
    assert(m_state == ArrivedToStation || m_state == WaitingToUnload);
    m_state = Unloading;

    return { makeEvent(
        Time{}, 
        UNLOAD_TIME, 
        [this]() { return driveToMining(); }
    )};
}

Events MiningTruck::driveToMining() {
    assert(m_state == Unloading);
    m_state = MovingToMining;

    return { makeEvent(
        Time{}, 
        DRIVE_TIME, 
        [this]() { return startMining(); }
    )};
}

} // namespace Helium3
