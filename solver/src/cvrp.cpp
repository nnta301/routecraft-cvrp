#include "routecraft/cvrp.hpp"

#include <stdexcept>

namespace routecraft {

const Node& node_by_internal_index(
    const Instance& instance,
    NodeIndex internal_index) {
    const Node* result = nullptr;

    for (const Node& node : instance.nodes) {
        if (node.internal_index != internal_index) {
            continue;
        }

        if (result != nullptr) {
            throw std::invalid_argument(
                "Tồn tại nhiều node cùng internal_index: " +
                std::to_string(internal_index));
        }

        result = &node;
    }

    if (result == nullptr) {
        throw std::out_of_range(
            "Không tìm thấy node có internal_index: " +
            std::to_string(internal_index));
    }

    return *result;
}

}
