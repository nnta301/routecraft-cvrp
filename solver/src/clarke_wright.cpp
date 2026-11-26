#include "routecraft/clarke_wright.hpp"

#include "routecraft/distance.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace routecraft {
namespace {

using Clock = std::chrono::steady_clock;

constexpr std::size_t kMaxGridConfigurations = 1000000;

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

// Điểm giảm dần; hòa thì i tăng dần, rồi j tăng dần.
bool candidate_before(const Candidate& a, const Candidate& b) {
    if (a.score != b.score) return a.score > b.score;
    if (a.first != b.first) return a.first < b.first;
    return a.second < b.second;
}

// Thủ tục ghép duy nhất của mọi biến thể Clarke-Wright: duyệt `candidates`
// (đã sắp) một lượt, ghép đầu mút hai tuyến khác nhau nếu không vượt tải.
Solution merge_routes(const Instance& instance, const std::vector<Candidate>& candidates) {
    const std::size_t node_count = instance.nodes.size();

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

    return solution;
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
    std::sort(candidates.begin(), candidates.end(), candidate_before);

    Solution solution = merge_routes(instance, candidates);
    const std::chrono::duration<double, std::milli> elapsed = Clock::now() - started_at;
    return ClarkeWrightResult{std::move(solution), elapsed.count()};
}

double mean_customer_demand(const Instance& instance) {
    double total_demand = 0.0;
    for (NodeIndex c = 1; c < instance.nodes.size(); ++c) {
        total_demand += static_cast<double>(node_by_internal_index(instance, c).demand);
    }
    const double customer_count = static_cast<double>(instance.nodes.size() - 1);
    return customer_count > 0.0 ? total_demand / customer_count : 0.0;
}

// Công thức điểm W1 duy nhất, dùng chung cho W1 và lưới W1.
double w1_score(
    double d0i,
    double d0j,
    double dij,
    double pair_demand,
    double mean_demand,
    const ClarkeWrightParameters& parameters) {
    double value = d0i + d0j - parameters.lambda * dij + parameters.mu * std::fabs(d0i - d0j);
    if (mean_demand > 0.0) {
        value += parameters.nu * pair_demand / mean_demand;
    }
    return value;
}

// Giá trị thứ k của một trục lưới: start + k*step.
std::size_t axis_count(const ParameterRange& range, const char* name) {
    if (!std::isfinite(range.start) || !std::isfinite(range.stop) || !std::isfinite(range.step)) {
        throw std::invalid_argument(std::string("Lưới ") + name + " có giá trị không hữu hạn");
    }
    if (range.step <= 0.0) {
        throw std::invalid_argument(std::string("Bước của lưới ") + name + " phải dương");
    }
    if (range.start > range.stop) {
        throw std::invalid_argument(std::string("Lưới ") + name + " có start lớn hơn stop");
    }
    const double steps = (range.stop - range.start) / range.step;
    if (!std::isfinite(steps) || steps > static_cast<double>(kMaxGridConfigurations)) {
        throw std::invalid_argument(std::string("Lưới ") + name + " có quá nhiều giá trị");
    }
    return static_cast<std::size_t>(std::llround(steps)) + 1;
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

    const double mean_demand = mean_customer_demand(instance);

    const PairScore score = [&](NodeIndex i, NodeIndex j) {
        const double pair_demand = static_cast<double>(
            node_by_internal_index(instance, i).demand + node_by_internal_index(instance, j).demand);
        return w1_score(
            static_cast<double>(distances(0, i)), static_cast<double>(distances(0, j)),
            static_cast<double>(distances(i, j)), pair_demand, mean_demand, parameters);
    };
    return build_routes(instance, distances, score, started_at);
}

void validate_clarke_wright_grid(const ClarkeWrightGrid& grid) {
    const std::size_t lambda_count = axis_count(grid.lambda, "lambda");
    const std::size_t mu_count = axis_count(grid.mu, "mu");
    const std::size_t nu_count = axis_count(grid.nu, "nu");
    if (lambda_count > kMaxGridConfigurations / mu_count ||
        lambda_count * mu_count > kMaxGridConfigurations / nu_count) {
        throw std::invalid_argument("Lưới có quá nhiều bộ tham số (tối đa 1.000.000)");
    }
}

ClarkeWrightGridResult solve_clarke_wright_w1_grid(
    const Instance& instance,
    const ClarkeWrightGrid& grid) {
    validate_clarke_wright_grid(grid);
    const std::size_t lambda_count = axis_count(grid.lambda, "lambda");
    const std::size_t mu_count = axis_count(grid.mu, "mu");
    const std::size_t nu_count = axis_count(grid.nu, "nu");
    require_valid_instance(instance);
    const auto started_at = Clock::now();
    const DistanceMatrix distances(instance);
    const double mean_demand = mean_customer_demand(instance);

    // Dữ liệu cặp dựng một lần, theo thứ tự (i, j) như build_routes.
    struct PairData {
        double d0i, d0j, dij, demand;
    };
    std::vector<PairData> pairs;
    std::vector<Candidate> candidates;
    const std::size_t node_count = instance.nodes.size();
    for (NodeIndex i = 1; i < node_count; ++i) {
        for (NodeIndex j = i + 1; j < node_count; ++j) {
            if (distances(0, i) + distances(0, j) - distances(i, j) <= 0) continue;
            pairs.push_back(PairData{
                static_cast<double>(distances(0, i)), static_cast<double>(distances(0, j)),
                static_cast<double>(distances(i, j)),
                static_cast<double>(node_by_internal_index(instance, i).demand +
                                    node_by_internal_index(instance, j).demand)});
            candidates.push_back(Candidate{i, j, 0.0});
        }
    }
    const std::vector<Candidate> original_order = candidates;

    ClarkeWrightGridResult best;
    Distance best_distance = 0;
    std::size_t best_routes = 0;
    std::size_t done = 0;
    for (std::size_t a = 0; a < lambda_count; ++a) {
        for (std::size_t b = 0; b < mu_count; ++b) {
            for (std::size_t c = 0; c < nu_count; ++c) {
                const ClarkeWrightParameters parameters{
                    grid.lambda.start + static_cast<double>(a) * grid.lambda.step,
                    grid.mu.start + static_cast<double>(b) * grid.mu.step,
                    grid.nu.start + static_cast<double>(c) * grid.nu.step};
                for (std::size_t p = 0; p < pairs.size(); ++p) {
                    const double value = w1_score(
                        pairs[p].d0i, pairs[p].d0j, pairs[p].dij, pairs[p].demand, mean_demand, parameters);
                    if (!std::isfinite(value)) {
                        throw std::invalid_argument("Điểm của cặp khách phải là số hữu hạn");
                    }
                    candidates[p] = original_order[p];
                    candidates[p].score = value;
                }
                std::sort(candidates.begin(), candidates.end(), candidate_before);
                Solution solution = merge_routes(instance, candidates);
                const Distance total = solution_distance(instance, solution);
                ++done;
                if (done == 1 || total < best_distance ||
                    (total == best_distance && solution.routes.size() < best_routes)) {
                    best_distance = total;
                    best_routes = solution.routes.size();
                    best.solution = std::move(solution);
                    best.best_parameters = parameters;
                }
            }
        }
    }
    best.configurations = done;
    best.total_distance = best_distance;
    const std::chrono::duration<double, std::milli> elapsed = Clock::now() - started_at;
    best.construction_time_ms = elapsed.count();
    return best;
}

}
