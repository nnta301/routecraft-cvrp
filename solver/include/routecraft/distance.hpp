#pragma once

#include "routecraft/cvrp.hpp"

#include <cstddef>
#include <optional>
#include <vector>

namespace routecraft {

// Trả nullopt nếu khoảng cách không hữu hạn hoặc không biểu diễn được bằng Distance.
std::optional<Distance> checked_euclidean_distance(
    const Node& first, const Node& second) noexcept;

// Ma trận khoảng cách n x n theo chỉ số nội bộ, dựng một lần bằng
// checked_euclidean_distance. Ném std::overflow_error nếu có cạnh không biểu diễn được.
class DistanceMatrix {
public:
    explicit DistanceMatrix(const Instance& instance);

    Distance operator()(NodeIndex first, NodeIndex second) const {
        return values_[first * size_ + second];
    }

    std::size_t size() const noexcept { return size_; }

private:
    std::size_t size_{};
    std::vector<Distance> values_;
};

Demand route_load(const Instance& instance, const Route& route);

Distance route_distance(const Instance& instance, const Route& route);

Distance solution_distance(const Instance& instance, const Solution& solution);

}
