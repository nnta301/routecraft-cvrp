# Solver

Thành phần này chứa lõi C++ dùng chung và CLI mỏng gọi lõi đó. `routecraft_core` hiện cung cấp mô hình dữ liệu CVRP tối thiểu gồm `Node`, `Instance`, `Route`, `Solution` và `ValidationResult`; depot dùng chỉ số nội bộ `0`, còn `original_id` được giữ để khôi phục ID nguồn.

Lõi cũng cung cấp tiện ích tính khoảng cách `EUC_2D`, tải tuyến, độ dài tuyến và tổng độ dài nghiệm. CLI `routecraft` hiện hỗ trợ `--help`, `-h`, `--version` cùng smoke test CTest. Việc tách core khỏi CLI giúp parser, heuristic, validator và API sau này dùng chung logic.

## Validator độc lập

Validator được khai báo tại `routecraft/validator.hpp`. `validate_solution` tính lại tải từng tuyến, khoảng cách từng tuyến và tổng khoảng cách chỉ từ `Instance` cùng `routes`, dùng `EUC_2D` cho từng cạnh. Nó kiểm tra depot đầu/cuối và depot ở giữa tuyến, chỉ số khách, khách thiếu/trùng, quá tải và tổng khoảng cách khai báo. `ValidationResult` chứa `feasible`, lỗi bằng tiếng Việt và các số liệu đã tính lại; các trường tải/khoảng cách khai báo trong `Solution` không được dùng để chấp nhận nghiệm.

Validator kiểm tra tràn số trước khi cộng tải, khoảng cách tuyến và tổng khoảng cách. Nếu vượt miền `int64_t`, nghiệm có `feasible=false` và lỗi tương ứng; số liệu bị tràn (cùng tổng phụ thuộc vào tuyến tràn) được giữ tại biên, không phải giá trị chính xác để xuất nghiệm. Những số liệu khác vẫn được tính độc lập.

## Parser `.vrp`

Parser được khai báo tại `routecraft/vrp_parser.hpp`. Parser hỗ trợ tập định dạng CVRPLIB của Sprint 1:

- Một depot.
- `TYPE: CVRP`.
- `EDGE_WEIGHT_TYPE: EUC_2D`.
- `NAME`, `DIMENSION`, `CAPACITY`, `NODE_COORD_SECTION`, `DEMAND_SECTION` và `DEPOT_SECTION`.
- ID nguồn không liên tiếp và depot không nằm đầu danh sách nguồn.
- Tối đa `99` khách hàng (`DIMENSION <= 100`, tính cả depot).

Parser kiểm tra dữ liệu không hợp lệ, bao gồm depot không hợp lệ, capacity không dương, demand âm hoặc vượt capacity, dữ liệu tọa độ/demand thiếu hoặc trùng, `DIMENSION` không khớp và loại bài toán/khoảng cách chưa hỗ trợ.

## Nearest Neighbor

Heuristic được khai báo tại `routecraft/nearest_neighbor.hpp`. `solve_nearest_neighbor` nhận `Instance` đã hợp lệ và trả về nghiệm cùng thời gian dựng nghiệm theo mili-giây.

Heuristic tạo từng tuyến từ depot nội bộ `0`, luôn chọn khách chưa phục vụ gần nhất còn vừa tải. Khi nhiều khách có cùng khoảng cách, thuật toán chọn khách có `internal_index` nhỏ hơn để cùng input luôn tạo cùng nghiệm. Mỗi tuyến được đóng tại depot `0`; số tuyến là kết quả của thuật toán, không được ấn định trước.

Heuristic không thay thế validator độc lập và không thực hiện Clarke–Wright, 2-opt hoặc relocate.

Trạng thái: mô hình dữ liệu CVRP, tiện ích khoảng cách, parser `.vrp`, Nearest Neighbor, validator độc lập và các unit test tương ứng đã có. Đọc instance từ CLI, xuất nghiệm TXT/JSON và API vẫn chưa triển khai.
