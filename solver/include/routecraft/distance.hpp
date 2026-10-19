#pragma once

#include "routecraft/cvrp.hpp"

namespace routecraft {

Distance euclidean_distance(const Node& first, const Node& second) noexcept;

Demand route_load(const Instance& instance, const Route& route);

Distance route_distance(const Instance& instance, const Route& route);

Distance solution_distance(const Instance& instance, const Solution& solution);

}