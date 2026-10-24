#include "routecraft/cvrp.hpp"
#include "routecraft/distance.hpp"

#include <exception>
#include <cmath>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using routecraft::Demand;
using routecraft::Distance;
using routecraft::Instance;
using routecraft::Node;
using routecraft::NodeIndex;
using routecraft::Route;
using routecraft::Solution;

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

Node make_node(
    NodeIndex internal_index,
    std::string original_id,
    double x,
    double y,
    Demand demand,
    bool is_depot = false) {
    return Node{
        internal_index,
        std::move(original_id),
        x,
        y,
        demand,
        is_depot,
    };
}

Instance make_instance() {
    return Instance{
        "core-test",
        10,
        {
            make_node(0, "42", 0.0, 0.0, 0, true),
            make_node(1, "7", 3.0, 4.0, 3),
            make_node(2, "99", 6.0, 4.0, 4),
        },
    };
}

void test_euclidean_distance_rounds_single_edge() {
    const Node first = make_node(0, "1", 0.0, 0.0, 0, true);
    const Node second = make_node(1, "2", 1.0, 1.0, 0);

    require(
        routecraft::euclidean_distance(first, second) == 1,
        "Khoảng cách sqrt(2) phải được làm tròn thành 1");
}

void test_route_distance_rounds_each_edge_before_summing() {
    const Instance instance{
        "rounding-test",
        10,
        {
            make_node(0, "10", 0.0, 0.0, 0, true),
            make_node(1, "20", 1.0, 1.0, 1),
            make_node(2, "30", 2.0, 2.0, 1),
        },
    };

    const Route route{{0, 1, 2, 0}};

    // sqrt(2) -> 1, sqrt(2) -> 1, sqrt(8) -> 3.
    require(
        routecraft::route_distance(instance, route) == 5,
        "Khoảng cách phải làm tròn từng cạnh trước khi cộng");
}

void test_route_distance_includes_return_to_depot() {
    const Instance instance = make_instance();
    const Route route{{0, 1, 2, 0}};

    // 0 -> 1: 5, 1 -> 2: 3, 2 -> 0: sqrt(52) -> 7.
    require(
        routecraft::route_distance(instance, route) == 15,
        "Độ dài tuyến phải bao gồm cạnh quay về depot");
}

void test_route_load_ignores_depot() {
    const Instance instance = make_instance();
    const Route route{{0, 1, 2, 0}};

    require(
        routecraft::route_load(instance, route) == 7,
        "Tải tuyến phải bằng tổng nhu cầu của các khách");
}

void test_solution_distance_sums_all_routes() {
    const Instance instance = make_instance();

    const Solution solution{
        {
            Route{{0, 1, 0}},
            Route{{0, 2, 0}},
        },
    };

    // Route 1: 5 + 5 = 10.
    // Route 2: 7 + 7 = 14.
    require(
        routecraft::solution_distance(instance, solution) == 24,
        "Tổng độ dài nghiệm phải bằng tổng độ dài mọi tuyến");
}

void test_original_id_and_internal_depot_convention() {
    const Instance instance = make_instance();
    const Route route{{0, 1, 2, 0}};

    require(
        instance.nodes.at(0).internal_index == 0,
        "Depot phải có chỉ số nội bộ là 0");
    require(
        instance.nodes.at(0).is_depot,
        "Node tại chỉ số nội bộ 0 phải được đánh dấu là depot");
    require(
        instance.nodes.at(0).original_id == "42",
        "ID gốc của depot phải được giữ lại");
    require(
        instance.nodes.at(1).original_id == "7",
        "ID gốc của khách phải được giữ lại");
    require(
        route.stops.front() == 0 && route.stops.back() == 0,
        "Route hợp lệ phải bắt đầu và kết thúc tại depot nội bộ 0");
}

void test_checked_distance_validates_domain_before_rounding() {
    Node first = make_node(0, "0", 0, 0, 0, true);
    Node second = make_node(1, "1", 1.5, 0, 1);
    require(routecraft::checked_euclidean_distance(first, second) == 2,
        "Khoảng cách hợp lệ phải giữ cách làm tròn nửa lên");
    second.x = 1;
    second.y = 1;
    require(routecraft::checked_euclidean_distance(first, second) == 1,
        "Khoảng cách sqrt(2) phải làm tròn thành 1");
    second.y = 0;
    const double upper_bound = std::ldexp(1.0, 63);
    second.x = std::nextafter(upper_bound, 0.0);
    require(routecraft::checked_euclidean_distance(first, second) ==
            static_cast<Distance>(second.x),
        "Giá trị double ngay dưới 2^63 vẫn biểu diễn được");
    second.x = upper_bound;
    require(!routecraft::checked_euclidean_distance(first, second),
        "2^63 phải bị loại trước llround");
    first.x = -std::numeric_limits<double>::max();
    second.x = std::numeric_limits<double>::max();
    require(!routecraft::checked_euclidean_distance(first, second),
        "Hiệu tọa độ hữu hạn bị tràn phải bị loại");
    first.x = 0;
    second.x = std::numeric_limits<double>::quiet_NaN();
    require(!routecraft::checked_euclidean_distance(first, second),
        "Khoảng cách NaN phải bị loại");
}

struct TestCase {
    const char* name;
    void (*run)();
};

}

int main() {
    const std::vector<TestCase> test_cases{
        {"euc_2d_kiem_tra_mien", test_checked_distance_validates_domain_before_rounding},
        {"euc_2d_lam_tron_mot_canh", test_euclidean_distance_rounds_single_edge},
        {"euc_2d_lam_tron_tung_canh", test_route_distance_rounds_each_edge_before_summing},
        {"tuyen_co_canh_quay_ve_depot", test_route_distance_includes_return_to_depot},
        {"tai_tuyen", test_route_load_ignores_depot},
        {"tong_do_dai_nghiem", test_solution_distance_sums_all_routes},
        {"id_goc_va_depot_noi_bo", test_original_id_and_internal_depot_convention},
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
