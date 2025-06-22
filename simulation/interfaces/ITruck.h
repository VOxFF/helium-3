/**
 * @file   ITruck.hpp
 * @brief  Interface that marks a machine as a *truck* in the Helium-3
 *         simulation.
 *
 * `ITruck` currently adds **no additional pure-virtual functions** on top of
 * the generic @ref Helium3::IMachine contract; a mining truck behaves like any
 * other powered unit (it can be powered on/off, updated every tick, queried
 * for state, and provide a log).
 *
 * The type nonetheless exists as an **extension point** for future more realistic
 * simulations like:
 *   - `double capacity() const` — payload mass   
 *   - `double velocity() const` — current ground speed 
  *
 * By introducing those methods here later, concrete classes (`MiningTruck`,
 * `ServiceTruck`, etc.) inherit them automatically without changing unrelated
 * machine classes.
 */

#pragma once
#include "interfaces/IMachine.h"
#include "common/Event.h"

namespace Helium3 {

/**
 * @class ITruck
 * @brief Tag-interface for all vehicle-type machines.
 *
 * At present the interface is empty because the simulation requirements are
 * satisfied by the base @ref IMachine API alone.  New truck-specific queries
 * or commands can be appended without disturbing non-truck code.
 */
class ITruck : public IMachine {
public:
    virtual ~ITruck() = default;

    
    virtual Event startMining() = 0;
    virtual Event driveToStation() = 0;
    virtual Event checkinAtStation() = 0;
    virtual Event startWaiting() = 0;
    virtual Event unload() = 0;
    virtual Event driveToMining() = 0;


    // --- future truck-specific API will appear here ---
};

} // namespace Helium3
