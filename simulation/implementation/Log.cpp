#include "Log.h"

#include <string>
#include <iostream>
#include <iomanip>  
#include <ctime>    

namespace Helium3 {
namespace {

const ILog::MachineSummary UNDEFINED{
    { {"Undefined", {}} },   // complette
    { {"Undefined", {}} }    // unfinshed
};

} //end of anonimous namespace 

void Log::add(const Event& e, TaskState taskState) 
{
    if (m_level == Console) {
        std::cout << (taskState == TaskState::Unfinished ? "X " : "  ");
        std::cout 
            << std::left
            << std::setw(10) << (e.machineId + ":")             // "Truck_0:"
            << std::setw(14) << ("(" + e.name + ")")            // "(Unloading)"
            << std::setw(8) << toString(e.start) << " -> "      // "19:21:40"
            << std::setw(8) << toString(e.end())  << " = "      // "19:26:40"
            << std::setw(6) << toString(e.duration)  << "  "    // "5m", "30m"
            << e.message + (taskState == TaskState::Unfinished ? " -- Unfinished" : "")
            << std::endl;
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