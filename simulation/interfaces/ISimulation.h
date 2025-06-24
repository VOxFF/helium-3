#pragma once

#include "common/Time.h"

namespace Helium3 {

class ISimulation {
public:
    virtual ~ISimulation() = default;

    virtual void initialize(unsigned int truckCount, unsigned int stationCount) = 0;
    virtual void run(const Duration& simulationLength) = 0;
};

} // namespace Helium3