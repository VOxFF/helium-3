#include "implementation/MiningTruck.h"

namespace Helium3::Testing {

class FixedTimeTruck : public MiningTruck {
public:
    FixedTimeTruck(const std::string id, Duration t) : MiningTruck(id), m_miningTime(t) {}

protected:
    // --- From ITruck ---
    Duration miningTime() const override { return m_miningTime; }

private:
    Duration m_miningTime;
};
    
} // end of namespace Helium3::Testing
