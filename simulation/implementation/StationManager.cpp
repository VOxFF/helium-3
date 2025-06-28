#include "StationManager.h"
#include "UnloadingStation.h"

#include <string>
#include <cassert>
#include <limits>

namespace Helium3 {

namespace {
const std::string PREFIX("Station_");
} 

void StationManager::initialize(unsigned int stationCount)
{
    assert(m_stations.empty());

    for (auto i = 0; i < stationCount; i++) 
    {   
        auto station = std::make_shared<UnloadingStation>(PREFIX + std::to_string(i));
#ifdef UNIFORM_IDLE_DISTRIBUTION        
        m_frequency[station.get()] = 0;
#endif        
        m_stations.push_back(std::move(station));
        
    }
}

std::vector<IStation*> StationManager::stations() const 
{
    std::vector<IStation*> ptrs;
    ptrs.reserve(m_stations.size());
    std::transform( m_stations.begin(), m_stations.end(), std::back_inserter(ptrs),
        [](const std::shared_ptr<IStation>& ptr) { return ptr.get();});

    return ptrs;
}


IStation* StationManager::getOptimalStation(const Time& currentTime) 
{
    assert(!m_stations.empty());
    
    IStation* bestStation = nullptr;

#ifdef UNIFORM_IDLE_DISTRIBUTION  
    //Pick least used of not busy
    std::vector<IStation*> idle;
    for (const auto& station : m_stations) 
    {
        if (station->count() == 0) 
            idle.push_back(station.get());
    }
    if(!idle.empty()){
        auto minFreq = std::numeric_limits<int>::max();
        for(auto station : idle) 
        {
            if(m_frequency[station] < minFreq)
            {
                bestStation = station;
                minFreq = m_frequency[station];
            }
        }
        m_frequency[bestStation]++;
        return bestStation;
    } 
#else
    //Pick first not busy
    for (const auto& station : m_stations) 
    {
        if (station->count() == 0) 
            return station.get();
    }
#endif

    
    // All stations are busy, find the one with shortest wait time
    auto minWaitTime = Duration::max();
    for (const auto& station : m_stations) {
        Duration waitTime = station->getWaitTime(currentTime);
        if (waitTime < minWaitTime) {
            minWaitTime = waitTime;
            bestStation = station.get();
        }
    }
    
    return bestStation;
}

} // namespace Helium3