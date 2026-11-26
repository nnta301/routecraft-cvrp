#pragma once

#include "routecraft/cvrp.hpp"

#include <functional>

namespace routecraft {

// Điểm dùng để xếp thứ tự một cặp khách (i, j), i < j, theo chỉ số nội bộ.
// Phải trả về số hữu hạn; ngược lại solver ném std::invalid_argument.
using PairScore = std::function<double(NodeIndex, NodeIndex)>;

// Tham số W1:
// score = d(0,i) + d(0,j) - lambda*d(i,j) + mu*|d(0,i) - d(0,j)| + nu*(q_i + q_j)/q_trung_binh
// (lambda, mu, nu) = (1, 0, 0) tương đương W0.
struct ClarkeWrightParameters {
    double lambda{1.0};
    double mu{0.0};
    double nu{0.0};
};

struct ClarkeWrightResult {
    Solution solution;
    double construction_time_ms{};
};

// Clarke-Wright parallel với hàm điểm tùy ý. Chỉ các cặp có savings thật
// s0(i,j) = d(0,i) + d(0,j) - d(i,j) > 0 là ứng viên; thủ tục ghép cố định,
// chỉ thứ tự ưu tiên lấy từ `score`.
ClarkeWrightResult solve_clarke_wright(const Instance& instance, const PairScore& score);

// W0: score = savings gốc s0.
ClarkeWrightResult solve_clarke_wright_w0(const Instance& instance);

// W1: savings tham số. Ném std::invalid_argument nếu tham số không hữu hạn.
ClarkeWrightResult solve_clarke_wright_w1(
    const Instance& instance,
    const ClarkeWrightParameters& parameters);

}
