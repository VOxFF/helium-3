#pragma once

#include <vector>

namespace Helium3 {

class ITruck;

/**
 * @interface ITrucksManager
 * @brief Manages all truck entities in the simulation.
 *
 * This interface is responsible for initializing and providing access to trucks.
 * It allows the simulation to query and interact with trucks without owning them directly.
 *
 * More truck-managing API will appear here as the simulation grows in complexity.
 */
class ITrucksManager {
public:
    virtual ~ITrucksManager() = default;

    /**
     * @brief Initialize the trucks manager with a specific number of trucks.
     * @param truckCount The number of trucks to initialize.
     */
    virtual void initialize(unsigned int truckCount) = 0;

    /**
     * @brief Get a list of all managed trucks.
     * @return A vector of pointers to ITruck instances.
     */
    virtual std::vector<ITruck*> trucks() const = 0;

    // --- More truck-managing API will appear here ---
};

} // namespace Helium3
