#include "routecraft/distance.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

namespace routecraft {

std::optional<Distance> checked_euclidean_distance(
    const Node& first, const Node& second) noexcept {
    const double distance = std::hypot(first.x - second.x, first.y - second.y);
    // double làm tròn INT64_MAX lên 2^63; phải loại cả giá trị tại biên này.
    if (!std::isfinite(distance) ||
        distance >= static_cast<double>(std::numeric_limits<Distance>::max())) {
        return std::nullopt;
    }
    return static_cast<Distance>(std::llround(distance));
}

namespace {

Distance require_edge_distance(const Node& first, const Node& second) {
    const std::optional<Distance> distance =
        checked_euclidean_distance(first, second);
    if (!distance.has_value()) {
        throw std::overflow_error(
            "Khoảng cách giữa node " + std::to_string(first.internal_index) +
            " và node " + std::to_string(second.internal_index) +
            " không hữu hạn hoặc vượt miền int64");
    }
    return *distance;
}

Distance checked_sum(Distance total, Distance addend) {
    if (addend > std::numeric_limits<Distance>::max() - total) {
        throw std::overflow_error("Tổng khoảng cách vượt miền int64");
    }
    return total + addend;
}

}

DistanceMatrix::DistanceMatrix(const Instance& instance)
    : size_(instance.nodes.size()) {
    if (size_ != 0 && size_ > std::numeric_limits<std::size_t>::max() / size_) {
        throw std::overflow_error("Số node quá lớn để dựng ma trận khoảng cách");
    }
    values_.assign(size_ * size_, 0);

    for (NodeIndex i = 0; i < size_; ++i) {
        const Node& first = node_by_internal_index(instance, i);
        for (NodeIndex j = i + 1; j < size_; ++j) {
            const Distance distance =
                require_edge_distance(first, node_by_internal_index(instance, j));
            values_[i * size_ + j] = distance;
            values_[j * size_ + i] = distance;
        }
    }
}

Demand route_load(const Instance& instance, const Route& route) {
    Demand load = 0;

    for (const NodeIndex node_index : route.stops) {
        if (node_index == 0) {
            continue;
        }

        load += node_by_internal_index(instance, node_index).demand;
    }

    return load;
}

Distance route_distance(const Instance& instance, const Route& route) {
    if (route.stops.size() < 2) {
        return 0;
    }

    Distance total_distance = 0;

    for (std::size_t position = 1; position < route.stops.size(); ++position) {
        const Node& previous =
            node_by_internal_index(instance, route.stops[position - 1]);
        const Node& current =
            node_by_internal_index(instance, route.stops[position]);

        total_distance = checked_sum(
            total_distance, require_edge_distance(previous, current));
    }

    return total_distance;
}

Distance solution_distance(const Instance& instance, const Solution& solution) {
    Distance total_distance = 0;

    for (const Route& route : solution.routes) {
        total_distance =
            checked_sum(total_distance, route_distance(instance, route));
    }

    return total_distance;
}

}
