#pragma once

#include "common/Time.h"

#include <vector>
#include <memory>

namespace Helium3 {

class IStation;

/**
 * @brief Interface for managing a collection of unloading stations.
 *
 * This interface allows initializing and accessing a set of stations,
 * and selecting the optimal one for a truck to use. It is designed
 * for extension — future implementations may incorporate distance,
 * load balancing, or historical metrics.
 */
class IStationManager {
public:
    virtual ~IStationManager() = default;

    /// Initialize the manager with the given number of unloading stations.
    virtual void initialize(unsigned int stationCount) = 0;

    /// Return a list of all managed stations.
    virtual std::vector<IStation*> stations() const = 0;

    /**
     * @brief Return the optimal station to assign a truck to.
     *
     * Current selection logic may be based on minimal queue size,
     * round-robin distribution, or other heuristics depending on implementation.
     */
    virtual IStation* getOptimalStation(const Time& current) const = 0; 
};

} // namespace Helium3
