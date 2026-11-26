#include "routecraft/clarke_wright.hpp"

#include "routecraft/distance.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>

namespace routecraft {
namespace {

using Clock = std::chrono::steady_clock;

struct Candidate {
    NodeIndex first{};
    NodeIndex second{};
    double score{};
};

void require_valid_instance(const Instance& instance) {
    if (instance.nodes.empty()) {
        throw std::invalid_argument("Instance phải chứa depot tại chỉ số nội bộ 0");
    }
    for (NodeIndex index = 1; index < instance.nodes.size(); ++index) {
        if (node_by_internal_index(instance, index).demand > instance.capacity) {
            throw std::invalid_argument("Có khách có nhu cầu vượt sức chứa xe");
        }
    }
}

ClarkeWrightResult build_routes(
    const Instance& instance,
    const DistanceMatrix& distances,
    const PairScore& score,
    Clock::time_point started_at) {
    const std::size_t node_count = instance.nodes.size();

    std::vector<Candidate> candidates;
    for (NodeIndex i = 1; i < node_count; ++i) {
        for (NodeIndex j = i + 1; j < node_count; ++j) {
            const Distance savings = distances(0, i) + distances(0, j) - distances(i, j);
            if (savings <= 0) {
                continue;  // savings thật không dương: không bao giờ ghép
            }
            const double value = score(i, j);
            if (!std::isfinite(value)) {
                throw std::invalid_argument("Điểm của cặp khách phải là số hữu hạn");
            }
            candidates.push_back(Candidate{i, j, value});
        }
    }

    // Điểm giảm dần; hòa thì i tăng dần, rồi j tăng dần.
    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        if (a.score != b.score) return a.score > b.score;
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });

    // Mỗi tuyến là danh sách khách (không gồm depot); route_of[c] là chỉ số tuyến.
    std::vector<std::vector<NodeIndex>> routes(node_count);
    std::vector<Demand> loads(node_count, 0);
    std::vector<std::size_t> route_of(node_count, 0);
    for (NodeIndex c = 1; c < node_count; ++c) {
        routes[c] = {c};
        loads[c] = node_by_internal_index(instance, c).demand;
        route_of[c] = c;
    }

    for (const Candidate& candidate : candidates) {
        const std::size_t route_a = route_of[candidate.first];
        const std::size_t route_b = route_of[candidate.second];
        if (route_a == route_b) continue;

        std::vector<NodeIndex>& a = routes[route_a];
        std::vector<NodeIndex>& b = routes[route_b];
        const bool i_is_end = a.front() == candidate.first || a.back() == candidate.first;
        const bool j_is_end = b.front() == candidate.second || b.back() == candidate.second;
        if (!i_is_end || !j_is_end) continue;
        if (loads[route_a] > instance.capacity - loads[route_b]) continue;

        // Đảo chiều để i ở cuối tuyến a và j ở đầu tuyến b, rồi nối b vào sau a.
        if (a.back() != candidate.first) std::reverse(a.begin(), a.end());
        if (b.front() != candidate.second) std::reverse(b.begin(), b.end());
        for (const NodeIndex customer : b) route_of[customer] = route_a;
        a.insert(a.end(), b.begin(), b.end());
        loads[route_a] += loads[route_b];
        b.clear();
        loads[route_b] = 0;
    }

    // Thứ tự output: các tuyến còn lại theo khách nhỏ nhất của tuyến tăng dần
    // (khách nhỏ nhất của tuyến c luôn là phần tử đầu tiên gặp khi duyệt c = 1..n-1);
    // trong mỗi tuyến, khách đầu có chỉ số nhỏ hơn hoặc bằng khách cuối.
    Solution solution;
    std::vector<bool> emitted(node_count, false);
    for (NodeIndex c = 1; c < node_count; ++c) {
        const std::size_t route_index = route_of[c];
        if (emitted[route_index]) continue;
        emitted[route_index] = true;
        std::vector<NodeIndex> customers = routes[route_index];
        if (customers.front() > customers.back()) {
            std::reverse(customers.begin(), customers.end());
        }
        Route route;
        route.stops.reserve(customers.size() + 2);
        route.stops.push_back(0);
        route.stops.insert(route.stops.end(), customers.begin(), customers.end());
        route.stops.push_back(0);
        solution.routes.push_back(std::move(route));
    }

    const std::chrono::duration<double, std::milli> elapsed = Clock::now() - started_at;
    return ClarkeWrightResult{std::move(solution), elapsed.count()};
}

}

ClarkeWrightResult solve_clarke_wright(const Instance& instance, const PairScore& score) {
    require_valid_instance(instance);
    if (!score) {
        throw std::invalid_argument("Hàm điểm không được rỗng");
    }
    const auto started_at = Clock::now();
    const DistanceMatrix distances(instance);
    return build_routes(instance, distances, score, started_at);
}

ClarkeWrightResult solve_clarke_wright_w0(const Instance& instance) {
    require_valid_instance(instance);
    const auto started_at = Clock::now();
    const DistanceMatrix distances(instance);
    const PairScore score = [&distances](NodeIndex i, NodeIndex j) {
        return static_cast<double>(distances(0, i) + distances(0, j) - distances(i, j));
    };
    return build_routes(instance, distances, score, started_at);
}

ClarkeWrightResult solve_clarke_wright_w1(
    const Instance& instance,
    const ClarkeWrightParameters& parameters) {
    if (!std::isfinite(parameters.lambda) || !std::isfinite(parameters.mu) ||
        !std::isfinite(parameters.nu)) {
        throw std::invalid_argument("Tham số lambda, mu, nu của W1 phải là số hữu hạn");
    }
    require_valid_instance(instance);
    const auto started_at = Clock::now();
    const DistanceMatrix distances(instance);

    double total_demand = 0.0;
    for (NodeIndex c = 1; c < instance.nodes.size(); ++c) {
        total_demand += static_cast<double>(node_by_internal_index(instance, c).demand);
    }
    const double customer_count = static_cast<double>(instance.nodes.size() - 1);
    const double mean_demand = customer_count > 0.0 ? total_demand / customer_count : 0.0;

    const PairScore score = [&](NodeIndex i, NodeIndex j) {
        const double d0i = static_cast<double>(distances(0, i));
        const double d0j = static_cast<double>(distances(0, j));
        double value = d0i + d0j - parameters.lambda * static_cast<double>(distances(i, j)) +
                       parameters.mu * std::fabs(d0i - d0j);
        if (mean_demand > 0.0) {
            const double pair_demand = static_cast<double>(
                node_by_internal_index(instance, i).demand + node_by_internal_index(instance, j).demand);
            value += parameters.nu * pair_demand / mean_demand;
        }
        return value;
    };
    return build_routes(instance, distances, score, started_at);
}

}
