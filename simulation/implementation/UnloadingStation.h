#pragma once

#include "interfaces/IStation.h"
#include "common/Log.h"

#include <string>
#include <queue>

namespace Helium3 {

/**
 * @class UnloadingStation
 * @brief A concrete implementation of an unloading station that can process
 *        one truck at a time and queue others.
 */
class UnloadingStation : public IStation {
public:
    explicit UnloadingStation(const std::string& id)
        : m_id(id), m_state(Idle), m_unloading(nullptr) {}

    ~UnloadingStation() override = default;

    enum StateID {
        Idle = -1,     ///< Initial or reset state
        Unloading,     ///< Actively unloading a truck
        Waiting,       ///< Has a queue but not currently unloading
        StatesCount    ///< Number of defined states (excluding Idle)
    };

    static const std::string& stateName(StateID id);

    // --- From IMachine ---
    const std::string& id() const override { return m_id; }
    const State& getState() const override { return m_state; }
    const Log& log() const override { return m_log; }

    // --- From IStation ---
    size_t count() const override { return m_queue.size(); }

    /**
     * @brief  Accept a truck into the station’s processing queue.
     *
     * @param truck  Reference to the arriving vehicle.  Ownership is *not*
     *               transferred; the simulation still manages the truck’s
     *               lifetime.
     * @return An Event if processing starts immediately, or an empty Event if queued.
     */
    Event enqueue(ITruck* truck) override;

    /**
     * @brief  Advance to the next truck in the queue.
     *
     * @return An Event to resume the next truck, or empty if queue is empty.
     */
    Event dequeue() override;

protected: 
    Event startUnloading(ITruck* truck);

private:
    std::string m_id;
    State m_state;
    Log m_log;

    std::queue<ITruck*> m_queue; ///< Queue of waiting trucks
    ITruck* m_unloading;         ///< Currently unloading truck, or nullptr
};

} // namespace Helium3
