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
    Time        start;          ///< absolute simulation time the event begins
    Duration    duration;       ///< how long the event lasts
    EventCallback onExpiration; ///< trigger when event is pulled out of queue (i.e. expired)

    /// Construct an event.
    Event(Time start, Duration duration, EventCallback onExpiration = {})
        : start(std::move(start)), duration(std::move(duration)), onExpiration(onExpiration) {}

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

