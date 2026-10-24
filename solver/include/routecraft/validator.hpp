#pragma once

#include "routecraft/cvrp.hpp"

namespace routecraft {

ValidationResult validate_solution(const Instance& instance, const Solution& solution);

}
