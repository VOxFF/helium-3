#pragma once

#include "interfaces/ITruck.h"
#include "common/Log.h"

namespace Helium3 {

class MiningTruck : public ITruck {
public:
    explicit MiningTruck(const std::string& id) : m_id(id), m_state(Idle) {}
    ~MiningTruck() override = default;

    enum StateID {
        Idle = -1,              ///< Special state used for initialization
        Mining,
        MovingToStation,
        ArrivedToStation,
        WaitingToUnload,
        Unloading,
        MovingToMining,

        StatesCount             ///< Total number of states (excluding Idle)
    };

    static const std::string& stateName(StateID id);

    // --- From IMachine ---
    const std::string& id() const override { return m_id; }
    const State& getState() const override { return m_state; }
    const Log& log() const override { return m_log; }

    // --- From ITruck ---
    Event startMining() override;
    Event driveToStation() override;
    Event checkinAtStation() override;
    Event startWaiting() override;
    Event unload() override;
    Event driveToMining() override;

    // --- Future truck-specific API will appear here ---

private:
    std::string m_id;
    State m_state;
    Log m_log;
};

} // namespace Helium3
