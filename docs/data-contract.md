# Hợp đồng dữ liệu v0 (dự kiến)

Đây là quy ước trao đổi dữ liệu dự kiến, **chưa phải API hay JSON Schema đã triển khai**.

## Input instance

JSON dự kiến gồm `instance` (tên/định danh), `capacity` (số nguyên dương), `nodes` (mảng node) và ánh xạ ID. Mỗi node có `internal_index` (số nguyên), `original_id` (ID nguồn, chuỗi hoặc số), `x`, `y` (số thực, đơn vị tọa độ), `demand` (số nguyên không âm) và `is_depot` (boolean). Depot luôn có `internal_index: 0`; chỉ có một depot. Các node còn lại được lập chỉ mục liên tiếp nội bộ.

Thứ tự phần tử trong `nodes` không quyết định định danh node. Mọi `NodeIndex` trong route phải được phân giải qua trường `Node::internal_index`; các chỉ số nội bộ phải duy nhất và liên tiếp từ `0` đến `DIMENSION - 1`.

Parser `.vrp` dự kiến chuyển dữ liệu nguồn sang quy ước này và giữ ánh xạ sang `original_id`. `DIMENSION` bao gồm depot. Input lỗi khi thiếu/nhân bản chỉ số, thiếu depot, capacity không dương, nhu cầu âm, hoặc có khách có `demand > capacity`.

## Output

Output JSON dự kiến có `routes`, `route_loads`, `route_distances`, `total_distance`, `num_routes`, `feasible`, `instance`, `method` và thời gian theo giai đoạn. `routes` dùng **chỉ số nội bộ** và luôn chứa `0` đầu/cuối; ví dụ minh họa (không phải kết quả thực nghiệm):

```json
{
  "instance": "toy-01",
  "method": "nn",
  "routes": [[0, 2, 1, 0]],
  "route_loads": [7],
  "route_distances": [18],
  "total_distance": 18,
  "num_routes": 1,
  "feasible": true,
  "timings_ms": {"construct": 1.2, "improve": 0.0, "validate": 0.1}
}
```

`route_loads`, `route_distances` tương ứng cùng thứ tự với `routes`; khoảng cách là số nguyên đơn vị tọa độ theo `EUC_2D`. `total_distance` là tổng các `route_distances`; `num_routes` là số tuyến; `feasible` phản ánh validator. `timings_ms` là số thực không âm, chỉ đo thời gian solver theo pha và loại trừ đọc/ghi file, khởi động subprocess và mạng. Tên pha có thể mở rộng nhưng phải được mô tả khi dùng.

TXT phục vụ người đọc phải dùng `original_id`, kèm tải và khoảng cách từng tuyến. JSON giữ chỉ số nội bộ để không mơ hồ, cùng ánh xạ ID trong dữ liệu instance/metadata khi cần khôi phục ID nguồn. Lỗi dự kiến trả mã/thông điệp có thể máy đọc được; không được xuất nghiệm khả thi giả khi input không hợp lệ.
