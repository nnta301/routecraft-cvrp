# Sprint 1 — CVRP Baseline CLI

## Trạng thái

Đã đóng. Các hạng mục baseline đã hoàn thành; một số việc được chuyển sang tồn đọng (xem bên dưới).

## Thay đổi phạm vi

Sau buổi trình bày đầu tiên, giảng viên hướng dẫn đề nghị tìm hướng nghiên cứu mới thay vì chỉ cài đặt các thuật toán đã có. Sprint 2 sẽ được lập kế hoạch sau khi chốt hướng với giảng viên.

## Mục tiêu

Hoàn thiện baseline CVRP có thể chạy độc lập từ CLI, đọc instance `.vrp`, tạo nghiệm Nearest Neighbor hợp lệ, kiểm tra độc lập và xuất kết quả TXT/JSON.

## Phạm vi

- Mô hình dữ liệu CVRP và quy ước depot nội bộ là `0`.
- Khoảng cách `EUC_2D`, làm tròn từng cạnh.
- Parser `.vrp` cho phạm vi định dạng được hỗ trợ.
- Nearest Neighbor.
- Validator tính lại độc lập: đủ/không trùng khách, đúng depot, không quá tải và đúng tổng khoảng cách.
- CLI `solve`, xuất TXT và JSON.
- Dữ liệu toy cùng kiểm thử tự động.

## Ngoài phạm vi

- Clarke–Wright, 2-opt, relocate.
- FastAPI, React, Docker, deploy.
- ML, ACO, GA, Tabu Search và các biến thể VRP ngoài phạm vi.

## Các issue đã cam kết

Các Issue đã cam kết cho Sprint 1:

- [#1 — CVRP domain model and EUC_2D distance utilities](https://github.com/nnta301/routecraft-cvrp/issues/1)
- [#2 — Parse supported CVRPLIB .vrp instances](https://github.com/nnta301/routecraft-cvrp/issues/2)
- [#3 — Implement deterministic capacity-aware Nearest Neighbor](https://github.com/nnta301/routecraft-cvrp/issues/3)
- [#4 — Add independent CVRP solution validator](https://github.com/nnta301/routecraft-cvrp/issues/4)
- [#5 — Add solve command and TXT/JSON result export](https://github.com/nnta301/routecraft-cvrp/issues/5)
- [#6 — Add Sprint 1 toy fixtures and end-to-end regression coverage](https://github.com/nnta301/routecraft-cvrp/issues/6)

## Đã hoàn thành

- Có scaffold CMake C++17 với static library `routecraft_core` và executable `routecraft` liên kết với core.
- Hoàn thành [#1 — CVRP domain model and EUC_2D distance utilities](https://github.com/nnta301/routecraft-cvrp/issues/1): mô hình dữ liệu CVRP dùng chung, quy ước depot nội bộ `0`, lưu `original_id`, tiện ích `EUC_2D`, tính tải/độ dài tuyến và tổng độ dài nghiệm.
- Có unit test CTest cho làm tròn từng cạnh, cạnh quay về depot, tải tuyến, tổng độ dài nghiệm và ánh xạ ID gốc.
- CLI hỗ trợ `--help`, `-h`, `--version`.
- Có smoke test CTest cho các hành vi CLI tối thiểu.
- Workflow GitHub Actions `Build and test` kiểm tra build và toàn bộ CTest đã đăng ký trên cả cấu hình `Debug` và `Release`.
- Hoàn thành [#2 — Parse supported CVRPLIB .vrp instances](https://github.com/nnta301/routecraft-cvrp/issues/2): parser hỗ trợ tập CVRPLIB của Sprint 1, kiểm tra dữ liệu đầu vào, chuẩn hóa depot nội bộ `0` và giữ ID nguồn.
- Hoàn thành [#3 — Implement deterministic capacity-aware Nearest Neighbor](https://github.com/nnta301/routecraft-cvrp/issues/3): bổ sung heuristic Nearest Neighbor có xét tải trọng trong lõi solver; dùng chỉ số nội bộ, đóng mỗi tuyến tại depot `0`, phá hòa theo `internal_index` và đo riêng thời gian dựng nghiệm.
- Có unit test CTest cho một khách, tuyến vừa đầy tải, nhiều tuyến, phục vụ mỗi khách đúng một lần và trường hợp đồng khoảng cách.

- Hoàn thành [#4 — Add independent CVRP solution validator](https://github.com/nnta301/routecraft-cvrp/issues/4): validator tính lại tập khách, depot, tải, khoảng cách và tổng khoảng cách chỉ từ instance và routes (commit `1d38eea`).
- Hoàn thành [#5 — Add solve command and TXT/JSON result export](https://github.com/nnta301/routecraft-cvrp/issues/5): lệnh `routecraft solve --input <file.vrp> --method nn --format json|txt`, chỉ xuất kết quả khi validator xác nhận nghiệm (commit `ffe319c`).
- Hoàn thành [#6 — Add Sprint 1 toy fixtures and end-to-end regression coverage](https://github.com/nnta301/routecraft-cvrp/issues/6): fixture trong `solver/tests/fixtures/` và kiểm thử e2e CLI (commit `7845677`).
- Kết quả chạy mẫu trên `sprint1-toy.vrp`: `total_distance` là 34, gồm 3 tuyến.

## Lỗi phát hiện khi review cuối sprint

Review cuối sprint ([báo cáo](../research/review-2026-11-26.md)) phát hiện hai lỗi, được sửa trong Sprint 1 bằng hai issue riêng:

- Validator chấp nhận tuyến không có khách: [#18](https://github.com/nnta301/routecraft-cvrp/issues/18).
- Test fail khi chạy từ Git Bash trên Windows do nạp nhầm DLL runtime MinGW: [#19](https://github.com/nnta301/routecraft-cvrp/issues/19).

## Chưa hoàn thành / chuyển tiếp

Các việc sau chuyển sang tồn đọng:

- [#21](https://github.com/nnta301/routecraft-cvrp/issues/21): BOM UTF-8 trong parser.
- [#22](https://github.com/nnta301/routecraft-cvrp/issues/22): `routecraft solve --help` trả mã lỗi thay vì in hướng dẫn.
- [#23](https://github.com/nnta301/routecraft-cvrp/issues/23): Ma trận khoảng cách dùng chung và truy cập node O(1).
- [#24](https://github.com/nnta301/routecraft-cvrp/issues/24): Kiểm thử chặt hơn: parse JSON thay vì so khớp chuỗi con, kiểm tra tuyến và cost trong e2e, thêm ca lỗi parser.
- [#25](https://github.com/nnta301/routecraft-cvrp/issues/25): CI trên Windows, sanitizer, coi warning là lỗi.
- [#26](https://github.com/nnta301/routecraft-cvrp/issues/26): Giao thức thực nghiệm: công thức gap, nguồn BKS, quy ước số xe.

## Bằng chứng

Bằng chứng hiện có trong repository: `CMakeLists.txt`, `solver/CMakeLists.txt`, `solver/cli/main.cpp` và `solver/tests/`. [GitHub Actions run 38596237542](https://github.com/nnta301/routecraft-cvrp/actions/runs/38596237542) đã chạy thành công cho scaffold ban đầu ở cả `Debug` và `Release`.

Bằng chứng cho Issue #1 gồm `solver/include/routecraft/cvrp.hpp`, `solver/include/routecraft/distance.hpp`, `solver/src/distance.cpp`, `solver/tests/cvrp_core_tests.cpp` và các kiểm tra Debug/Release của PR liên kết Issue #1.

Bằng chứng cho Issue #2 gồm `solver/include/routecraft/vrp_parser.hpp`, `solver/src/vrp_parser.cpp`, `solver/tests/vrp_parser_tests.cpp` và kết quả kiểm tra Debug/Release của PR liên kết Issue #2.

Bằng chứng cho Issue #3 gồm `solver/include/routecraft/nearest_neighbor.hpp`, `solver/src/nearest_neighbor.cpp`, `solver/tests/nearest_neighbor_tests.cpp` và kết quả kiểm tra Debug/Release của PR liên kết Issue #3.

Bằng chứng cho Issue #4, #5 và #6 là các commit `1d38eea` (validator độc lập), `ffe319c` (lệnh `solve` và xuất kết quả) và `7845677` (fixture và kiểm thử e2e CLI trong `solver/tests/fixtures/`) trên `main`.

## Tổng kết

Sprint 1 đã đóng. Retrospective ngắn:

### Điều làm tốt

Đạt được parser, Nearest Neighbor, validator, CLI `solve` xuất JSON/TXT, kiểm thử e2e và CI Debug/Release.

### Trở ngại hoặc khó khăn

- Tài liệu sprint không được cập nhật cùng PR tính năng.
- Chưa chạy test trong môi trường dev Windows (Git Bash).

### Bài học

- Cập nhật tài liệu sprint ngay trong PR tính năng.
- Chạy test cả trên máy dev Windows.
