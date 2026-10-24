#include "routecraft/nearest_neighbor.hpp"
#include "routecraft/result_export.hpp"
#include "routecraft/validator.hpp"
#include "routecraft/version.hpp"
#include "routecraft/vrp_parser.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

namespace {

void print_help(std::ostream& stream) {
    stream << "Cách dùng: routecraft [--help|-h|--version] | solve --input <file.vrp> --method nn --format json|txt\n"
           << "\n"
           << "Lệnh:\n"
           << "  --help, -h   Hiển thị hướng dẫn này.\n"
           << "  --version    Hiển thị phiên bản.\n"
           << "  solve        Đọc CVRP, chạy Nearest Neighbor và xuất nghiệm.\n"
           << "\n"
           << "Sprint 1 chỉ hỗ trợ --method nn và --format json hoặc txt.\n";
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
    for (int index = 2; index < argc; index += 2) {
        if (index + 1 >= argc) return usage_error("Thiếu giá trị cho tham số");
        const std::string_view option{argv[index]};
        const std::string value{argv[index + 1]};
        if (option == "--input" && input_path.empty()) input_path = value;
        else if (option == "--method" && method.empty()) method = value;
        else if (option == "--format" && format.empty()) format = value;
        else return usage_error("Tham số không hỗ trợ hoặc bị lặp: " + std::string(option));
    }
    if (input_path.empty() || method.empty() || format.empty()) {
        return usage_error("solve yêu cầu --input, --method và --format");
    }
    if (method != "nn") return usage_error("Sprint 1 chỉ hỗ trợ --method nn");
    if (format != "json" && format != "txt") return usage_error("--format chỉ nhận json hoặc txt");

    try {
        const routecraft::Instance instance = routecraft::parse_cvrplib_file(input_path);
        const routecraft::NearestNeighborResult result = routecraft::solve_nearest_neighbor(instance);
        const auto validation_started = std::chrono::steady_clock::now();
        const routecraft::ValidationResult validation = routecraft::validate_solution(instance, result.solution);
        const auto validation_finished = std::chrono::steady_clock::now();
        const routecraft::SolutionTimings timings{
            result.construction_time_ms,
            std::chrono::duration<double, std::milli>(validation_finished - validation_started).count(),
        };
        if (!validation.feasible) {
            std::cerr << "Lỗi: Nghiệm bị validator từ chối:\n";
            for (const std::string& error : validation.errors) std::cerr << "- " << error << "\n";
            return 1;
        }
        if (format == "json") {
            std::cout << routecraft::serialize_solution_json(instance, result.solution, validation, timings);
        } else {
            std::cout << routecraft::serialize_solution_txt(instance, result.solution, validation, timings);
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
