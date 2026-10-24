#pragma once

#include "routecraft/cvrp.hpp"

#include <optional>

namespace routecraft {

Distance euclidean_distance(const Node& first, const Node& second) noexcept;

// Trả nullopt nếu khoảng cách không hữu hạn hoặc không biểu diễn được bằng Distance.
std::optional<Distance> checked_euclidean_distance(
    const Node& first, const Node& second) noexcept;

Demand route_load(const Instance& instance, const Route& route);

Distance route_distance(const Instance& instance, const Route& route);

Distance solution_distance(const Instance& instance, const Solution& solution);

}
