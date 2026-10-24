#pragma once

#include "routecraft/cvrp.hpp"

namespace routecraft {

// Tính lại từ instance hợp lệ và routes; số liệu tràn được giữ tại biên int64_t,
// kèm lỗi và feasible=false. Chỉ dùng số liệu đầy đủ khi không có lỗi tương ứng.
ValidationResult validate_solution(const Instance& instance, const Solution& solution);

}
