#include "StationsManager.h"
#include "UnloadingStation.h"

#include <string>
#include <cassert>

namespace Helium3 {

namespace {
const std::string PREFIX("Station-");
} 

void StationsManager::initialize(unsigned int stationCount)
{
    assert(m_queue.empty() && m_stations.empty());

    for (auto i = 0; i < stationCount; i++) 
    {   
        // Not quite optimal, consider other structure than priority_queue
        auto callback = [this, i]() {
            std::vector<StationsManager::RankedStation> temp;
            auto target = m_stations[i].get();
            auto targetFound = false;

            while (!m_queue.empty() && !targetFound) {
                auto [station, op_count] = m_queue.top(); m_queue.pop();
                targetFound = station == target; 
                temp.push_back({station, targetFound ? op_count+1 : op_count});
                
            }
            for (const auto& entry : temp) 
                m_queue.push(entry);
        };
 
        auto station = std::make_shared<UnloadingStation>(PREFIX + std::to_string(i), callback);
        m_queue.emplace(station.get(), 0);     // initially zero operations
        m_stations.push_back(std::move(station));
    }
}

std::vector<IStation*> StationsManager::stations() const 
{
    std::vector<IStation*> ptrs;
    ptrs.reserve(m_stations.size());
    std::transform( m_stations.begin(), m_stations.end(), std::back_inserter(ptrs),
        [](const std::shared_ptr<IStation>& ptr) { return ptr.get();});

    return ptrs;
}

IStation* StationsManager::getOptimalStation() const 
{
    assert(!m_queue.empty());
    return m_queue.top().first;
}



} // namespace Helium3