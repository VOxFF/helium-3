#include "TrucksManager.h"
#include "MiningTruck.h"

#include <string>
#include <cassert>

namespace Helium3 {

namespace {

} 

void TrucksManager::initialize(unsigned int truckCount) 
{
    assert(m_trucks.empty());
    for(auto i = 0; i < truckCount; i++)
        m_trucks.emplace_back(m_truckFactory ? m_truckFactory(m_stationManager, i) : std::make_shared<MiningTruck>(m_stationManager, MiningTruck::namePrefix() + std::to_string(i)));
}

std::vector<ITruck*> TrucksManager::trucks() const 
{
    std::vector<ITruck*> ptrs;
    ptrs.reserve(m_trucks.size());
    std::transform( m_trucks.begin(), m_trucks.end(), std::back_inserter(ptrs),
        [](const std::shared_ptr<ITruck>& ptr) { return ptr.get();});

    return ptrs;
}

} // namespace Helium3