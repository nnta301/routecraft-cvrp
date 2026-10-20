#pragma once

#include "routecraft/cvrp.hpp"

namespace routecraft {

struct NearestNeighborResult {
    Solution solution;
    double construction_time_ms{};
};

NearestNeighborResult solve_nearest_neighbor(const Instance& instance);

}
