#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace routecraft {

using NodeIndex = std::size_t;
using Demand = std::int64_t;
using Distance = std::int64_t;

struct Node {
    NodeIndex internal_index{};
    std::string original_id;
    double x{};
    double y{};
    Demand demand{};
    bool is_depot{false};
};

struct Instance {
    std::string name;
    Demand capacity{};
    std::vector<Node> nodes;
};

struct Route {
    std::vector<NodeIndex> stops;
};

struct Solution {
    std::vector<Route> routes;
};

struct ValidationResult {
    bool feasible{false};
    std::vector<std::string> errors;
    std::vector<Demand> route_loads;
    std::vector<Distance> route_distances;
    Distance total_distance{};
};

}