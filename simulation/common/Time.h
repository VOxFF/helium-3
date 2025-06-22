#pragma once
#include <chrono>


namespace Helium3 {

using Clock = std::chrono::system_clock;        // pick a clock
using Time  = Clock::time_point;                // alias for its time_point
using Duration = std::chrono::duration<double>; // alias for duration

} //end of namespace Helium3