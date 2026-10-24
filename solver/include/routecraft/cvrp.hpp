#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
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

const Node& node_by_internal_index(
    const Instance& instance,
    NodeIndex internal_index);

struct Route {
    std::vector<NodeIndex> stops;
};

struct Solution {
    std::vector<Route> routes;
    std::vector<Demand> route_loads;
    std::vector<Distance> route_distances;
    std::optional<Distance> total_distance;
};

struct ValidationResult {
    bool feasible{false};
    std::vector<std::string> errors;
    std::vector<Demand> route_loads;
    std::vector<Distance> route_distances;
    Distance total_distance{};
};

}
