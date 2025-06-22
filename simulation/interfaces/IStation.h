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
#include "simulation/interfaces/IMachine.h"

namespace Helium3 {

class ITruck;

/**
 * @class IStation
 * @brief Abstract base for immobile facilities that service trucks.
 *
 * Stations should also participate in the global update loop (i.e., are
 * energised machines with state/logging).
 */
class IStation {
public:
    virtual ~IStation() = default;

    /**
     * @brief  Accept a truck into the station’s processing queue.
     *
     * @param truck  Reference to the arriving vehicle.  Ownership is *not*
     *               transferred; the simulation still manages the truck’s
     *               lifetime.
     */
    virtual void enqueue(const ITruck& truck) = 0;
};

} // namespace Helium3
