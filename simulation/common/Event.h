/**
 * @file   Event.hpp
 * @brief  Time-boxed action scheduled for a truck (used in the event queue).
 */

#pragma once
#include "common/State.h"
#include "common/Time.h"

namespace Helium3 {

class ITruck;

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
    Time        start;      ///< absolute simulation time the event begins
    Duration    duration;   ///< how long the event lasts
    ITruck&     truck;      ///< truck involved in this event (non-owning)

    /// Construct an event.
    Event(Time start, Duration duration, ITruck& truck)
        : start(std::move(start)), duration(std::move(duration)), truck(truck) {}

    virtual ~Event() = default;

    /**
     * @brief Strict weak ordering for priority queues.
     *
     * Earlier start wins; if starts are equal, shorter duration wins.
     */
    bool operator>(const Event& rhs) const {
        return (start == rhs.start)
                   ? duration > rhs.duration
                   : start > rhs.start;
    }
};

} // namespace Helium3

