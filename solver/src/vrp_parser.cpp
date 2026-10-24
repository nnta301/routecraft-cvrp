#include "routecraft/vrp_parser.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <system_error>
#include <unordered_map>
#include <utility>
#include <vector>

namespace routecraft {
namespace {

struct Coordinates {
    double x{};
    double y{};
};

enum class Section {
    header,
    node_coordinates,
    demands,
    depot,
    finished,
};

struct Metadata {
    std::optional<std::string> name;
    std::optional<std::string> type;
    std::optional<std::size_t> dimension;
    std::optional<Demand> capacity;
    std::optional<std::string> edge_weight_type;
};

using CoordinateMap = std::unordered_map<std::string, Coordinates>;
using DemandMap = std::unordered_map<std::string, Demand>;

constexpr std::size_t max_supported_node_count = 100;

[[noreturn]] void fail(const std::string& message) {
    throw ParseError("Dữ liệu CVRPLIB không hợp lệ: " + message);
}

[[noreturn]] void fail(std::size_t line_number, const std::string& message) {
    fail("dòng " + std::to_string(line_number) + ": " + message);
}

std::string trim(std::string value) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }

    const std::size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::string uppercase(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::toupper(character));
        });

    return value;
}

Demand parse_demand_value(
    const std::string& text,
    std::size_t line_number,
    const std::string& field_name) {
    Demand value = 0;
    const char* begin = text.data();
    const char* end = begin + text.size();
    const auto conversion = std::from_chars(begin, end, value);

    if (conversion.ec != std::errc{} || conversion.ptr != end) {
        fail(line_number, field_name + " phải là số nguyên hợp lệ");
    }

    return value;
}

std::size_t parse_dimension(const std::string& text, std::size_t line_number) {
    const Demand value = parse_demand_value(text, line_number, "DIMENSION");

    if (value <= 0) {
        fail(line_number, "DIMENSION phải lớn hơn 0");
    }

    if (static_cast<std::uintmax_t>(value) >
        static_cast<std::uintmax_t>(std::numeric_limits<std::size_t>::max())) {
        fail(line_number, "DIMENSION vượt quá giới hạn hỗ trợ");
    }

    if (static_cast<std::uintmax_t>(value) > max_supported_node_count) {
        fail(line_number, "parser chỉ hỗ trợ tối đa 99 khách hàng (DIMENSION tối đa 100)");
    }

    return static_cast<std::size_t>(value);
}

double parse_coordinate_value(
    const std::string& text,
    std::size_t line_number,
    const std::string& coordinate_name) {
    try {
        std::size_t parsed_length = 0;
        const double value = std::stod(text, &parsed_length);

        if (parsed_length != text.size() || !std::isfinite(value)) {
            fail(line_number, coordinate_name + " phải là số thực hữu hạn hợp lệ");
        }

        return value;
    } catch (const std::exception&) {
        fail(line_number, coordinate_name + " phải là số thực hữu hạn hợp lệ");
    }
}

std::pair<std::string, std::string> split_header(
    const std::string& line,
    std::size_t line_number) {
    const std::size_t colon_position = line.find(':');

    if (colon_position != std::string::npos) {
        const std::string key = uppercase(trim(line.substr(0, colon_position)));
        const std::string value = trim(line.substr(colon_position + 1));

        if (key.empty() || value.empty()) {
            fail(line_number, "header phải có cả tên và giá trị");
        }

        return {key, value};
    }

    std::istringstream stream(line);
    std::string key;
    stream >> key;

    std::string value;
    std::getline(stream, value);
    value = trim(value);

    if (key.empty() || value.empty()) {
        fail(line_number, "header phải có cả tên và giá trị");
    }

    return {uppercase(key), value};
}

void read_header(
    Metadata& metadata,
    const std::string& line,
    std::size_t line_number) {
    const auto [key, value] = split_header(line, line_number);

    if (key == "NAME") {
        if (metadata.name.has_value()) {
            fail(line_number, "NAME xuất hiện nhiều lần");
        }
        metadata.name = value;
        return;
    }

    if (key == "TYPE") {
        if (metadata.type.has_value()) {
            fail(line_number, "TYPE xuất hiện nhiều lần");
        }
        metadata.type = value;
        return;
    }

    if (key == "DIMENSION") {
        if (metadata.dimension.has_value()) {
            fail(line_number, "DIMENSION xuất hiện nhiều lần");
        }
        metadata.dimension = parse_dimension(value, line_number);
        return;
    }

    if (key == "CAPACITY") {
        if (metadata.capacity.has_value()) {
            fail(line_number, "CAPACITY xuất hiện nhiều lần");
        }

        const Demand capacity = parse_demand_value(value, line_number, "CAPACITY");
        if (capacity <= 0) {
            fail(line_number, "CAPACITY phải lớn hơn 0");
        }

        metadata.capacity = capacity;
        return;
    }

    if (key == "EDGE_WEIGHT_TYPE") {
        if (metadata.edge_weight_type.has_value()) {
            fail(line_number, "EDGE_WEIGHT_TYPE xuất hiện nhiều lần");
        }
        metadata.edge_weight_type = value;
    }
}

std::pair<std::string, Coordinates> read_coordinate(
    const std::string& line,
    std::size_t line_number) {
    std::istringstream stream(line);
    std::string source_id;
    std::string x_text;
    std::string y_text;
    std::string extra;

    if (!(stream >> source_id >> x_text >> y_text) || (stream >> extra)) {
        fail(line_number, "dòng NODE_COORD_SECTION phải có dạng: id x y");
    }

    return {
        source_id,
        Coordinates{
            parse_coordinate_value(x_text, line_number, "tọa độ x"),
            parse_coordinate_value(y_text, line_number, "tọa độ y"),
        },
    };
}

std::pair<std::string, Demand> read_demand(
    const std::string& line,
    std::size_t line_number) {
    std::istringstream stream(line);
    std::string source_id;
    std::string demand_text;
    std::string extra;

    if (!(stream >> source_id >> demand_text) || (stream >> extra)) {
        fail(line_number, "dòng DEMAND_SECTION phải có dạng: id demand");
    }

    const Demand demand = parse_demand_value(demand_text, line_number, "demand");
    if (demand < 0) {
        fail(line_number, "demand không được âm");
    }

    return {source_id, demand};
}

std::string read_depot_id(const std::string& line, std::size_t line_number) {
    std::istringstream stream(line);
    std::string source_id;
    std::string extra;

    if (!(stream >> source_id) || (stream >> extra)) {
        fail(line_number, "dòng DEPOT_SECTION phải chỉ chứa một ID depot hoặc -1");
    }

    return source_id;
}

void require_required_headers(const Metadata& metadata) {
    if (!metadata.name.has_value()) {
        fail("thiếu header NAME");
    }
    if (!metadata.type.has_value()) {
        fail("thiếu header TYPE");
    }
    if (!metadata.dimension.has_value()) {
        fail("thiếu header DIMENSION");
    }
    if (!metadata.capacity.has_value()) {
        fail("thiếu header CAPACITY");
    }
    if (!metadata.edge_weight_type.has_value()) {
        fail("thiếu header EDGE_WEIGHT_TYPE");
    }

    if (uppercase(trim(*metadata.type)) != "CVRP") {
        fail("chỉ hỗ trợ TYPE: CVRP");
    }

    if (uppercase(trim(*metadata.edge_weight_type)) != "EUC_2D") {
        fail("chỉ hỗ trợ EDGE_WEIGHT_TYPE: EUC_2D");
    }
}

}

ParseError::ParseError(const std::string& message)
    : std::runtime_error(message) {}

Instance parse_cvrplib(std::istream& input) {
    Metadata metadata;
    CoordinateMap coordinates;
    DemandMap demands;
    std::vector<std::string> coordinate_order;
    std::vector<std::string> depot_ids;

    Section section = Section::header;
    bool saw_node_coordinate_section = false;
    bool saw_demand_section = false;
    bool saw_depot_section = false;
    bool depot_section_terminated = false;
    bool saw_eof = false;

    std::string line;
    std::size_t line_number = 0;

    while (std::getline(input, line)) {
        ++line_number;
        const std::string trimmed_line = trim(line);

        if (trimmed_line.empty()) {
            continue;
        }

        if (section == Section::finished) {
            fail(line_number, "không được có nội dung sau EOF");
        }

        const std::string uppercase_line = uppercase(trimmed_line);

        if (uppercase_line == "EOF") {
            saw_eof = true;
            section = Section::finished;
            continue;
        }

        if (uppercase_line == "NODE_COORD_SECTION") {
            if (saw_node_coordinate_section) {
                fail(line_number, "NODE_COORD_SECTION xuất hiện nhiều lần");
            }

            saw_node_coordinate_section = true;
            section = Section::node_coordinates;
            continue;
        }

        if (uppercase_line == "DEMAND_SECTION") {
            if (saw_demand_section) {
                fail(line_number, "DEMAND_SECTION xuất hiện nhiều lần");
            }

            saw_demand_section = true;
            section = Section::demands;
            continue;
        }

        if (uppercase_line == "DEPOT_SECTION") {
            if (saw_depot_section) {
                fail(line_number, "DEPOT_SECTION xuất hiện nhiều lần");
            }

            saw_depot_section = true;
            section = Section::depot;
            continue;
        }

        if (section == Section::header) {
            read_header(metadata, trimmed_line, line_number);
            continue;
        }

        if (section == Section::node_coordinates) {
            const auto [source_id, coordinate] = read_coordinate(trimmed_line, line_number);
            const auto insertion = coordinates.emplace(source_id, coordinate);

            if (!insertion.second) {
                fail(line_number, "trùng tọa độ cho node " + source_id);
            }

            coordinate_order.push_back(source_id);
            continue;
        }

        if (section == Section::demands) {
            const auto [source_id, demand] = read_demand(trimmed_line, line_number);
            const auto insertion = demands.emplace(source_id, demand);

            if (!insertion.second) {
                fail(line_number, "trùng demand cho node " + source_id);
            }

            continue;
        }

        if (depot_section_terminated) {
            fail(line_number, "DEPOT_SECTION chỉ được kết thúc bằng -1");
        }

        if (trimmed_line == "-1") {
            depot_section_terminated = true;
            continue;
        }

        depot_ids.push_back(read_depot_id(trimmed_line, line_number));
        if (depot_ids.size() > 1) {
            fail(line_number, "chỉ hỗ trợ đúng một depot");
        }
    }

    if (input.bad() || !input.eof()) {
        fail("lỗi khi đọc dữ liệu đầu vào");
    }

    if (!saw_eof) {
        fail("thiếu marker EOF");
    }

    require_required_headers(metadata);

    if (!saw_node_coordinate_section) {
        fail("thiếu NODE_COORD_SECTION");
    }
    if (!saw_demand_section) {
        fail("thiếu DEMAND_SECTION");
    }
    if (!saw_depot_section) {
        fail("thiếu DEPOT_SECTION");
    }
    if (!depot_section_terminated) {
        fail("DEPOT_SECTION phải kết thúc bằng -1");
    }
    if (depot_ids.size() != 1) {
        fail("phải khai báo đúng một depot");
    }

    const std::size_t dimension = *metadata.dimension;
    if (coordinates.size() != dimension) {
        fail("DIMENSION không khớp số node trong NODE_COORD_SECTION");
    }
    if (demands.size() != dimension) {
        fail("DIMENSION không khớp số node trong DEMAND_SECTION");
    }

    for (const auto& [source_id, coordinate] : coordinates) {
        static_cast<void>(coordinate);

        if (demands.find(source_id) == demands.end()) {
            fail("thiếu demand cho node " + source_id);
        }
    }

    for (const auto& [source_id, demand] : demands) {
        static_cast<void>(demand);

        if (coordinates.find(source_id) == coordinates.end()) {
            fail("thiếu tọa độ cho node " + source_id);
        }
    }

    const std::string& depot_id = depot_ids.front();
    if (coordinates.find(depot_id) == coordinates.end()) {
        fail("depot " + depot_id + " không có tọa độ");
    }
    if (demands.find(depot_id) == demands.end()) {
        fail("depot " + depot_id + " không có demand");
    }

    for (const auto& [source_id, demand] : demands) {
        if (source_id != depot_id && demand > *metadata.capacity) {
            fail("node " + source_id + " có demand vượt CAPACITY");
        }
    }

    std::vector<Node> nodes;
    nodes.reserve(dimension);

    const auto append_node = [&](const std::string& source_id, bool is_depot) {
        const Coordinates& coordinate = coordinates.at(source_id);
        const Demand demand = demands.at(source_id);

        nodes.push_back(Node{
            nodes.size(),
            source_id,
            coordinate.x,
            coordinate.y,
            demand,
            is_depot,
        });
    };

    append_node(depot_id, true);

    for (const std::string& source_id : coordinate_order) {
        if (source_id != depot_id) {
            append_node(source_id, false);
        }
    }

    return Instance{
        *metadata.name,
        *metadata.capacity,
        std::move(nodes),
    };
}

Instance parse_cvrplib_file(const std::filesystem::path& path) {
    std::ifstream input(path);

    if (!input.is_open()) {
        throw ParseError("Không thể mở file .vrp: " + path.string());
    }

    return parse_cvrplib(input);
}

}
