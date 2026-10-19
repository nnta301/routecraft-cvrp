# Solver

Thành phần này chứa lõi C++ dùng chung và CLI mỏng gọi lõi đó. `routecraft_core` hiện cung cấp mô hình dữ liệu CVRP tối thiểu gồm `Node`, `Instance`, `Route`, `Solution` và `ValidationResult`; depot dùng chỉ số nội bộ `0`, còn `original_id` được giữ để khôi phục ID nguồn.

Lõi cũng cung cấp tiện ích tính khoảng cách `EUC_2D`, tải tuyến, độ dài tuyến và tổng độ dài nghiệm. CLI `routecraft` hiện hỗ trợ `--help`, `-h`, `--version` cùng smoke test CTest. Việc tách core khỏi CLI giúp parser, heuristic, validator và API sau này dùng chung logic.

## Parser `.vrp`

Parser được khai báo tại `routecraft/vrp_parser.hpp`. Parser hỗ trợ tập định dạng CVRPLIB của Sprint 1:

- Một depot.
- `TYPE: CVRP`.
- `EDGE_WEIGHT_TYPE: EUC_2D`.
- `NAME`, `DIMENSION`, `CAPACITY`, `NODE_COORD_SECTION`, `DEMAND_SECTION` và `DEPOT_SECTION`.
- ID nguồn không liên tiếp và depot không nằm đầu danh sách nguồn.
- Tối đa `99` khách hàng (`DIMENSION <= 100`, tính cả depot).

Parser kiểm tra dữ liệu không hợp lệ, bao gồm depot không hợp lệ, capacity không dương, demand âm hoặc vượt capacity, dữ liệu tọa độ/demand thiếu hoặc trùng, `DIMENSION` không khớp và loại bài toán/khoảng cách chưa hỗ trợ.

Trạng thái: mô hình dữ liệu CVRP, tiện ích khoảng cách, parser `.vrp` và unit test đã có. Nearest Neighbor, validator độc lập, đọc instance từ CLI, xuất nghiệm TXT/JSON và API vẫn chưa triển khai.
