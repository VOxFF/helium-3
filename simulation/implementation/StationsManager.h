#pragma once

#include "interfaces/IStationsManager.h"
#include "interfaces/IStation.h"
#include <queue>
#include <utility>
#include <memory>

namespace Helium3 {

class StationsManager : public IStationsManager {
public:
    virtual ~StationsManager() = default;

    // --- From IStation ---
    void initialize(unsigned int stationCount) override;   
    std::vector<IStation*> stations() const override;
    IStation* getOptimalStation() const override;

private:
    std::vector<std::shared_ptr<IStation>> m_stations;

    /*
     * Stations will be prioritized by:
     *  1. Least number of trucks in queue (lower load)
     *  2. Fewer operations performed (to balance usage)
     */
    using RankedStation = std::pair<IStation*, int>;

    struct StationComparator {
        bool operator()(const RankedStation& a, const RankedStation& b) const {
            auto [stationA, opsA] = a;
            auto [stationB, opsB] = b;

            size_t loadA = stationA->count();
            size_t loadB = stationB->count();

            if (loadA != loadB)
                return loadA > loadB; // Prefer lower load
            return opsA > opsB;       // Prefer fewer total operations
        }
    };

    std::priority_queue<RankedStation, std::vector<RankedStation>, StationComparator> m_queue;
};

} // namespace Helium3
