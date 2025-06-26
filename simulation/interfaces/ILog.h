#pragma once

#include "common/Event.h"

#include <map>

namespace Helium3 {

class ILog {
public:
    virtual ~ILog() = default;

    struct StateSummary {
        size_t occurrences = 0;
        Duration totalDuration{};
        std::string message;
    };
    struct MachineSummary {
        std::string machineId;
        std::map<int, StateSummary> complette;
        std::map<int, StateSummary> unfinshed;
    };



    virtual void add(const Event& e) = 0;
    
};

}   //end of namespace Helium3