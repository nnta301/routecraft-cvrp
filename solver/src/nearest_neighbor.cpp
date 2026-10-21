#include "routecraft/nearest_neighbor.hpp"

#include "routecraft/distance.hpp"

#include <chrono>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace routecraft {
namespace {

struct Candidate {
    NodeIndex node_index{};
    Distance distance{};
};

std::optional<Candidate> find_nearest_feasible_customer(
    const Instance& instance,
    const std::vector<bool>& served,
    NodeIndex current_node,
    Demand current_load) {
    std::optional<Candidate> best_candidate;
    const Node& current = node_by_internal_index(instance, current_node);

    for (NodeIndex candidate_index = 1; candidate_index < instance.nodes.size(); ++candidate_index) {
        if (served.at(candidate_index)) {
            continue;
        }

        const Node& candidate = node_by_internal_index(instance, candidate_index);

        if (candidate.demand > instance.capacity - current_load) {
            continue;
        }

        const Distance candidate_distance =
            euclidean_distance(current, candidate);

        if (!best_candidate.has_value() ||
            candidate_distance < best_candidate->distance ||
            (candidate_distance == best_candidate->distance &&
             candidate_index < best_candidate->node_index)) {
            best_candidate = Candidate{candidate_index, candidate_distance};
        }
    }

    return best_candidate;
}

}

NearestNeighborResult solve_nearest_neighbor(const Instance& instance) {
    if (instance.nodes.empty()) {
        throw std::invalid_argument("Instance phải chứa depot tại chỉ số nội bộ 0");
    }

    const auto started_at = std::chrono::steady_clock::now();

    Solution solution;
    std::vector<bool> served(instance.nodes.size(), false);
    served.at(0) = true;

    std::size_t remaining_customers = instance.nodes.size() - 1;

    while (remaining_customers > 0) {
        Route route{{0}};
        NodeIndex current_node = 0;
        Demand current_load = 0;

        while (true) {
            const std::optional<Candidate> next_customer =
                find_nearest_feasible_customer(
                    instance,
                    served,
                    current_node,
                    current_load);

            if (!next_customer.has_value()) {
                if (route.stops.size() == 1) {
                    throw std::logic_error(
                        "Không thể chọn khách hợp lệ dù vẫn còn khách chưa phục vụ");
                }

                route.stops.push_back(0);
                solution.routes.push_back(std::move(route));
                break;
            }

            route.stops.push_back(next_customer->node_index);
            served.at(next_customer->node_index) = true;
            current_load +=
                node_by_internal_index(instance, next_customer->node_index).demand;
            current_node = next_customer->node_index;
            --remaining_customers;
        }
    }

    const auto finished_at = std::chrono::steady_clock::now();
    const std::chrono::duration<double, std::milli> elapsed =
        finished_at - started_at;

    return NearestNeighborResult{
        std::move(solution),
        elapsed.count(),
    };
}

}
