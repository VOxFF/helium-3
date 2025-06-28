#pragma once

#include "common/Event.h"

#include <map>

namespace Helium3 {

/**
 * @brief Interface for recording and querying simulation events and summaries.
 */
class ILog {
public:
    virtual ~ILog() = default;

    /**
     * @brief Indicates whether an event is completed or still in progress.
     */
    enum class TaskState {
        Complete,   ///< Event finished normally
        Unfinished  ///< Event did not complete before simulation end
    };

    /**
     * @brief Summary of occurrences and total duration for a single state.
     */
    struct StateSummary {
        size_t   occurrences = 0;    ///< Number of times this state occurred
        Duration totalDuration{};    ///< Cumulative time spent in this state
    };

    /**
     * @brief Aggregate summaries for each state of a particular machine.
     */
    struct MachineSummary {
        std::map<std::string, StateSummary> complete;    ///< Completed-state summaries
        std::map<std::string, StateSummary> unfinished;  ///< Unfinished-state summaries
    };

    /**
     * @brief Record an event into the log.
     * 
     * @param e          The Event object to add.
     * @param taskState  Whether the event completed or was left unfinished.
     */
    virtual void add(const Event& e, 
                     TaskState taskState = TaskState::Complete) = 0;

    /**
     * @brief Retrieve all logged events filtered by completion state.
     * 
     * @param taskState  Which set of events to return.
     * @return const Events&  Reference to the container of matching events.
     */
    virtual const Events& events(TaskState taskState = TaskState::Complete) const = 0;

    /**
     * @brief Get the list of machine IDs that have been logged.
     * 
     * @return std::vector<std::string>&  Vector of unique machine identifiers.
     */
    virtual std::vector<std::string> loggedIds() const = 0;

    /**
     * @brief Retrieve the summary for a specific machine.
     * 
     * @param machineId  The unique identifier of the machine.
     * @return const MachineSummary&  Summary of states for that machine.
     */
    virtual const MachineSummary& summary(const std::string& machineId) const = 0;
};


}   //end of namespace Helium3
