# Solver

Thành phần này chứa lõi C++ dùng chung và CLI mỏng gọi lõi đó. `routecraft_core` hiện cung cấp mô hình dữ liệu CVRP tối thiểu gồm `Node`, `Instance`, `Route`, `Solution` và `ValidationResult`; depot dùng chỉ số nội bộ `0`, còn `original_id` được giữ để khôi phục ID nguồn.

Lõi cũng cung cấp tiện ích tính khoảng cách `EUC_2D`, tải tuyến, độ dài tuyến và tổng độ dài nghiệm. CLI `routecraft` hiện hỗ trợ `--help`, `-h`, `--version` cùng smoke test CTest. Việc tách core khỏi CLI giúp parser, heuristic, validator và API sau này dùng chung logic.

## CLI `solve`

Lệnh `routecraft solve --input <file.vrp> --method nn --format json|txt` thực hiện parser, Nearest Neighbor, validator và xuất kết quả ra stdout. Sprint 1 chỉ hỗ trợ `nn`. JSON dùng chỉ số nội bộ trong route và có `node_ids` để ánh xạ về ID nguồn; TXT hiển thị ID nguồn cùng tải và khoảng cách từng tuyến. Lỗi và chẩn đoán được ghi ra stderr; CLI chỉ xuất kết quả khi validator xác nhận nghiệm khả thi.

## Validator độc lập

Validator được khai báo tại `routecraft/validator.hpp`. `validate_solution` tính lại tải từng tuyến, khoảng cách từng tuyến và tổng khoảng cách chỉ từ `Instance` cùng `routes`, dùng `EUC_2D` cho từng cạnh. Nó kiểm tra depot đầu/cuối và depot ở giữa tuyến, chỉ số khách, khách thiếu/trùng, quá tải và tổng khoảng cách khai báo. `ValidationResult` chứa `feasible`, lỗi bằng tiếng Việt và các số liệu đã tính lại; các trường tải/khoảng cách khai báo trong `Solution` không được dùng để chấp nhận nghiệm.

Validator dùng `checked_euclidean_distance` trong utility dùng chung để kiểm tra khoảng cách cạnh hữu hạn và nằm trong miền `Distance` trước khi gọi `llround`. Validator cũng kiểm tra tràn số trước khi cộng tải, khoảng cách tuyến và tổng khoảng cách. Nếu vượt miền `int64_t`, nghiệm có `feasible=false` và lỗi tương ứng; số liệu bị tràn (cùng tổng phụ thuộc vào tuyến tràn) được giữ tại biên, không phải giá trị chính xác để xuất nghiệm. Những số liệu khác vẫn được tính độc lập.

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

## Kiểm thử baseline Sprint 1

Từ thư mục gốc repository, cấu hình và chạy CTest ở cả hai cấu hình:

```text
cmake -S . -B build/Debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/Debug
ctest --test-dir build/Debug --output-on-failure

cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/Release
ctest --test-dir build/Release --output-on-failure
```

`routecraft_sprint1_e2e` chạy fixture `.vrp` qua CLI thật, đối chiếu JSON/TXT của toy instance và tính độc lập tải, khoảng cách theo tuyến cùng tổng khoảng cách. Baseline hiện chỉ phục vụ CVRP một depot với `EUC_2D`, `--method nn`, không giới hạn số xe `K` và chưa có pha cải thiện hay benchmark hiệu năng.

Trạng thái: mô hình dữ liệu CVRP, tiện ích khoảng cách, parser `.vrp`, Nearest Neighbor, validator độc lập, CLI `solve`, xuất TXT/JSON và các unit test tương ứng đã có. API vẫn chưa triển khai.
