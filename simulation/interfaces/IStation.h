
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
 * 
 * Concrete subclasses (e.g., `UnloadStation`, future `ChargeStation`) decide what 
 * to do with the queued trucks: unload payload, recharge batteries, perform maintenance, and so on.
 */
class IStation : public IMachine {
public:
    virtual ~IStation() = default;

    virtual size_t count() const = 0;

    /**
     * @brief Enqueues a truck at this station and generates subsequent events.
     *
     * Adds the given truck to the station’s processing.
     *
     * @param truck Pointer to the truck to enqueue.
     * @return Events A list of events triggered by enqueuing the truck.
     */
    virtual Events enqueue(ITruck* truck) = 0;

    /**
     * @brief Removes and returns the next batch of scheduled events.
     *
     * Fineshe the processing of curent truck and continues with earliest‐enqued.
     *
     * @return Events A container of Events occurend druign the opration.
     */
    virtual Events dequeue() = 0;
    
    /**
     * @brief Returns the remaining duration of the current processing chain relative to the given time.
     *
     * Calculates how much time is left in the station's ongoing processing sequence
     * when compared against the specified `currentTime`.
     *
     * @param currentTime The reference time against which to measure remaining processing.
     * @return Duration The time remaining until the current processing chain completes.
     */
    virtual Duration getWaitTime(Time currentTime) const = 0;
};

} // namespace Helium3