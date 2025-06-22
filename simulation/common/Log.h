
#pragma once

#include "simulation/common/Time.h"
#include "simulation/common/State.h"

#include <utility>
#include <vector>


namespace Helium3 {

class Log {
public:
    virtual ~Log() = default;

    void add(const Time& t, const State& s) {
        m_events.push_back({t,s});
    }

private: 
    using Event = std::pair<Time, State>;
    std::vector<Event> m_events;
    
};

}   //end of namespace Helium3