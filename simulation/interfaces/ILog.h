#pragma once

#include "common/Event.h"

#include <map>

namespace Helium3 {

class ILog {
public:
    virtual ~ILog() = default;

    enum class TaskState {
        Complete,
        Unfinished
    };

    struct StateSummary {
        size_t occurrences = 0;
        Duration totalDuration{};
    };
    struct MachineSummary {
        std::map<std::string, StateSummary> complete;
        std::map<std::string, StateSummary> unfinished;
    };

    virtual void add(const Event& e, TaskState taskState = TaskState::Complete) = 0;
    virtual const MachineSummary& summary(const std::string& machineId) const = 0;
};

}   //end of namespace Helium3
