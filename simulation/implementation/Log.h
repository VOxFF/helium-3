
#pragma once

#include "interfaces/ILog.h"

#include <unordered_map>

namespace Helium3 {

class Log : public ILog {
public:
    virtual ~Log() = default;

    // --- From ILog ---
    void add(const Event& e, TaskState taskState) override;
    const Events& events(TaskState taskState = TaskState::Complete) const override;

    std::vector<std::string> loggedIds() const override;
    const MachineSummary& summary(const std::string& machineId) const override;

private: 
    Events m_events;
    Events m_unfinished;
    std::unordered_map<std::string, MachineSummary> m_stats;
};

}   //end of namespace Helium3
