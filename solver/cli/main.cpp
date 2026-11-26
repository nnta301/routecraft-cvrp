#include "routecraft/clarke_wright.hpp"
#include "routecraft/nearest_neighbor.hpp"
#include "routecraft/result_export.hpp"
#include "routecraft/validator.hpp"
#include "routecraft/version.hpp"
#include "routecraft/vrp_parser.hpp"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace {

void print_help(std::ostream& stream) {
    stream << "Cách dùng: routecraft [--help|-h|--version] | solve --input <file.vrp> --method nn|cw [--lambda L] [--mu M] [--nu N] --format json|txt\n"
           << "\n"
           << "Lệnh:\n"
           << "  --help, -h   Hiển thị hướng dẫn này.\n"
           << "  --version    Hiển thị phiên bản.\n"
           << "  solve        Đọc CVRP, chạy Nearest Neighbor (nn) hoặc Clarke–Wright parallel (cw) và xuất nghiệm.\n"
           << "\n"
           << "--lambda, --mu, --nu (mặc định 1, 0, 0) chỉ dùng với --method cw.\n"
           << "--format nhận json hoặc txt.\n";
}

bool parse_finite(const std::string& text, double& value) {
    if (text.empty()) return false;
    char* end = nullptr;
    value = std::strtod(text.c_str(), &end);
    return end == text.c_str() + text.size() && std::isfinite(value);
}

int usage_error(std::string_view message) {
    std::cerr << "Lỗi: " << message << "\n";
    print_help(std::cerr);
    return 2;
}

int solve(int argc, char* argv[]) {
    std::string input_path;
    std::string method;
    std::string format;
    bool input_seen = false;
    bool method_seen = false;
    bool format_seen = false;
    routecraft::ClarkeWrightParameters parameters;
    bool lambda_seen = false;
    bool mu_seen = false;
    bool nu_seen = false;
    for (int index = 2; index < argc; index += 2) {
        if (index + 1 >= argc) return usage_error("Thiếu giá trị cho tham số");
        const std::string_view option{argv[index]};
        const std::string value{argv[index + 1]};
        if (option == "--input" && !input_seen) {
            input_seen = true;
            input_path = value;
        } else if (option == "--method" && !method_seen) {
            method_seen = true;
            method = value;
        } else if (option == "--format" && !format_seen) {
            format_seen = true;
            format = value;
        } else if ((option == "--lambda" && !lambda_seen) ||
                   (option == "--mu" && !mu_seen) ||
                   (option == "--nu" && !nu_seen)) {
            double number = 0.0;
            if (!parse_finite(value, number)) {
                return usage_error("Giá trị của " + std::string(option) + " phải là số hữu hạn");
            }
            if (option == "--lambda") { lambda_seen = true; parameters.lambda = number; }
            else if (option == "--mu") { mu_seen = true; parameters.mu = number; }
            else { nu_seen = true; parameters.nu = number; }
        }
        else return usage_error("Tham số không hỗ trợ hoặc bị lặp: " + std::string(option));
    }
    if (!input_seen || !method_seen || !format_seen ||
        input_path.empty() || method.empty() || format.empty()) {
        return usage_error("solve yêu cầu --input, --method và --format");
    }
    if (method != "nn" && method != "cw") return usage_error("--method chỉ nhận nn hoặc cw");
    if (method == "nn" && (lambda_seen || mu_seen || nu_seen)) {
        return usage_error("--lambda, --mu, --nu chỉ dùng với --method cw");
    }
    if (format != "json" && format != "txt") return usage_error("--format chỉ nhận json hoặc txt");

    try {
        const routecraft::Instance instance = routecraft::parse_cvrplib_file(input_path);
        routecraft::Solution solution;
        double construction_time_ms = 0.0;
        if (method == "cw") {
            routecraft::ClarkeWrightResult result = routecraft::solve_clarke_wright_w1(instance, parameters);
            solution = std::move(result.solution);
            construction_time_ms = result.construction_time_ms;
        } else {
            routecraft::NearestNeighborResult result = routecraft::solve_nearest_neighbor(instance);
            solution = std::move(result.solution);
            construction_time_ms = result.construction_time_ms;
        }
        const routecraft::SolutionMethod solution_method{method, parameters.lambda, parameters.mu, parameters.nu};
        const auto validation_started = std::chrono::steady_clock::now();
        const routecraft::ValidationResult validation = routecraft::validate_solution(instance, solution);
        const auto validation_finished = std::chrono::steady_clock::now();
        const routecraft::SolutionTimings timings{
            construction_time_ms,
            std::chrono::duration<double, std::milli>(validation_finished - validation_started).count(),
        };
        if (!validation.feasible) {
            std::cerr << "Lỗi: Nghiệm bị validator từ chối:\n";
            for (const std::string& error : validation.errors) std::cerr << "- " << error << "\n";
            return 1;
        }
        if (format == "json") {
            std::cout << routecraft::serialize_solution_json(instance, solution, validation, timings, solution_method);
        } else {
            std::cout << routecraft::serialize_solution_txt(instance, solution, validation, timings, solution_method);
        }
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "Lỗi: " << exception.what() << "\n";
        return 1;
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc == 1) {
        print_help(std::cerr);
        return 2;
    }

    const std::string_view argument{argv[1]};
    if (argument == "--help" || argument == "-h") {
        if (argc != 2) return usage_error("--help không nhận thêm tham số");
        print_help(std::cout);
        return 0;
    }
    if (argument == "--version") {
        if (argc != 2) return usage_error("--version không nhận thêm tham số");
        std::cout << "routecraft " << routecraft::version() << "\n";
        return 0;
    }
    if (argument == "solve") {
        return solve(argc, argv);
    }

    return usage_error("Tùy chọn hoặc lệnh không hỗ trợ");
}
