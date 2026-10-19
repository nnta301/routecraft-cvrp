#include "routecraft/distance.hpp"

#include <cmath>

namespace routecraft {

Distance euclidean_distance(const Node& first, const Node& second) noexcept {
    const double delta_x = first.x - second.x;
    const double delta_y = first.y - second.y;

    return static_cast<Distance>(std::llround(std::hypot(delta_x, delta_y)));
}

Demand route_load(const Instance& instance, const Route& route) {
    Demand load = 0;

    for (const NodeIndex node_index : route.stops) {
        if (node_index == 0) {
            continue;
        }

        load += instance.nodes.at(node_index).demand;
    }

    return load;
}

Distance route_distance(const Instance& instance, const Route& route) {
    if (route.stops.size() < 2) {
        return 0;
    }

    Distance total_distance = 0;

    for (std::size_t position = 1; position < route.stops.size(); ++position) {
        const Node& previous = instance.nodes.at(route.stops[position - 1]);
        const Node& current = instance.nodes.at(route.stops[position]);

        total_distance += euclidean_distance(previous, current);
    }

    return total_distance;
}

Distance solution_distance(const Instance& instance, const Solution& solution) {
    Distance total_distance = 0;

    for (const Route& route : solution.routes) {
        total_distance += route_distance(instance, route);
    }

    return total_distance;
}

}