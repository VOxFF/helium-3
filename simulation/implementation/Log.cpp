#include "Log.h"

namespace Helium3 {
namespace {

const ILog::MachineSummary UNDEFINED{
    { {"Undefined", {}} },   // complete
    { {"Undefined", {}} }    // unfinished
};

} //end of anonimous namespace 

void Log::add(const Event& e, TaskState taskState) 
{
    auto& destination = taskState == TaskState::Complete ? m_events : m_unfinished;
    destination.push_back(e);

    auto& machineStat = m_stats[e.machineId];
    auto& collection = taskState == TaskState::Complete ? machineStat.complete : machineStat.unfinished;
    auto& record = collection[e.name];

    record.occurrences++;
    record.totalDuration += e.duration;
}

const Events& Log::events(TaskState taskState) const 
{
    return taskState == TaskState::Complete ? m_events : m_unfinished;
}

std::vector<std::string> Log::loggedIds() const 
{
    std::vector<std::string> ids;
    ids.reserve(m_stats.size());
    for (const auto& [id, _] : m_stats) {
        ids.push_back(id);
    }
    std::sort(ids.rbegin(), ids.rend());
    return ids;
}

const ILog::MachineSummary& Log::summary(const std::string& machineId) const 
{
    if(auto it = m_stats.find(machineId); it != m_stats.end())
        return it->second;

    return UNDEFINED;
}

}   //end of namespace Helium3
