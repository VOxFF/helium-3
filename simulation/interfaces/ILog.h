#pragma once

#include "common/Event.h"

#include <map>

namespace Helium3 {

class ILog {
public:
    virtual ~ILog() = default;

    enum class TaskState {
        Complette,
        Unfinished
    };

    struct StateSummary {
        size_t occurrences = 0;
        Duration totalDuration{};
    };
    struct MachineSummary {
        std::map<std::string, StateSummary> complette;
        std::map<std::string, StateSummary> unfinshed;
    };

    virtual void add(const Event& e, TaskState taskState = TaskState::Complette) = 0;
    virtual const MachineSummary& summary(const std::string& machienId) const = 0;
};

}   //end of namespace Helium3