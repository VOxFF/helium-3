#pragma once
#include <string>
#include <functional>

namespace Helium3 {

/**
 * @brief Simple enumeration of machine states, intended as a starting point.
 *
 * Defines basic lifecycle states of a machine (e.g., Idle, Mining, Moving,
 * ArrivedToStation, Unloading). Ineger type can later be replaced by a richer
 * class-based state representation as the simulation’s complexity grows.
 */
using State = int;

} //end of namespace Helium3
