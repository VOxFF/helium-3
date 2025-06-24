#pragma once

#include "interfaces/ITrucksManager.h"
#include "interfaces/ITruck.h"

#include <memory>
#include <functional>

namespace Helium3 {

class TrucksManager : public ITrucksManager {
public:
    /**
     * @brief Constructs a TrucksManager with an optional custom factory.
     * 
     * By default, `MiningTruck` instances are created during initialization.
     * If a factory is provided, it is used to create the trucks instead.
     *
     * @param factory Optional function that creates a truck for a given index.
     */
    TrucksManager(const TruckFactory& factory = {}) : m_truckFactory(factory) {}
    virtual ~TrucksManager() = default;

    // --- From ITrucksManager ---
    void initialize(unsigned int truckCount) override;
    std::vector<ITruck*> trucks() const override;

    // --- More truck-managing API will appear here ---

private:
    TruckFactory m_truckFactory;
    std::vector<std::shared_ptr<ITruck>> m_trucks;
};


} // namespace Helium3
