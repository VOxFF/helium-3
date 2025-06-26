
#pragma once

#include "interfaces/ILog.h"

#include <unordered_map>

namespace Helium3 {

class Log : public ILog {
public:
    enum VerboseLevel {
        Console, 
        Silent
    };

    explicit Log(VerboseLevel level = Console) : m_level(level) {}
    virtual ~Log() = default;

    // --- From ITruck ---
    void add(const Event& e, TaskState taskState) override;
    const MachineSummary& summary(const std::string& machienId) const override;

private: 
    VerboseLevel m_level;
    std::unordered_map<std::string, MachineSummary> m_stats;
};

}   //end of namespace Helium3