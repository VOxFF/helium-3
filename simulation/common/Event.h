/**
 * @file   Event.hpp
 * @brief  Time-boxed action scheduled for a truck (used in the event queue).
 */

#pragma once
#include "common/State.h"
#include "common/Time.h"

#include <functional>
#include <vector>
#include <optional>

namespace Helium3 {

/// Callback that returns one or more follow-up events.
using EventCallback = std::function<std::vector<struct Event>(void)>;

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
    std::string message;
    EventCallback onExpiration; ///< Triggered when the event is pulled from the queue (i.e., expires)
 


    /// Construct an event.
    Event() = default;
    Event(const std::string& machineId, State state,  Time start, Duration duration, EventCallback onExpiration = {}, const std::string& message = {})
        : machineId(machineId), state(std::move(state)), start(std::move(start)), duration(std::move(duration)), message(message) ,onExpiration(onExpiration) {}
    
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

using Events = std::vector<Event>;


// -- Utility -- 
/**
 * @brief Appends elements from b to a.
 * @tparam T Element type.
 * @param a Destination vector.
 * @param b Source vector.
 * @return Modified a.
 */
template<typename T>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

/**
 * @brief Appends an element to the vector.
 * @tparam T Element type.
 * @param a Destination vector.
 * @param item Element to append.
 * @return Modified a.
 */
template<typename T>
std::vector<T>& operator+=(std::vector<T>& a, const T& item) {
    a.push_back(item);
    return a;
}


} // namespace Helium3

