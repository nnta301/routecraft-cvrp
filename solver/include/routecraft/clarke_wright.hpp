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

// Một trục của lưới: giá trị thứ k = start + k*step, k = 0..count-1,
// count = llround((stop - start)/step) + 1.
struct ParameterRange {
    double start{};
    double stop{};
    double step{};
};

// Mặc định là lưới G của giao thức thực nghiệm: 20 x 21 x 21 = 8.820 bộ.
struct ClarkeWrightGrid {
    ParameterRange lambda{0.1, 2.0, 0.1};
    ParameterRange mu{0.0, 2.0, 0.1};
    ParameterRange nu{0.0, 2.0, 0.1};
};

struct ClarkeWrightGridResult {
    Solution solution;
    ClarkeWrightParameters best_parameters;
    std::size_t configurations{};
    Distance total_distance{};
    double construction_time_ms{};
};

// Ném std::invalid_argument nếu lưới không hợp lệ (xem solve_clarke_wright_w1_grid).
void validate_clarke_wright_grid(const ClarkeWrightGrid& grid);

// Chạy W1 với mọi bộ trong lưới (lambda ngoài cùng, rồi mu, rồi nu; đều tăng dần).
// Chọn quãng đường nhỏ nhất; hòa thì ít tuyến hơn; hòa nữa thì bộ đứng trước.
// Ném std::invalid_argument nếu lưới không hợp lệ (step <= 0, không hữu hạn,
// start > stop, hoặc quá 1.000.000 bộ).
ClarkeWrightGridResult solve_clarke_wright_w1_grid(
    const Instance& instance,
    const ClarkeWrightGrid& grid = {});

}
