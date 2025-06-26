#include "Log.h"

#include <string>
#include <iostream>
#include <iomanip>  
#include <ctime>    

namespace Helium3 {
namespace {

std::string toString(const Time& t) {
    auto timeT = std::chrono::system_clock::to_time_t(t);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&timeT), "%T"); // "HH:MM:SS"
    return oss.str();
}

std::string toString(const Duration& d) {
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


} //end of anonimous namespace 

void Log::add(const Event& e) {
    if (m_level == Console) {
        std::cout 
            << e.machineId << ":\t"
            << toString(e.start) << " -> " 
            << toString(e.end()) << " = "
            << toString(e.duration) << "\t"
            << e.message << std::endl;
    }

    m_events.push_back(e);
}



}   //end of namespace Helium3