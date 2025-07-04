
#pragma once

#include "interfaces/IMachine.h"
#include "common/Event.h"

#include <memory>

namespace Helium3 {

/**
 * @class ITruck
 * @brief Interface for all truck-type machines in the simulation.
 *
 * Defines truck-specific state transitions as event-generating actions.
 * These simulate the behavior of an autonomous vehicle mining and
 * unloading Helium-3.
 * 
 * The interface exists as an **extension point** for more realistic
 * future simulations, for example:
 *   - `double capacity() const` — payload mass   
 *   - `double velocity() const` — current ground speed 
 *
 * By introducing those methods here later, concrete classes (`MiningTruck`,
 * `ServiceTruck`, etc.) inherit them automatically without requiring changes
 * to unrelated machine types.
 */
class ITruck : public IMachine {
public:
    virtual ~ITruck() = default;

    /// Begin mining at a site.
    virtual Events startMining() = 0;

    /// Begin travel from site to station.
    virtual Events driveToStation() = 0;

    /// Arrive at the station.
    virtual Events checkinAtStation() = 0;

    /// Wait in queue to unload.
    virtual Events startWaiting() = 0;

    /// Unload Helium-3 at station.
    virtual Events unload() = 0;

    /// Return to the mining site.
    virtual Events driveToMining() = 0;

    // --- future truck-specific API will appear here ---
};

/**
 * @brief Factory function used to create custom truck instances.
 * 
 * Primarily useful in testing to override the default behavior of
 * `MiningTruck` by supplying custom truck implementations.
 */
using TruckFactory = std::function<std::shared_ptr<ITruck>(int)>;

} // namespace Helium3
