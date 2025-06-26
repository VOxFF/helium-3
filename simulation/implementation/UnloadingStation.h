#pragma once

#include "interfaces/IStation.h"

#include <string>
#include <queue>
#include <functional>

namespace Helium3 {

/**
 * @class UnloadingStation
 * @brief A concrete implementation of an unloading station that can process
 *        one truck at a time and queue others.
 */
class UnloadingStation : public IStation {
public:
    using Callback = std::function<void()>;

    /**
     * @brief Constructs an unloading station with a unique identifier.
     * 
     * @param id        Unique station identifier.
     * @param callback  Optional callback invoked when the station's internal state 
     *                  changes and priority ordering needs to be updated externally 
     *                  (e.g., in a stations manager).
     */
    explicit UnloadingStation(const std::string& id, Callback callback = {})
        : m_id(id), m_state(Idle), m_unloading(nullptr), m_currentUnloadingEnd(Time{}), m_callback(callback) {}

    ~UnloadingStation() override = default;

    enum StateID {
        Idle = -1,     ///< Initial or reset state
        Unloading,     ///< Actively unloading a truck
        Waiting,       ///< Not uloading and no trucks in queue, waiting for Truck arrival
        StatesCount    ///< Number of defined states (excluding Idle)
    };

    static const std::string& stateName(StateID id);

    // --- From IMachine ---
    const std::string& id() const override { return m_id; }
    const State& state() const override { return m_state; }

    // --- From IStation ---
    /// @brief Considers both enqueued trucks and the one currently being processed.
    size_t count() const override { return m_queue.size() + (m_unloading ? 1 : 0); }

    /**
     * @brief  Accept a truck into the station's processing queue.
     *
     * @param truck  Reference to the arriving vehicle.  Ownership is *not*
     *               transferred; the simulation still manages the truck's
     *               lifetime.
     * @return An Event if processing starts immediately, or an empty Event if queued.
     */
    Events enqueue(ITruck* truck) override;

    /**
     * @brief  Advance to the next truck in the queue.
     *
     * @return An Event to resume the next truck, or empty if queue is empty.
     */
    Events dequeue() override;
    

    Duration getWaitTime(Time currentTime) const override;

protected: 
    Events startUnloading(ITruck* truck);

private:
    std::string m_id;
    State m_state;

    std::queue<ITruck*> m_queue; ///< Queue of waiting trucks
    ITruck* m_unloading;         ///< Currently unloading truck, or nullptr
    
  
    Time m_currentUnloadingEnd;  ///< When current truck will finish unloading

    Callback m_callback;        ///< Callback to update ordering of station prirorities
};

} // namespace Helium3