#pragma once

#include "routecraft/cvrp.hpp"

#include <string>

namespace routecraft {

struct SolutionTimings {
    double construction_time_ms{};
    double validation_time_ms{};
};

std::string serialize_solution_json(
    const Instance& instance,
    const Solution& solution,
    const ValidationResult& validation,
    const SolutionTimings& timings);

std::string serialize_solution_txt(
    const Instance& instance,
    const Solution& solution,
    const ValidationResult& validation,
    const SolutionTimings& timings);

}
