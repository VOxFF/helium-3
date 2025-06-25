#include "Log.h"
#include "MiningTruck.h"
#include "UnloadingStation.h"

#include <string>
#include <iostream>
#include <iomanip>  
#include <ctime>    

namespace Helium3 {
namespace {

std::string toString(const Time& t) {
    auto timeT = std::chrono::system_clock::to_time_t(t);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&timeT), "%F %T"); // e.g., "2025-06-20 13:45:30"
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
    if (hours > 0 || minutes > 0)
        oss << minutes << "m ";
    oss << seconds << "s";

    return oss.str();
}

} //end of anonimous namespace 

void Log::add(const Event& e) {
    if (m_level == Console) {
        auto msg = resolveState(e.machineId, e.state);
        auto end = e.start + e.duration;

        std::cout 
            << e.machineId << ":\t["
            << toString(e.start) << " , " 
            << toString(end) << ", "
            << toString(e.duration) << "] "
            << msg << std::endl;
    }

    m_events.push_back(e);
}

// naive implementation
std::string Log::resolveState(const std::string& machineID, const State& state) const
{
    if (machineID.starts_with("Truck")) 
        return MiningTruck::stateName(static_cast<MiningTruck::StateID>(state));
    
    if (machineID.starts_with("Station")) 
        return UnloadingStation::stateName(static_cast<UnloadingStation::StateID>(state));

    return "Unknown state";
}


}   //end of namespace Helium3