#include "routecraft/validator.hpp"

#include <exception>
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
using routecraft::ValidationResult;

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
    return Node{internal_index, std::move(original_id), x, y, demand, is_depot};
}

Instance make_instance(Demand capacity = 10) {
    return Instance{
        "validator-test",
        capacity,
        {
            make_node(0, "depot", 0.0, 0.0, 0, true),
            make_node(1, "khach-1", 3.0, 4.0, 4),
            make_node(2, "khach-2", 6.0, 4.0, 5),
        },
    };
}

bool has_error_containing(const ValidationResult& result, const std::string& text) {
    for (const std::string& error : result.errors) {
        if (error.find(text) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void test_valid_solution_recalculates_metrics_and_ignores_reported_route_metrics() {
    const Instance instance = make_instance();
    const Solution solution{
        {Route{{0, 1, 2, 0}}},
        {999},
        {999},
        15,
    };

    const ValidationResult result = routecraft::validate_solution(instance, solution);

    require(result.feasible, "Nghiệm hợp lệ phải khả thi");
    require(result.errors.empty(), "Nghiệm hợp lệ không được có lỗi");
    require(result.route_loads == std::vector<Demand>{9}, "Phải tự tính lại tải tuyến");
    require(
        result.route_distances == std::vector<Distance>{15},
        "Phải tự tính lại khoảng cách tuyến theo EUC_2D");
    require(result.total_distance == 15, "Phải tự tính lại tổng khoảng cách");
}

void test_route_must_start_and_end_at_depot() {
    const Solution solution{{Route{{1, 2, 1}}}};
    const ValidationResult result =
        routecraft::validate_solution(make_instance(), solution);

    require(!result.feasible, "Tuyến sai depot phải không khả thi");
    require(has_error_containing(result, "không bắt đầu tại depot 0"), "Phải báo depot đầu");
    require(has_error_containing(result, "không kết thúc tại depot 0"), "Phải báo depot cuối");
}

void test_depot_in_middle_is_rejected() {
    const Solution solution{{Route{{0, 1, 0, 2, 0}}}};
    const ValidationResult result =
        routecraft::validate_solution(make_instance(), solution);

    require(!result.feasible, "Depot ở giữa tuyến phải không khả thi");
    require(has_error_containing(result, "depot 0 ở vị trí không hợp lệ"), "Phải báo depot ở giữa");
}

void test_invalid_customer_index_does_not_access_out_of_range() {
    const Solution solution{{Route{{0, 99, 0}}}};
    const ValidationResult result =
        routecraft::validate_solution(make_instance(), solution);

    require(!result.feasible, "Chỉ số khách không hợp lệ phải không khả thi");
    require(has_error_containing(result, "chỉ số khách không hợp lệ 99"), "Phải báo chỉ số sai");
    require(result.route_distances == std::vector<Distance>{0}, "Khoảng cách không xác định phải là 0");
}

void test_missing_and_duplicate_customers_are_rejected() {
    const Solution solution{{Route{{0, 1, 1, 0}}}};
    const ValidationResult result =
        routecraft::validate_solution(make_instance(), solution);

    require(!result.feasible, "Khách thiếu hoặc trùng phải không khả thi");
    require(has_error_containing(result, "khach-1"), "Phải báo khách trùng");
    require(has_error_containing(result, "khach-2"), "Phải báo khách thiếu");
}

void test_over_capacity_is_rejected() {
    const Solution solution{{Route{{0, 1, 2, 0}}}};
    const ValidationResult result = routecraft::validate_solution(make_instance(8), solution);

    require(!result.feasible, "Tuyến quá tải phải không khả thi");
    require(has_error_containing(result, "vượt tải: 9 > 8"), "Phải báo tải tính lại");
}

void test_incorrect_reported_total_distance_is_rejected() {
    const Solution solution{
        {Route{{0, 1, 2, 0}}},
        {0},
        {0},
        999,
    };
    const ValidationResult result =
        routecraft::validate_solution(make_instance(), solution);

    require(!result.feasible, "Tổng khoảng cách khai báo sai phải không khả thi");
    require(
        has_error_containing(result, "Tổng khoảng cách khai báo 999 không khớp tổng tính lại 15"),
        "Phải đối chiếu tổng khai báo với tổng tính lại");
    require(result.total_distance == 15, "Không được dùng tổng khai báo sai");
}

struct TestCase {
    const char* name;
    void (*run)();
};

}

int main() {
    const std::vector<TestCase> test_cases{
        {"nghiem_hop_le_tinh_lai_so_lieu", test_valid_solution_recalculates_metrics_and_ignores_reported_route_metrics},
        {"depot_dau_cuoi", test_route_must_start_and_end_at_depot},
        {"depot_o_giua", test_depot_in_middle_is_rejected},
        {"chi_so_khach_khong_hop_le", test_invalid_customer_index_does_not_access_out_of_range},
        {"khach_thieu_va_trung", test_missing_and_duplicate_customers_are_rejected},
        {"vuot_tai", test_over_capacity_is_rejected},
        {"tong_khoang_cach_khai_bao_sai", test_incorrect_reported_total_distance_is_rejected},
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
