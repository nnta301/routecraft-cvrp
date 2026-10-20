#include "routecraft/distance.hpp"
#include "routecraft/nearest_neighbor.hpp"

#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using routecraft::Demand;
using routecraft::Instance;
using routecraft::NearestNeighborResult;
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

void require_stops_equal(
    const Route& route,
    const std::vector<NodeIndex>& expected_stops,
    const std::string& message) {
    require(route.stops == expected_stops, message);
}

void require_solutions_equal(
    const Solution& first,
    const Solution& second,
    const std::string& message) {
    require(first.routes.size() == second.routes.size(), message);

    for (std::size_t route_index = 0; route_index < first.routes.size(); ++route_index) {
        require(
            first.routes.at(route_index).stops == second.routes.at(route_index).stops,
            message);
    }
}

void require_valid_solver_output(
    const Instance& instance,
    const Solution& solution) {
    std::vector<std::size_t> visit_counts(instance.nodes.size(), 0);

    for (const Route& route : solution.routes) {
        require(
            route.stops.size() >= 3,
            "Mỗi tuyến phải chứa depot, ít nhất một khách và depot");

        require(
            route.stops.front() == 0 && route.stops.back() == 0,
            "Mỗi tuyến phải bắt đầu và kết thúc tại depot 0");

        require(
            routecraft::route_load(instance, route) <= instance.capacity,
            "Tải tuyến không được vượt capacity");

        for (std::size_t position = 1; position + 1 < route.stops.size(); ++position) {
            const NodeIndex customer_index = route.stops.at(position);

            require(
                customer_index > 0 && customer_index < instance.nodes.size(),
                "Tuyến chỉ được chứa chỉ số khách hợp lệ ở giữa hai depot");

            ++visit_counts.at(customer_index);
        }
    }

    for (NodeIndex customer_index = 1; customer_index < instance.nodes.size(); ++customer_index) {
        require(
            visit_counts.at(customer_index) == 1,
            "Mỗi khách phải được phục vụ đúng một lần");
    }
}

void test_one_customer_creates_one_non_empty_route() {
    const Instance instance{
        "one-customer",
        10,
        {
            make_node(0, "depot", 0.0, 0.0, 0, true),
            make_node(1, "customer-1", 3.0, 4.0, 4),
        },
    };

    const NearestNeighborResult result =
        routecraft::solve_nearest_neighbor(instance);

    require(result.solution.routes.size() == 1, "Phải tạo đúng một tuyến");
    require_stops_equal(
        result.solution.routes.at(0),
        {0, 1, 0},
        "Tuyến một khách phải có dạng 0 -> 1 -> 0");
    require(
        result.construction_time_ms >= 0.0,
        "Thời gian xây dựng nghiệm phải không âm");
    require_valid_solver_output(instance, result.solution);
}

void test_route_can_fill_capacity_exactly() {
    const Instance instance{
        "exact-capacity",
        10,
        {
            make_node(0, "depot", 0.0, 0.0, 0, true),
            make_node(1, "customer-1", 1.0, 0.0, 4),
            make_node(2, "customer-2", 2.0, 0.0, 6),
        },
    };

    const NearestNeighborResult result =
        routecraft::solve_nearest_neighbor(instance);

    require(result.solution.routes.size() == 1, "Hai khách vừa tải phải cùng một tuyến");
    require_stops_equal(
        result.solution.routes.at(0),
        {0, 1, 2, 0},
        "Tuyến phải phục vụ cả hai khách trước khi quay về depot");
    require(
        routecraft::route_load(instance, result.solution.routes.at(0)) == 10,
        "Tải tuyến phải bằng đúng capacity");
    require_valid_solver_output(instance, result.solution);
}

void test_multiple_routes_serve_every_customer_once() {
    const Instance instance{
        "multiple-routes",
        5,
        {
            make_node(0, "depot", 0.0, 0.0, 0, true),
            make_node(1, "customer-1", 1.0, 0.0, 3),
            make_node(2, "customer-2", 2.0, 0.0, 3),
            make_node(3, "customer-3", 3.0, 0.0, 2),
        },
    };

    const NearestNeighborResult result =
        routecraft::solve_nearest_neighbor(instance);

    require(result.solution.routes.size() == 2, "Phải tạo hai tuyến theo capacity");
    require_stops_equal(
        result.solution.routes.at(0),
        {0, 1, 3, 0},
        "Tuyến đầu phải chọn khách gần nhất còn vừa tải");
    require_stops_equal(
        result.solution.routes.at(1),
        {0, 2, 0},
        "Khách còn lại phải nằm trong tuyến thứ hai");
    require_valid_solver_output(instance, result.solution);
}

void test_equal_distances_use_lower_internal_index() {
    const Instance instance{
        "equal-distances",
        2,
        {
            make_node(0, "depot", 0.0, 0.0, 0, true),
            make_node(1, "customer-1", 1.0, 0.0, 1),
            make_node(2, "customer-2", 0.0, 1.0, 1),
        },
    };

    const NearestNeighborResult first_result =
        routecraft::solve_nearest_neighbor(instance);
    const NearestNeighborResult second_result =
        routecraft::solve_nearest_neighbor(instance);

    require(first_result.solution.routes.size() == 1, "Phải tạo một tuyến");
    require_stops_equal(
        first_result.solution.routes.at(0),
        {0, 1, 2, 0},
        "Khi đồng khoảng cách, phải chọn internal_index nhỏ hơn");
    require_solutions_equal(
        first_result.solution,
        second_result.solution,
        "Cùng input phải luôn tạo cùng nghiệm");
    require_valid_solver_output(instance, first_result.solution);
}

struct TestCase {
    const char* name;
    void (*run)();
};

}

int main() {
    const std::vector<TestCase> test_cases{
        {"mot_khach_mot_tuyen_khong_rong", test_one_customer_creates_one_non_empty_route},
        {"tuyen_vua_day_tai", test_route_can_fill_capacity_exactly},
        {"nhieu_tuyen_phuc_vu_dung_mot_lan", test_multiple_routes_serve_every_customer_once},
        {"pha_hoa_theo_chi_so_noi_bo", test_equal_distances_use_lower_internal_index},
    };

    int failed_tests = 0;

    for (const TestCase& test_case : test_cases) {
        try {
            test_case.run();
            std::cout << "[PASS] " << test_case.name << '\n';
        } catch (const std::exception& exception) {
            ++failed_tests;
            std::cerr << "[FAIL] " << test_case.name << ": "
                      << exception.what() << '\n';
        }
    }

    return failed_tests == 0 ? 0 : 1;
}
