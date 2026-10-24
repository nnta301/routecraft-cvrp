# Sprint 1 — CVRP Baseline CLI

## Trạng thái

Đang thực hiện.

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
- CLI hỗ trợ `--help`, `-h`, `--version` và `solve --input <file.vrp> --method nn --format json|txt`.
- Có smoke test CTest cho các hành vi CLI tối thiểu.
- Workflow GitHub Actions `Build and test` kiểm tra build và toàn bộ CTest đã đăng ký trên cả cấu hình `Debug` và `Release`.
- Hoàn thành [#2 — Parse supported CVRPLIB .vrp instances](https://github.com/nnta301/routecraft-cvrp/issues/2): parser hỗ trợ tập CVRPLIB của Sprint 1, kiểm tra dữ liệu đầu vào, chuẩn hóa depot nội bộ `0` và giữ ID nguồn.
- Hoàn thành [#3 — Implement deterministic capacity-aware Nearest Neighbor](https://github.com/nnta301/routecraft-cvrp/issues/3): bổ sung heuristic Nearest Neighbor có xét tải trọng trong lõi solver; dùng chỉ số nội bộ, đóng mỗi tuyến tại depot `0`, phá hòa theo `internal_index` và đo riêng thời gian dựng nghiệm.
- Có unit test CTest cho một khách, tuyến vừa đầy tải, nhiều tuyến, phục vụ mỗi khách đúng một lần và trường hợp đồng khoảng cách.
- Hoàn thành [#4 — Add independent CVRP solution validator](https://github.com/nnta301/routecraft-cvrp/issues/4): validator tính lại độc lập tải, khoảng cách và tổng khoảng cách; phát hiện depot/chỉ số/khách/tải/số liệu không hợp lệ.
- Hoàn thành [#5 — Add solve command and TXT/JSON result export](https://github.com/nnta301/routecraft-cvrp/issues/5): CLI thực hiện parser → Nearest Neighbor → validator → xuất JSON/TXT cho nghiệm khả thi.

## Chưa hoàn thành / chuyển tiếp

Trên `main`, hạng mục còn chờ là [#6](https://github.com/nnta301/routecraft-cvrp/issues/6): fixture toy và regression end-to-end đang chờ PR/merge. Không vì vậy mà các thành phần validator, `solve` hoặc xuất TXT/JSON đã merge bị xem là chưa triển khai.

## Bằng chứng

Bằng chứng hiện có trong repository: `CMakeLists.txt`, `solver/CMakeLists.txt`, `solver/cli/main.cpp` và `solver/tests/`. [GitHub Actions run 38596237542](https://github.com/nnta301/routecraft-cvrp/actions/runs/38596237542) đã chạy thành công cho scaffold ban đầu ở cả `Debug` và `Release`.

Bằng chứng cho Issue #1 gồm `solver/include/routecraft/cvrp.hpp`, `solver/include/routecraft/distance.hpp`, `solver/src/distance.cpp`, `solver/tests/cvrp_core_tests.cpp` và các kiểm tra Debug/Release của PR liên kết Issue #1.

Bằng chứng cho Issue #2 gồm `solver/include/routecraft/vrp_parser.hpp`, `solver/src/vrp_parser.cpp`, `solver/tests/vrp_parser_tests.cpp` và kết quả kiểm tra Debug/Release của PR liên kết Issue #2.

Bằng chứng cho Issue #3 gồm `solver/include/routecraft/nearest_neighbor.hpp`, `solver/src/nearest_neighbor.cpp`, `solver/tests/nearest_neighbor_tests.cpp` và kết quả kiểm tra Debug/Release của PR liên kết Issue #3.

Bằng chứng cho Issue #4 gồm `solver/include/routecraft/validator.hpp`, `solver/src/validator.cpp`, `solver/tests/validator_tests.cpp` và kết quả kiểm tra Debug/Release của PR liên kết Issue #4. Bằng chứng cho Issue #5 gồm `solver/cli/main.cpp`, `solver/src/result_export.cpp`, `solver/tests/cli_solve.cmake`, `solver/tests/result_export_tests.cpp` và kết quả kiểm tra Debug/Release của PR liên kết Issue #5.

## Tổng kết

Sprint đang diễn ra; retrospective sẽ được hoàn thiện khi Sprint kết thúc.

### Điều làm tốt

Scaffold tách core/CLI và smoke test đã tạo nền tảng tối thiểu để triển khai baseline.

### Trở ngại hoặc khó khăn

Chưa có retrospective hoàn chỉnh khi Sprint vẫn còn issue #6 chờ được merge.
