#pragma once

#include "routecraft/cvrp.hpp"

#include <string>

namespace routecraft {

struct SolutionTimings {
    double construction_time_ms{};
    double validation_time_ms{};
};

// Phương pháp đã sinh nghiệm. Tham số lambda/mu/nu chỉ được xuất khi method là "cw".
struct SolutionMethod {
    std::string name{"nn"};
    double lambda{1.0};
    double mu{0.0};
    double nu{0.0};
};

std::string serialize_solution_json(
    const Instance& instance,
    const Solution& solution,
    const ValidationResult& validation,
    const SolutionTimings& timings,
    const SolutionMethod& method = {});

std::string serialize_solution_txt(
    const Instance& instance,
    const Solution& solution,
    const ValidationResult& validation,
    const SolutionTimings& timings,
    const SolutionMethod& method = {});

}
