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
    for(auto i = 0; i < truckCount; i++){
        auto id = MiningTruck::namePrefix() + std::to_string(i);
        m_trucks.emplace(id, m_truckFactory ? m_truckFactory(i) : std::make_shared<MiningTruck>(id));
    }
}

std::vector<ITruck*> TrucksManager::trucks() const 
{
    std::vector<ITruck*> ptrs;
    ptrs.reserve(m_trucks.size());
    std::transform( m_trucks.begin(), m_trucks.end(), std::back_inserter(ptrs),
        [](const auto& pair) { return pair.second.get();});

    return ptrs;
}

 ITruck* TrucksManager::truck(const std::string& id) const 
 {
    if(auto it = m_trucks.find(id); it != m_trucks.end())
        return it->second.get();
    return nullptr;
 }

} // namespace Helium3