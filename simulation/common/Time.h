#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>  

namespace Helium3 {

using Clock = std::chrono::system_clock;                // pick a clock
using Duration = std::chrono::nanoseconds;              // alias for duration
using Time = std::chrono::time_point<Clock, Duration>;  // alias for its time_point

inline std::string toString(const Time& t) 
{
    auto timeT = std::chrono::system_clock::to_time_t(t);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&timeT), "%T"); // "HH:MM:SS"
    return oss.str();
}

inline std::string toString(const Duration& d) 
{
    using namespace std::chrono;

    auto total_seconds = duration_cast<seconds>(d).count();
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    std::ostringstream oss;
    if (hours > 0)
        oss << hours << "h ";
    if (minutes > 0)
        oss << minutes << "m ";
    if (seconds > 0 || (hours == 0 && minutes == 0)) 
        oss << seconds << "s";

    return oss.str();
}

} //end of namespace Helium3