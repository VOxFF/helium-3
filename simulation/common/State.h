/**
 * @file   State.hpp
 * @brief  Generic finite-state token used by any @c IMachine implementation
 *         (e.g., Mining Truck, Unload Station).
 *
 * A <em>State</em> is an immutable value object identified by an unsigned
 * integer and a human-readable name.  
 * Equality and inequality operators compare states **by id only**.
 *
 * Optionally, a state can expose a transition functor that yields the logical
 * “next” state in a deterministic sequence (e.g., @c Idle → @c MovingToSite → @c Mining).
 */

#pragma once
#include <string>
#include <functional>

namespace Helium3 {


/**
 * @class State
 * @brief Lightweight value object representing a machine state.
 *
 * Example usage:
 * @code
 * const State Idle   {0, "Idle"};
 *
 * if (truck.GetState() == Idle)
 *     ...
 * @endcode
 */
class State {
public:
    
    /**
     * @brief Construct a state.
     * @param id   Numeric identifier (must be unique within a state set).
     * @param name Human-readable name (for logging / debugging).
     */
    explicit State(unsigned int id, const std::string& name, std::function<State()> nextFn = {})
        : m_id(id), m_name(name), m_nextFn(nextFn) {}

    
    virtual ~State() = default;

    /// Compare two states by their numeric id.
    inline bool operator==(const State& rhs) const { return m_id == rhs.m_id; }
    inline bool operator!=(const State& rhs) const { return m_id != rhs.m_id; }

    /**
     * @brief Get the state’s display name.
     * @return Constant reference to the name string.
     */
    inline const std::string& name() const { return m_name; }

    /**
     * @brief Return the next state in a predefined sequence. Return Idle state if fucntin is not specified. 
     * @return The next @c State value.
     */
    inline State next() const { return m_nextFn ? m_nextFn() : State(0, "Idle"); }

protected:
    unsigned int m_id;               
    std::string  m_name;             
    std::function<State()> m_nextFn; 
};

} //end of namespace Helium3
