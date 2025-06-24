#pragma once

#include "interfaces/ITrucksManager.h"

#include <memory>

namespace Helium3 {

class TrucksManager : public ITrucksManager {
public:
    virtual ~TrucksManager() = default;

    // --- From ITrucksManager ---
    void initialize(unsigned int truckCount) override;
    std::vector<ITruck*> trucks() const override;

    // --- More truck-managing API will appear here ---

private:
    std::vector<std::shared_ptr<ITruck>> m_trucks;
};

} // namespace Helium3
