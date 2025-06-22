

#pragma once
#include "interfaces/ITruck.h"
#include "common/Log.h"


namespace Helium3 {


class MiningTruck : public ITruck {
public:
    MiningTruck(const std::string& id) : m_id(id), m_state(Idle) {}
    virtual ~MiningTruck() = default;

    enum StateID {
        Idle = -1,
        Mining,
        MovingToStation,
        ArrivedToStation,
        WatingToUnlodad,
        Unloading,
        MovingToMining,

        StatesCount // total number of states (not including Idle)
    };
    const std::string& stateName(StateID);

    /*
    from IMachine
    */
    const std::string& id() const override {return m_id;}
    const State& getState() const override {return m_state;}
    const Log& log() const override {return m_log;}

   /*
   from ITruck
   */
    
    Event startMining() override;
    Event driveToStation() override;
    Event checkinAtStation() override;
    Event startWaiting() override;
    Event unload() override;
    Event driveToMining() override;


    // --- future truck-specific API will appear here ---

private:
    std::string m_id;
    State m_state;
    Log m_log;
   

};

} // namespace Helium3
