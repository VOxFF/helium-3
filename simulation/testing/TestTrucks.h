#include "implementation/MiningTruck.h"

namespace Helium3::Testing {

class FixedTimeTruck : public MiningTruck {
public:
    FixedTimeTruck(IStationManager& mgr, const std::string id, Duration t) : MiningTruck(mgr, id), m_miningTime(t) {}

protected:
    // --- From MiningTruck ---
    Duration miningTime() const override { return m_miningTime; }

private:
    Duration m_miningTime;
};
    
} // end of namespace Helium3::Testing
