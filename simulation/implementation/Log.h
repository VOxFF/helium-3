
#pragma once

#include "interfaces/ILog.h"

#include <vector>

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
    void add(const Event& e) override;

protected: 
    std::string resolveState(const std::string& machineID, const State& state) const; 

private: 
    std::vector<Event> m_events;
    VerboseLevel m_level;
    
};

}   //end of namespace Helium3