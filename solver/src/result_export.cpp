#include "routecraft/result_export.hpp"

#include <algorithm>
#include <iomanip>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace routecraft {
namespace {

void require_consistent_result(
    const Solution& solution,
    const ValidationResult& validation) {
    if (!validation.feasible ||
        validation.route_loads.size() != solution.routes.size() ||
        validation.route_distances.size() != solution.routes.size()) {
        throw std::invalid_argument("Không thể xuất nghiệm không khả thi hoặc số liệu không khớp tuyến");
    }
}

std::string escape_json(const std::string& value) {
    std::ostringstream output;
    output.imbue(std::locale::classic());
    for (const unsigned char character : value) {
        switch (character) {
        case '"': output << "\\\""; break;
        case '\\': output << "\\\\"; break;
        case '\b': output << "\\b"; break;
        case '\f': output << "\\f"; break;
        case '\n': output << "\\n"; break;
        case '\r': output << "\\r"; break;
        case '\t': output << "\\t"; break;
        default:
            if (character < 0x20) {
                output << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                       << static_cast<int>(character) << std::dec << std::setfill(' ');
            } else {
                output << character;
            }
        }
    }
    return output.str();
}

void write_timings(std::ostream& output, const SolutionTimings& timings) {
    output << std::fixed << std::setprecision(3)
           << "\"construct\": " << timings.construction_time_ms
           << ", \"improve\": 0.000"
           << ", \"validate\": " << timings.validation_time_ms;
}

}

std::string serialize_solution_json(
    const Instance& instance,
    const Solution& solution,
    const ValidationResult& validation,
    const SolutionTimings& timings) {
    require_consistent_result(solution, validation);
    std::ostringstream output;
    output.imbue(std::locale::classic());
    output << "{\n"
           << "  \"instance\": \"" << escape_json(instance.name) << "\",\n"
           << "  \"method\": \"nn\",\n"
           << "  \"routes\": [";
    for (std::size_t route_index = 0; route_index < solution.routes.size(); ++route_index) {
        if (route_index != 0) output << ", ";
        output << "[";
        const Route& route = solution.routes.at(route_index);
        for (std::size_t stop_index = 0; stop_index < route.stops.size(); ++stop_index) {
            if (stop_index != 0) output << ", ";
            output << route.stops.at(stop_index);
        }
        output << "]";
    }
    output << "],\n  \"route_loads\": [";
    for (std::size_t index = 0; index < validation.route_loads.size(); ++index) {
        if (index != 0) output << ", ";
        output << validation.route_loads.at(index);
    }
    output << "],\n  \"route_distances\": [";
    for (std::size_t index = 0; index < validation.route_distances.size(); ++index) {
        if (index != 0) output << ", ";
        output << validation.route_distances.at(index);
    }
    output << "],\n  \"total_distance\": " << validation.total_distance << ",\n"
           << "  \"num_routes\": " << solution.routes.size() << ",\n"
           << "  \"feasible\": true,\n"
           << "  \"timings_ms\": { ";
    write_timings(output, timings);
    output << " },\n  \"node_ids\": [";
    std::vector<const Node*> nodes;
    nodes.reserve(instance.nodes.size());
    for (const Node& node : instance.nodes) nodes.push_back(&node);
    std::sort(nodes.begin(), nodes.end(), [](const Node* first, const Node* second) {
        return first->internal_index < second->internal_index;
    });
    for (std::size_t index = 0; index < nodes.size(); ++index) {
        if (index != 0) output << ", ";
        output << "{\"internal_index\": " << nodes.at(index)->internal_index
               << ", \"original_id\": \"" << escape_json(nodes.at(index)->original_id)
               << "\"}";
    }
    output << "]\n}\n";
    return output.str();
}

std::string serialize_solution_txt(
    const Instance& instance,
    const Solution& solution,
    const ValidationResult& validation,
    const SolutionTimings& timings) {
    require_consistent_result(solution, validation);
    std::ostringstream output;
    output.imbue(std::locale::classic());
    output << "Instance: " << instance.name << "\n"
           << "Method: nn\n"
           << "Feasible: true\n";
    for (std::size_t route_index = 0; route_index < solution.routes.size(); ++route_index) {
        output << "Route " << route_index + 1 << ": ";
        const Route& route = solution.routes.at(route_index);
        for (std::size_t stop_index = 0; stop_index < route.stops.size(); ++stop_index) {
            if (stop_index != 0) output << " -> ";
            output << node_by_internal_index(instance, route.stops.at(stop_index)).original_id;
        }
        output << " | Load: " << validation.route_loads.at(route_index)
               << " | Distance: " << validation.route_distances.at(route_index) << "\n";
    }
    output << "Total distance: " << validation.total_distance << "\n"
           << std::fixed << std::setprecision(3)
           << "Timings (ms): construct=" << timings.construction_time_ms
           << ", improve=0.000, validate=" << timings.validation_time_ms << "\n";
    return output.str();
}

}
