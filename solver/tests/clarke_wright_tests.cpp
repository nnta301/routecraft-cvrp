#include "routecraft/clarke_wright.hpp"

#include "routecraft/distance.hpp"
#include "routecraft/validator.hpp"
#include "routecraft/vrp_parser.hpp"

#include <cmath>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using routecraft::Instance;
using routecraft::Node;
using routecraft::NodeIndex;
using routecraft::Route;
using routecraft::Solution;

std::string fixture_dir;

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

Instance make_instance(
    routecraft::Demand capacity,
    const std::vector<std::vector<double>>& points,
    const std::vector<routecraft::Demand>& demands) {
    Instance instance;
    instance.name = "test";
    instance.capacity = capacity;
    for (std::size_t index = 0; index < points.size(); ++index) {
        instance.nodes.push_back(Node{
            index, std::to_string(index + 1), points[index][0], points[index][1], demands[index], index == 0});
    }
    return instance;
}

// Depot (0,0); 1=(3,0); 2=(3,4); 3=(0,4); mỗi khách nhu cầu 1.
Instance triangle_instance(routecraft::Demand capacity) {
    return make_instance(capacity, {{0, 0}, {3, 0}, {3, 4}, {0, 4}}, {0, 1, 1, 1});
}

void require_routes(const Solution& solution, const std::vector<std::vector<NodeIndex>>& expected, const std::string& message) {
    require(solution.routes.size() == expected.size(), message + ": sai số tuyến");
    for (std::size_t index = 0; index < expected.size(); ++index) {
        require(solution.routes[index].stops == expected[index], message + ": sai tuyến " + std::to_string(index));
    }
}

void require_valid(const Instance& instance, const Solution& solution, const std::string& message) {
    const routecraft::ValidationResult validation = routecraft::validate_solution(instance, solution);
    require(validation.feasible, message + ": validator từ chối nghiệm");
}

// d01=3, d02=5, d03=4, d12=4, d13=5, d23=3.
// s(1,2)=3+5-4=4, s(1,3)=3+4-5=2, s(2,3)=5+4-3=6.
// Thứ tự: (2,3)=6, (1,2)=4, (1,3)=2.
// Ghép (2,3) -> [2,3]; ghép (1,2) -> [1,2,3]; (1,3) cùng tuyến nên bỏ qua.
// Tuyến 0-1-2-3-0 dài 3+4+3+4 = 14.
void test_hand_computed_w0() {
    const Instance instance = triangle_instance(10);
    const routecraft::ClarkeWrightResult result = routecraft::solve_clarke_wright_w0(instance);
    require_routes(result.solution, {{0, 1, 2, 3, 0}}, "w0 tính tay");
    const routecraft::ValidationResult validation = routecraft::validate_solution(instance, result.solution);
    require(validation.feasible && validation.total_distance == 14, "w0 tính tay: tổng quãng đường phải bằng 14");
    require(result.construction_time_ms >= 0.0, "thời gian construct không âm");
}

// Sức chứa 2: ghép (2,3) được (tải 2); (1,2) có tải 3 > 2 nên bị chặn; (1,3) bị chặn.
// Còn [1] và [2,3]: 6 + (5+3+4) = 18.
void test_capacity_blocks_merge() {
    const Instance instance = triangle_instance(2);
    const routecraft::ClarkeWrightResult result = routecraft::solve_clarke_wright_w0(instance);
    require_routes(result.solution, {{0, 1, 0}, {0, 2, 3, 0}}, "tải chặn ghép");
    const routecraft::ValidationResult validation = routecraft::validate_solution(instance, result.solution);
    require(validation.feasible && validation.total_distance == 18, "tải chặn ghép: tổng phải bằng 18");
}

// Hai khách thẳng hàng hai phía depot: s0 = 3 + 4 - 7 = 0, không bao giờ ghép,
// kể cả khi hàm điểm đánh giá cặp này rất cao.
void test_non_positive_savings_never_merge() {
    const Instance instance = make_instance(10, {{0, 0}, {3, 0}, {-4, 0}}, {0, 1, 1});
    require_routes(routecraft::solve_clarke_wright_w0(instance).solution, {{0, 1, 0}, {0, 2, 0}}, "w0 s0=0");
    const routecraft::PairScore huge = [](NodeIndex, NodeIndex) { return 1e9; };
    require_routes(routecraft::solve_clarke_wright(instance, huge).solution, {{0, 1, 0}, {0, 2, 0}}, "điểm lớn s0=0");
}

// Điểm hằng: thứ tự (1,2), (1,3), (2,3).
// (1,2): [1,2]. (1,3): 1 là đầu mút, đảo [1,2] thành [2,1], nối 3 -> [2,1,3]. (2,3) cùng tuyến.
// Đầu 2 <= cuối 3 nên giữ nguyên chiều: 0-2-1-3-0.
void test_ties_are_deterministic() {
    const Instance instance = triangle_instance(10);
    const routecraft::PairScore constant = [](NodeIndex, NodeIndex) { return 1.0; };
    const routecraft::ClarkeWrightResult first = routecraft::solve_clarke_wright(instance, constant);
    const routecraft::ClarkeWrightResult second = routecraft::solve_clarke_wright(instance, constant);
    require_routes(first.solution, {{0, 2, 1, 3, 0}}, "hòa điểm");
    require(first.solution.routes[0].stops == second.solution.routes[0].stops, "hòa điểm phải lặp lại được");
}

void test_arbitrary_score_stays_feasible() {
    const Instance instance = make_instance(
        7,
        {{0, 0}, {10, 2}, {7, 7}, {-5, 8}, {-9, -3}, {2, -8}, {8, -6}, {-2, 5}},
        {0, 3, 2, 4, 1, 3, 2, 2});
    const routecraft::DistanceMatrix distances(instance);
    const routecraft::PairScore reversed = [&distances](NodeIndex i, NodeIndex j) {
        return -static_cast<double>(distances(0, i) + distances(0, j) - distances(i, j));
    };
    require_valid(instance, routecraft::solve_clarke_wright(instance, reversed).solution, "điểm ưu tiên ngược");
    const routecraft::PairScore by_index = [](NodeIndex i, NodeIndex j) { return static_cast<double>(i * 10 + j); };
    require_valid(instance, routecraft::solve_clarke_wright(instance, by_index).solution, "điểm theo chỉ số");
}

template <typename Fn>
bool throws_invalid_argument(Fn&& fn) {
    try {
        fn();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

void test_non_finite_values_are_rejected() {
    const Instance instance = triangle_instance(10);
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    for (const double bad : {nan, inf, -inf}) {
        require(throws_invalid_argument([&] { routecraft::solve_clarke_wright_w1(instance, {bad, 0.0, 0.0}); }),
                "lambda không hữu hạn phải bị từ chối");
        require(throws_invalid_argument([&] { routecraft::solve_clarke_wright_w1(instance, {1.0, bad, 0.0}); }),
                "mu không hữu hạn phải bị từ chối");
        require(throws_invalid_argument([&] { routecraft::solve_clarke_wright_w1(instance, {1.0, 0.0, bad}); }),
                "nu không hữu hạn phải bị từ chối");
        const routecraft::PairScore score = [bad](NodeIndex, NodeIndex) { return bad; };
        require(throws_invalid_argument([&] { routecraft::solve_clarke_wright(instance, score); }),
                "điểm không hữu hạn phải bị từ chối");
    }
}

void test_w1_defaults_match_w0() {
    const Instance instance = triangle_instance(10);
    const Solution w0 = routecraft::solve_clarke_wright_w0(instance).solution;
    const Solution w1 = routecraft::solve_clarke_wright_w1(instance, {}).solution;
    require(w0.routes.size() == w1.routes.size() && w0.routes[0].stops == w1.routes[0].stops,
            "W1 mặc định phải trùng W0");
}

void test_fixtures_validate() {
    const std::vector<std::string> names{
        "sprint1-depot-not-first.vrp", "sprint1-full-capacity.vrp", "sprint1-noncontiguous-ids.vrp",
        "sprint1-one-customer.vrp", "sprint1-toy.vrp"};
    const std::vector<routecraft::ClarkeWrightParameters> settings{
        {1.0, 0.0, 0.0}, {1.4, 0.3, 0.5}, {0.6, -0.2, 1.0}};
    for (const std::string& name : names) {
        const Instance instance = routecraft::parse_cvrplib_file(std::filesystem::path(fixture_dir) / name);
        require_valid(instance, routecraft::solve_clarke_wright_w0(instance).solution, name + " W0");
        for (const routecraft::ClarkeWrightParameters& parameters : settings) {
            require_valid(instance, routecraft::solve_clarke_wright_w1(instance, parameters).solution, name + " W1");
        }
    }
}

struct TestCase {
    const char* name;
    std::function<void()> run;
};

}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Cách dùng: routecraft_clarke_wright_tests <thư mục fixtures>\n";
        return 2;
    }
    fixture_dir = argv[1];

    const std::vector<TestCase> test_cases{
        {"w0_tinh_tay", test_hand_computed_w0},
        {"tai_chan_ghep", test_capacity_blocks_merge},
        {"savings_khong_duong_khong_ghep", test_non_positive_savings_never_merge},
        {"hoa_diem_xac_dinh", test_ties_are_deterministic},
        {"diem_tuy_y_van_hop_le", test_arbitrary_score_stays_feasible},
        {"tham_so_khong_huu_han_bi_tu_choi", test_non_finite_values_are_rejected},
        {"w1_mac_dinh_bang_w0", test_w1_defaults_match_w0},
        {"fixture_qua_validator", test_fixtures_validate},
    };

    int failed_tests = 0;
    for (const TestCase& test_case : test_cases) {
        try {
            test_case.run();
            std::cout << "[PASS] " << test_case.name << '\n';
        } catch (const std::exception& exception) {
            ++failed_tests;
            std::cerr << "[FAIL] " << test_case.name << ": " << exception.what() << '\n';
        }
    }
    return failed_tests == 0 ? 0 : 1;
}
