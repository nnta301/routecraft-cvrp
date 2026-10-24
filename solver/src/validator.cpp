#include "routecraft/validator.hpp"

#include "routecraft/distance.hpp"

#include <stdexcept>
#include <limits>
#include <vector>

namespace routecraft {
namespace {

// Giữ tổng tại biên khi vượt miền biểu diễn; không thực hiện phép cộng gây tràn.
bool checked_add(std::int64_t& total, std::int64_t value) {
    const auto maximum = std::numeric_limits<std::int64_t>::max();
    const auto minimum = std::numeric_limits<std::int64_t>::min();
    if (value > 0 && total > maximum - value) {
        total = maximum;
        return false;
    }
    if (value < 0 && total < minimum - value) {
        total = minimum;
        return false;
    }
    total += value;
    return true;
}

void add_error(ValidationResult& result, const std::string& error) {
    result.errors.push_back(error);
}

const Node* find_node(const Instance& instance, NodeIndex node_index) {
    try {
        return &node_by_internal_index(instance, node_index);
    } catch (const std::out_of_range&) {
        return nullptr;
    } catch (const std::invalid_argument&) {
        return nullptr;
    }
}

std::string route_label(std::size_t route_index) {
    return "Tuyến " + std::to_string(route_index + 1);
}

}

ValidationResult validate_solution(const Instance& instance, const Solution& solution) {
    ValidationResult result;
    bool total_distance_valid = true;
    std::vector<std::size_t> customer_visits(instance.nodes.size(), 0);

    for (std::size_t route_index = 0; route_index < solution.routes.size(); ++route_index) {
        const Route& route = solution.routes.at(route_index);
        const std::string label = route_label(route_index);
        Demand route_load = 0;
        Distance route_distance = 0;
        bool contains_invalid_index = false;
        bool load_valid = true;
        bool distance_valid = true;
        std::vector<const Node*> resolved_stops;
        resolved_stops.reserve(route.stops.size());

        if (route.stops.empty()) {
            add_error(result, label + " rỗng, không bắt đầu và kết thúc tại depot 0");
        } else {
            if (route.stops.front() != 0) {
                add_error(result, label + " không bắt đầu tại depot 0");
            }
            if (route.stops.back() != 0) {
                add_error(result, label + " không kết thúc tại depot 0");
            }
        }

        for (std::size_t position = 0; position < route.stops.size(); ++position) {
            const NodeIndex node_index = route.stops.at(position);
            const Node* node = find_node(instance, node_index);

            if (node == nullptr) {
                add_error(
                    result,
                    label + " chứa chỉ số khách không hợp lệ " +
                        std::to_string(node_index) + " tại vị trí " +
                        std::to_string(position + 1));
                contains_invalid_index = true;
                resolved_stops.push_back(nullptr);
                continue;
            }

            resolved_stops.push_back(node);
            if (node_index == 0) {
                if (position != 0 && position + 1 != route.stops.size()) {
                    add_error(
                        result,
                        label + " có depot 0 ở vị trí không hợp lệ " +
                            std::to_string(position + 1));
                }
                continue;
            }

            if (load_valid && !checked_add(route_load, node->demand)) {
                load_valid = false;
                add_error(result, "Tải tuyến " + std::to_string(route_index + 1) + " vượt miền biểu diễn int64_t");
            }
            if (node_index < customer_visits.size()) {
                ++customer_visits.at(node_index);
            }
        }

        if (route_load > instance.capacity) {
            add_error(
                result,
                label + " vượt tải: " + std::to_string(route_load) +
                    " > " + std::to_string(instance.capacity));
        }

        if (!contains_invalid_index) {
            for (std::size_t position = 1; position < resolved_stops.size(); ++position) {
                const auto edge_distance = checked_euclidean_distance(
                    *resolved_stops.at(position - 1),
                    *resolved_stops.at(position));
                if (!edge_distance.has_value() ||
                    !checked_add(route_distance, edge_distance.value())) {
                    route_distance = std::numeric_limits<Distance>::max();
                    distance_valid = false;
                    add_error(result, "Khoảng cách tuyến " + std::to_string(route_index + 1) + " vượt miền biểu diễn int64_t");
                    break;
                }
            }
        }

        result.route_loads.push_back(route_load);
        result.route_distances.push_back(route_distance);
        if (!distance_valid) {
            total_distance_valid = false;
            result.total_distance = std::numeric_limits<Distance>::max();
        } else if (total_distance_valid && !checked_add(result.total_distance, route_distance)) {
            total_distance_valid = false;
            add_error(result, "Tổng khoảng cách vượt miền biểu diễn int64_t");
        }
    }

    for (const Node& node : instance.nodes) {
        if (node.internal_index == 0) {
            continue;
        }

        const std::size_t visit_count =
            node.internal_index < customer_visits.size()
                ? customer_visits.at(node.internal_index)
                : 0;
        if (visit_count == 0) {
            add_error(
                result,
                "Khách " + node.original_id + " (chỉ số " +
                    std::to_string(node.internal_index) + ") chưa được phục vụ");
        } else if (visit_count > 1) {
            add_error(
                result,
                "Khách " + node.original_id + " (chỉ số " +
                    std::to_string(node.internal_index) + ") được phục vụ " +
                    std::to_string(visit_count) + " lần");
        }
    }

    if (total_distance_valid && solution.total_distance.has_value() &&
        solution.total_distance.value() != result.total_distance) {
        add_error(
            result,
            "Tổng khoảng cách khai báo " +
                std::to_string(solution.total_distance.value()) +
                " không khớp tổng tính lại " + std::to_string(result.total_distance));
    }

    result.feasible = result.errors.empty();
    return result;
}

}
