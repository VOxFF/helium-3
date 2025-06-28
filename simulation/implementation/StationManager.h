#pragma once

#include "interfaces/IStationManager.h"
#include "interfaces/IStation.h"
#include <map>
#include <utility>
#include <memory>

//#define UNIFORM_IDLE_DISTRIBUTION

namespace Helium3 {

class StationManager : public IStationManager {
public:
    virtual ~StationManager() = default;

    // --- From IStation ---
    void initialize(unsigned int stationCount) override;   
    std::vector<IStation*> stations() const override;
    IStation* getOptimalStation(const Time& current) override;

private:
    std::vector<std::shared_ptr<IStation>> m_stations;

#ifdef UNIFORM_IDLE_DISTRIBUTION
    std::map<IStation*, int> m_frequency;  //if all statiosn are not busy, pick one which was used less
#endif
};

} // namespace Helium3
