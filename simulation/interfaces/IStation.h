/**
 * @file   IStation.hpp
 * @brief  Interface for a fixed ground facility that receives trucks.
 *
 * A *station* sits at a mining site or processing hub and accepts arriving
 * trucks into an internal queue.  Concrete subclasses (e.g., `UnloadStation`,
 * `ChargeStation`) decide what to do with the queued trucks: unload payload,
 * recharge batteries, perform maintenance, and so on.
 *
 */

#pragma once
#include "interfaces/IMachine.h"
#include "common/Event.h"
#include "common/Time.h"  

namespace Helium3 {

class ITruck;

/**
 * @class IStation
 * @brief Abstract base for immobile facilities that service trucks.
 *
 * Stations should also participate in the global update loop (i.e., are
 * energised machines with state/logging).
 */
class IStation : public IMachine {
public:
    virtual ~IStation() = default;

    virtual size_t count() const = 0;

    /**
     * @brief  Accept a truck into the station's processing queue.
     *
     * @param truck  Reference to the arriving vehicle.  Ownership is *not*
     *               transferred; the simulation still manages the truck's
     *               lifetime.
     */
    virtual Events enqueue(ITruck* truck) = 0;
    virtual Events dequeue() = 0;
    
    virtual Duration getWaitTime(Time currentTime) const = 0;
};

} // namespace Helium3