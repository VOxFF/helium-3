#pragma once

#include "interfaces/ITruck.h"

namespace Helium3 {

class MiningTruck : public ITruck {
public:
    explicit MiningTruck(IStationManager& mgr, const std::string& id) : m_stationManager(mgr), m_id(id), m_state(Idle) {}
    ~MiningTruck() override = default;

    enum StateID {
        Idle = -1,              ///< Special state used for initialization
        Mining = 10,
        MovingToStation,
        ArrivedToStation,
        WaitingToUnload,
        Unloading,
        MovingToMining,
    };

    static const std::string& stateName(StateID id);
    static const std::string& namePrefix();

    // --- From IMachine ---
    const std::string& id() const override { return m_id; }
    const State& state() const override { return m_state; }
    

    // --- From ITruck ---
    Events startMining() override;
    Events driveToStation() override;
    Events checkinAtStation() override;
    Events startWaiting() override;
    Events unload() override;
    Events driveToMining() override;

    // --- Future truck-specific API will appear here ---

protected:
    /**
     * @brief Returns the duration of the mining operation.
     *
     * The default implementation returns a random time in the range between
     * 1 and 5 hours. However, for testing purposes, overriding this method
     * in test-specific subclasses (e.g., `TestTruck`) can provide fixed durations
     * for more predictable behavior.
     *
     * @return Duration of the mining operation.
     */
    virtual Duration miningTime() const;

    Event makeEvent(Time start, Duration duration, EventCallback onExpirationCbk) const;

private:
    
    IStationManager& m_stationManager; ///<Requried by Truck to pick up the best Station
    std::string m_id;
    State m_state;
};

} // namespace Helium3
