#include "routecraft/vrp_parser.hpp"

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using routecraft::Instance;
using routecraft::ParseError;

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

std::string valid_instance_text() {
    return R"(NAME : irregular-toy
TYPE : CVRP
DIMENSION : 3
CAPACITY : 10
EDGE_WEIGHT_TYPE : EUC_2D
NODE_COORD_SECTION
7 3 4
42 0 0
99 6 4
DEMAND_SECTION
7 3
42 0
99 4
DEPOT_SECTION
42
-1
EOF
)";
}

Instance parse_text(const std::string& text) {
    std::istringstream input(text);
    return routecraft::parse_cvrplib(input);
}

std::string replace_once(
    std::string text,
    const std::string& from,
    const std::string& to) {
    const std::size_t position = text.find(from);

    if (position == std::string::npos) {
        throw std::runtime_error("Không tìm thấy đoạn cần thay trong fixture");
    }

    text.replace(position, from.size(), to);
    return text;
}

void require_parse_error(const std::string& text, const std::string& case_name) {
    try {
        static_cast<void>(parse_text(text));
    } catch (const ParseError&) {
        return;
    }

    throw std::runtime_error("Parser phải từ chối trường hợp: " + case_name);
}

void test_parses_valid_instance_and_normalizes_depot() {
    const Instance instance = parse_text(valid_instance_text());

    require(instance.name == "irregular-toy", "NAME phải được giữ lại");
    require(instance.capacity == 10, "CAPACITY phải được đọc đúng");
    require(instance.nodes.size() == 3, "DIMENSION phải tính cả depot");

    require(instance.nodes.at(0).internal_index == 0, "Depot phải có chỉ số nội bộ 0");
    require(instance.nodes.at(0).original_id == "42", "Phải giữ ID gốc của depot");
    require(instance.nodes.at(0).is_depot, "Node 0 phải được đánh dấu là depot");
    require(instance.nodes.at(0).x == 0.0 && instance.nodes.at(0).y == 0.0,
            "Tọa độ depot phải được giữ lại");

    require(instance.nodes.at(1).internal_index == 1, "Khách đầu tiên phải có chỉ số 1");
    require(instance.nodes.at(1).original_id == "7", "Phải giữ ID nguồn không liên tiếp");
    require(!instance.nodes.at(1).is_depot, "Khách không được đánh dấu là depot");
    require(instance.nodes.at(1).demand == 3, "Demand khách phải được đọc đúng");

    require(instance.nodes.at(2).internal_index == 2, "Khách thứ hai phải có chỉ số 2");
    require(instance.nodes.at(2).original_id == "99", "Phải giữ ID nguồn cuối");
    require(instance.nodes.at(2).demand == 4, "Demand khách thứ hai phải được đọc đúng");
}

void test_rejects_invalid_inputs() {
    const std::string valid = valid_instance_text();

    const std::vector<std::pair<const char*, std::string>> invalid_cases{
        {
            "thiếu depot",
            replace_once(valid, "42\n-1\nEOF\n", "-1\nEOF\n"),
        },
        {
            "nhiều depot",
            replace_once(valid, "42\n-1\nEOF\n", "42\n7\n-1\nEOF\n"),
        },
        {
            "capacity bằng 0",
            replace_once(valid, "CAPACITY : 10", "CAPACITY : 0"),
        },
        {
            "capacity âm",
            replace_once(valid, "CAPACITY : 10", "CAPACITY : -1"),
        },
        {
            "demand âm",
            replace_once(valid, "7 3\n42 0\n99 4\nDEPOT_SECTION",
                         "7 -1\n42 0\n99 4\nDEPOT_SECTION"),
        },
        {
            "demand vượt capacity",
            replace_once(valid, "7 3\n42 0\n99 4\nDEPOT_SECTION",
                         "7 3\n42 0\n99 11\nDEPOT_SECTION"),
        },
        {
            "thiếu tọa độ",
            replace_once(valid, "99 6 4\nDEMAND_SECTION", "DEMAND_SECTION"),
        },
        {
            "trùng tọa độ",
            replace_once(valid, "99 6 4\nDEMAND_SECTION", "7 6 4\nDEMAND_SECTION"),
        },
        {
            "thiếu demand",
            replace_once(valid, "99 4\nDEPOT_SECTION", "DEPOT_SECTION"),
        },
        {
            "trùng demand",
            replace_once(valid, "99 4\nDEPOT_SECTION", "7 4\nDEPOT_SECTION"),
        },
        {
            "dimension không khớp",
            replace_once(valid, "DIMENSION : 3", "DIMENSION : 4"),
        },
        {
            "thiếu marker EOF",
            replace_once(valid, "EOF\n", ""),
        },
        {
            "dimension vượt giới hạn số khách",
            replace_once(valid, "DIMENSION : 3", "DIMENSION : 101"),
        },
        {
            "TYPE không hỗ trợ",
            replace_once(valid, "TYPE : CVRP", "TYPE : TSP"),
        },
        {
            "EDGE_WEIGHT_TYPE không hỗ trợ",
            replace_once(valid, "EDGE_WEIGHT_TYPE : EUC_2D",
                         "EDGE_WEIGHT_TYPE : MAN_2D"),
        },
    };

    for (const auto& [case_name, input] : invalid_cases) {
        require_parse_error(input, case_name);
    }
}

void test_rejects_stream_read_error() {
    std::istringstream input(valid_instance_text());
    input.setstate(std::ios::badbit);

    try {
        static_cast<void>(routecraft::parse_cvrplib(input));
    } catch (const ParseError&) {
        return;
    }

    throw std::runtime_error("Parser phải từ chối stream gặp lỗi đọc");
}

struct TestCase {
    const char* name;
    void (*run)();
};

}

int main() {
    const std::vector<TestCase> test_cases{
        {
            "doc_instance_hop_le_va_chuan_hoa_depot",
            test_parses_valid_instance_and_normalizes_depot,
        },
        {
            "tu_choi_du_lieu_khong_hop_le",
            test_rejects_invalid_inputs,
        },
        {
            "tu_choi_loi_doc_stream",
            test_rejects_stream_read_error,
        },
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
