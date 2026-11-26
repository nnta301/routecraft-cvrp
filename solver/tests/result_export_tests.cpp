#include "routecraft/result_export.hpp"
#include "routecraft/validator.hpp"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

routecraft::Instance make_instance() {
    return {"export-test", 10, {{0, "depot", 0, 0, 0, true}, {1, "customer", 1, 0, 3, false}}};
}

void test_infeasible_solution_cannot_be_serialized() {
    const routecraft::Instance instance = make_instance();
    const routecraft::Solution solution{{routecraft::Route{{0, 0}}}};
    const routecraft::ValidationResult validation = routecraft::validate_solution(instance, solution);
    require(!validation.feasible, "Tiền điều kiện: nghiệm phải bị validator bác bỏ");
    bool rejected = false;
    try {
        static_cast<void>(routecraft::serialize_solution_json(instance, solution, validation, {}));
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    require(rejected, "Không được serialize nghiệm bị validator bác bỏ");
}

void test_parameters_only_for_cw() {
    const routecraft::Instance instance = make_instance();
    const routecraft::Solution solution{{routecraft::Route{{0, 1, 0}}}};
    const routecraft::ValidationResult validation = routecraft::validate_solution(instance, solution);
    require(validation.feasible, "Tiền điều kiện: nghiệm phải khả thi");
    const std::string nn_json = routecraft::serialize_solution_json(instance, solution, validation, {});
    require(nn_json.find("\"method\": \"nn\"") != std::string::npos, "JSON nn phải ghi method nn");
    require(nn_json.find("\"parameters\"") == std::string::npos, "JSON nn không được có parameters");
    const routecraft::SolutionMethod cw{"cw", 1.5, 0.0, 2.0};
    const std::string cw_json = routecraft::serialize_solution_json(instance, solution, validation, {}, cw);
    require(cw_json.find("\"method\": \"cw\"") != std::string::npos, "JSON cw phải ghi method cw");
    require(cw_json.find("\"parameters\": {\"lambda\": 1.5, \"mu\": 0, \"nu\": 2}") != std::string::npos,
            "JSON cw phải có parameters");
    const std::string cw_txt = routecraft::serialize_solution_txt(instance, solution, validation, {}, cw);
    require(cw_txt.find("Parameters: lambda=1.5, mu=0, nu=2") != std::string::npos, "TXT cw phải có tham số");
}

struct TestCase { const char* name; void (*run)(); };

}

int main() {
    const std::vector<TestCase> test_cases{
        {"khong_xuat_nghiem_khong_kha_thi", test_infeasible_solution_cannot_be_serialized},
        {"tham_so_chi_cho_cw", test_parameters_only_for_cw},
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
