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


IStation* StationManager::getOptimalStation(const Time& currentTime) const 
{
    assert(!m_stations.empty());
    
    // First, check if any station is free (not currently unloading)
    for (const auto& station : m_stations) {
        if (station->count() == 0) {
            return station.get();  // Return first available station
        }
    }
    
    // All stations are occupied, find the one with shortest wait time
    IStation* bestStation = nullptr;
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