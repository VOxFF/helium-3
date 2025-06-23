/**
 * @file   ITruck.hpp
 * @brief  Interface that marks a machine as a *truck* in the Helium-3
 *         simulation.
 *
 * `ITruck` currently adds **additional virtual functions** on top of the
 * generic @ref Helium3::IMachine contract that represent major
 * lifecycle transitions in truck behavior.
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

#pragma once

#include "interfaces/IMachine.h"
#include "common/Event.h"

namespace Helium3 {

/**
 * @class ITruck
 * @brief Interface for all truck-type machines in the simulation.
 *
 * Defines truck-specific state transitions as event-generating actions.
 * These simulate the behavior of an autonomous vehicle mining and
 * unloading Helium-3.
 */
class ITruck : public IMachine {
public:
    virtual ~ITruck() = default;

    /// Begin mining at a site.
    virtual Event startMining() = 0;

    /// Begin travel from site to station.
    virtual Event driveToStation() = 0;

    /// Arrive at the station.
    virtual Event checkinAtStation() = 0;

    /// Wait in queue to unload.
    virtual Event startWaiting() = 0;

    /// Unload Helium-3 at station.
    virtual Event unload() = 0;

    /// Return to the mining site.
    virtual Event driveToMining() = 0;

    // --- future truck-specific API will appear here ---
};

} // namespace Helium3
