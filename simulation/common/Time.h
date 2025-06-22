#pragma once
#include <chrono>


namespace Helium3 {

using Clock = std::chrono::system_clock;      // pick a clock
using Time  = Clock::time_point;              // alias for its time_point

} //end of namespace Helium3