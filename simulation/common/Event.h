/**
 * @file   Event.hpp
 * @brief  Time-boxed action scheduled for a truck (used in the event queue).
 */

#pragma once
#include "common/State.h"
#include "common/Time.h"

#include <functional>
#include <optional>

namespace Helium3 {

struct Event;

using EventCallback = std::function<std::optional<Event>(void)>; 

/**
 * @struct Event
 * @brief  Represents “truck @p truck will do X starting at @p start for
 *         @p duration”.
 *
 * Events are sorted in a `std::priority_queue<Event, …, std::greater<Event>>`
 * using @c operator> so that *earlier* start times—and, when equal, *shorter*
 * durations—bubble to the top.
 */
struct Event {
    std::string machineId;      ///< ID of the machine that triggered the event
    State       state = -1;     ///< State the machine transitioned into
    Time        start = {};     ///< Absolute simulation time when the event begins
    Duration    duration = {};  ///< Duration of the event
    EventCallback onExpiration; ///< Triggered when the event is pulled from the queue (i.e., expires)


    /// Construct an event.
    Event() = default;
    Event(const std::string& machineId, State state,  Time start, Duration duration, EventCallback onExpiration = {})
        : machineId(machineId), state(std::move(state)), start(std::move(start)), duration(std::move(duration)), onExpiration(onExpiration) {}
    
    virtual ~Event() = default;

    inline Time end() const { return start + duration; }

    /**
     * @brief Strict weak ordering for priority queues.
     *
     * Earlier start wins; if starts are equal, shorter duration wins.
     */
    bool operator>(const Event& rhs) const {
        return end() > rhs.end();
    }
};

} // namespace Helium3

