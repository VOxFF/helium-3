#pragma once
#include <chrono>


namespace Helium3 {

using Clock = std::chrono::system_clock;                // pick a clock
using Duration = std::chrono::nanoseconds;              // alias for duration
using Time = std::chrono::time_point<Clock, Duration>;  // alias for its time_point

} //end of namespace Helium3