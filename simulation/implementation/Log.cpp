#include "Log.h"

#include <string>
#include <iostream>
#include <iomanip>  
#include <ctime>    

namespace Helium3 {
namespace {

const ILog::MachineSummary UNDEFINED{{"Undefined", {}},{{"Undefined", {}}}};

std::string toString(const Time& t) 
{
    auto timeT = std::chrono::system_clock::to_time_t(t);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&timeT), "%T"); // "HH:MM:SS"
    return oss.str();
}

std::string toString(const Duration& d) 
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


} //end of anonimous namespace 

void Log::add(const Event& e, TaskState taskState) 
{
    if (m_level == Console) {
        std::cout 
            << e.machineId << ":\t"
            << toString(e.start) << " -> " 
            << toString(e.end()) << " = "
            << toString(e.duration) << "\t"
            << e.message << std::endl;
    }

    // If we want to collect all the records
    //m_events.push_back(e);

    auto& machineStat = m_stats[e.machineId];
    auto& collection = taskState == TaskState::Complette ? machineStat.complette : machineStat.unfinshed;
    auto& record = collection[e.name];

    record.occurrences++;
    record.totalDuration += e.duration;
}

const ILog::MachineSummary& Log::summary(const std::string& machienId) const 
{
    if(auto it = m_stats.find(machienId); it != m_stats.end())
        return it->second;

    return UNDEFINED;
}



}   //end of namespace Helium3