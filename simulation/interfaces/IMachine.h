#pragma once
#include <string>
#include "common/Time.h"
#include "common/State.h"


namespace Helium3 {
 
/**
 * @class IMachine
 * @brief Abstract base class for all mechanical units in the Helium-3
 *        simulation.
 *
 * Examples of concrete subclasses:
 *   • `MiningTruck`   — mobile excavator that travels, mines, unloads  
 *   • `UnloadStation` — fixed platform that queues trucks and transfers ore  
 *   • `SolarCharger`  — static device that recharges batteries, logs uptime  
 */
class IMachine {
public:
   
    virtual ~IMachine() = default;

    /**
     * @brief Return the machine’s unique ID string.
     *
     * Examples: Truck01, Station01 etc.
     */
    virtual const std::string& id() const = 0;

    /**
     * @brief Query the current finite state.
     * @return A constant reference to the machine’s @c State object.
     */
    virtual const State& state() const = 0;
};

} // namespace Helium3
