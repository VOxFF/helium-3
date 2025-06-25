#pragma once

#include "common/Event.h"

namespace Helium3 {

class ILog {
public:
    virtual ~ILog() = default;

    virtual void add(const Event& e) = 0;
    
};

}   //end of namespace Helium3