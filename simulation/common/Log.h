
#pragma once

#include "common/Time.h"
#include "common/State.h"

#include <utility>
#include <vector>

namespace Helium3 {

class Log {
public:
    virtual ~Log() = default;

    void add(const Time& t, const State& s) {
        m_events.emplace_back(t,s);
    }

private: 
    using LogEvent = std::pair<Time, State>;
    std::vector<LogEvent> m_events;
    
};

}   //end of namespace Helium3