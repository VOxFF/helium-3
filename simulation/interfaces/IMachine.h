
/**
 * @file    IMachine.hpp
 * @brief   Pure-virtual interface for every powered unit in the Helium-3
 *          lunar-mining simulation (trucks, unload stations, chargers, etc.).
 *
 * A concrete “machine” must expose four core capabilities:
 *   • A unique textual identifier ( @ref id() )  
 *   • A way to reset itself to its initial simulation state ( @ref reset() )  
 *   • A finite-state view of its behaviour ( @ref state() / @ref update() )  
 *   • Read-only access to its internal event/metric log ( @ref log() )
 */

#pragma once
#include <string>

namespace Helium3 {

class State;   
class Time;    
class Log;     

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
     * @brief Energise the device.
     *
     * Transitions the internal finite-state machine from @c Off to @c Idle.  
     * 
     * Further implementations should perform any start-up checks (e.g., battery level,
     * safety interlocks) before declaring the machine ready.
     */
    virtual void powerOn() = 0;

    /**
     * @brief De-energise the device.
     *
     * Forces an immediate transition to the @c Off state from whatever state the
     * machine is currently in.  
     * 
     * Concrete classes must cancel or gracefully halt
     * ongoing operations and ensure the device is left in a safe condition.
     */
    virtual void powerOff() = 0;

    /**
     * @brief Reset all internal counters, timers and state variables so that
     *        the next call to @ref update() behaves as if the object had just
     *        been constructed.
     */
    virtual void reset() = 0;

    /**
     * @brief Query the current finite state.
     * @return A constant reference to the machine’s @c State object.
     */
    virtual const State& state() const = 0;

    /**
     * @brief Advance the machine by one simulation time-step.
     *
     * @param now Absolute simulation time at which the update is executed.
     * @return    The machine’s new @c State after all internal transitions are
     *            processed.
     */
    virtual const State& update(const Time& now) = 0;

    /**
     * @brief Access the machine-local log for telemetry or debugging output.
     *
     * Returned as a const reference so external callers can read entries
     * without mutating the log’s contents or structure.
     */
    virtual const Log& log() const = 0;
};

} // namespace Helium3
