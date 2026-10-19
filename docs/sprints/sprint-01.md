# Sprint 1 — CVRP Baseline CLI

## Status

In progress.

## Goal

Hoàn thiện baseline CVRP có thể chạy độc lập từ CLI, đọc instance `.vrp`, tạo nghiệm Nearest Neighbor hợp lệ, kiểm tra độc lập và xuất kết quả TXT/JSON.

## Scope

- Mô hình dữ liệu CVRP và quy ước depot nội bộ là `0`.
- Khoảng cách `EUC_2D`, làm tròn từng cạnh.
- Parser `.vrp` cho phạm vi định dạng được hỗ trợ.
- Nearest Neighbor.
- Validator tính lại độc lập: đủ/không trùng khách, đúng depot, không quá tải và đúng tổng khoảng cách.
- CLI `solve`, xuất TXT và JSON.
- Dữ liệu toy cùng kiểm thử tự động.

## Out of scope

- Clarke–Wright, 2-opt, relocate.
- FastAPI, React, Docker, deploy.
- ML, ACO, GA, Tabu Search và các biến thể VRP ngoài phạm vi.

## Planned issues

Các Issue đã cam kết cho Sprint 1:

- [#1 — CVRP domain model and EUC_2D distance utilities](https://github.com/nnta301/routecraft-cvrp/issues/1)
- [#2 — Parse supported CVRPLIB .vrp instances](https://github.com/nnta301/routecraft-cvrp/issues/2)
- [#3 — Implement deterministic capacity-aware Nearest Neighbor](https://github.com/nnta301/routecraft-cvrp/issues/3)
- [#4 — Add independent CVRP solution validator](https://github.com/nnta301/routecraft-cvrp/issues/4)
- [#5 — Add solve command and TXT/JSON result export](https://github.com/nnta301/routecraft-cvrp/issues/5)
- [#6 — Add Sprint 1 toy fixtures and end-to-end regression coverage](https://github.com/nnta301/routecraft-cvrp/issues/6)

## Completed

- Có scaffold CMake C++17 với static library `routecraft_core` và executable `routecraft` liên kết với core.
- Hoàn thành [#1 — CVRP domain model and EUC_2D distance utilities](https://github.com/nnta301/routecraft-cvrp/issues/1): mô hình dữ liệu CVRP dùng chung, quy ước depot nội bộ `0`, lưu `original_id`, tiện ích `EUC_2D`, tính tải/độ dài tuyến và tổng độ dài nghiệm.
- Có unit test CTest cho làm tròn từng cạnh, cạnh quay về depot, tải tuyến, tổng độ dài nghiệm và ánh xạ ID gốc.
- CLI hỗ trợ `--help`, `-h`, `--version`; lệnh `solve` được từ chối vì chưa triển khai.
- Có smoke test CTest cho các hành vi CLI tối thiểu.
- Workflow GitHub Actions `Build and test` đã chạy thành công trên GitHub cho cả cấu hình `Debug` và `Release`, chỉ xác minh scaffold build được và các CTest hiện có pass.

## Not completed / carried over

Các hạng mục baseline còn lại chưa hoàn thành: parser `.vrp`, Nearest Neighbor, validator độc lập, lệnh `solve`, xuất TXT/JSON, dữ liệu toy và các kiểm thử tích hợp tương ứng.

## Evidence

Evidence hiện có trong repository: `CMakeLists.txt`, `solver/CMakeLists.txt`, `solver/cli/main.cpp` và `solver/tests/`. [GitHub Actions run 38596237542](https://github.com/nnta301/routecraft-cvrp/actions/runs/38596237542) đã chạy thành công cho scaffold ban đầu ở cả `Debug` và `Release`.

Evidence cho Issue #1 gồm `solver/include/routecraft/cvrp.hpp`, `solver/include/routecraft/distance.hpp`, `solver/src/distance.cpp`, `solver/tests/cvrp_core_tests.cpp` và các kiểm tra Debug/Release của PR liên kết Issue #1.

## Retrospective

Sprint đang diễn ra; retrospective sẽ được hoàn thiện khi Sprint kết thúc.

### What went well

Scaffold tách core/CLI và smoke test đã tạo nền tảng tối thiểu để triển khai baseline.

### What was blocked or difficult

Chưa có thông tin retrospective hoàn chỉnh khi các hạng mục baseline chưa được triển khai.