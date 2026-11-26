# Hợp đồng dữ liệu v0 (dự kiến)

Đây là quy ước trao đổi dữ liệu dự kiến, **chưa phải API hay JSON Schema đã triển khai**.

## Input instance

JSON dự kiến gồm `instance` (tên/định danh), `capacity` (số nguyên dương), `nodes` (mảng node) và ánh xạ ID. Mỗi node có `internal_index` (số nguyên), `original_id` (ID nguồn, chuỗi hoặc số), `x`, `y` (số thực, đơn vị tọa độ), `demand` (số nguyên không âm) và `is_depot` (boolean). Depot luôn có `internal_index: 0`; chỉ có một depot. Các node còn lại được lập chỉ mục liên tiếp nội bộ.

Thứ tự phần tử trong `nodes` không quyết định định danh node. Mọi `NodeIndex` trong route phải được phân giải qua trường `Node::internal_index`; các chỉ số nội bộ phải duy nhất và liên tiếp từ `0` đến `DIMENSION - 1`.

Parser `.vrp` dự kiến chuyển dữ liệu nguồn sang quy ước này và giữ ánh xạ sang `original_id`. `DIMENSION` bao gồm depot. Input lỗi khi thiếu/nhân bản chỉ số, thiếu depot, capacity không dương, nhu cầu âm, hoặc có khách có `demand > capacity`.

## Output

`method` là `nn` hoặc `cw`. Khi `method` là `cw` (Clarke–Wright parallel), JSON có thêm object `parameters` gồm `lambda`, `mu`, `nu` là số hữu hạn (mặc định 1, 0, 0, tương đương savings gốc), ví dụ `"parameters": {"lambda": 1, "mu": 0, "nu": 0}`; với `nn` không có `parameters`. Output TXT của `cw` có thêm dòng `Parameters: lambda=..., mu=..., nu=...` sau dòng `Method`.

`method` còn có thể là `cw-grid`: chạy W1 với mọi bộ trong một lưới tham số, trong một tiến trình, và xuất nghiệm tốt nhất (quãng đường nhỏ nhất; hòa thì ít tuyến hơn; hòa nữa thì bộ đứng trước trong thứ tự lưới: λ ngoài cùng, rồi μ, rồi ν, đều tăng dần). JSON có `parameters` là bộ tốt nhất và object `grid`: `"grid": {"lambda": {"start": 0.1, "stop": 2, "step": 0.1}, "mu": {...}, "nu": {...}, "configurations": 8820}`. Giá trị thứ k của một trục là `start + k*step`, số giá trị là `llround((stop - start)/step) + 1`; lưới mặc định là lưới G (λ 0,1–2,0; μ và ν 0–2,0; bước 0,1; 8.820 bộ). `timings_ms.construct` là thời gian toàn bộ lưới. Output TXT có thêm dòng `Parameters: ...` của bộ tốt nhất và `Grid configurations: N`. CLI: `--grid-lambda`, `--grid-mu`, `--grid-nu` dạng `START:STOP:STEP`; step không dương, giá trị không hữu hạn, start > stop, quá 1.000.000 bộ, dùng `--grid-*` với method khác, hoặc dùng `--lambda/--mu/--nu` với `cw-grid` đều là lỗi cú pháp (mã 2).

Output JSON có `routes`, `route_loads`, `route_distances`, `total_distance`, `num_routes`, `feasible`, `instance`, `method`, `timings_ms` và `node_ids`. `routes` dùng **chỉ số nội bộ** và luôn chứa `0` đầu/cuối; `node_ids` là mảng object có `internal_index` và `original_id` để khôi phục ID nguồn. Ví dụ minh họa:

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
  "timings_ms": {"construct": 1.2, "improve": 0.0, "validate": 0.1},
  "node_ids": [
    {"internal_index": 0, "original_id": "42"},
    {"internal_index": 1, "original_id": "7"},
    {"internal_index": 2, "original_id": "99"}
  ]
}
```

`route_loads`, `route_distances` tương ứng cùng thứ tự với `routes`; khoảng cách là số nguyên đơn vị tọa độ theo `EUC_2D`. Validator luôn tính lại ba số liệu này từ `instance` và `routes`, không tin các số liệu đã khai báo bởi nghiệm; CLI xuất số liệu đã tính lại. `num_routes` là số tuyến; `feasible` phản ánh validator. `timings_ms` chứa `construct`, `improve` và `validate`; Sprint 1 đặt `improve` bằng `0.0` vì chưa có pha cải thiện. Các thời gian là số thực không âm, chỉ đo thời gian solver theo pha và loại trừ đọc/ghi file, khởi động subprocess và mạng.

TXT phục vụ người đọc phải dùng `original_id`, kèm tải và khoảng cách từng tuyến. JSON giữ chỉ số nội bộ để không mơ hồ, cùng ánh xạ ID trong dữ liệu instance/metadata khi cần khôi phục ID nguồn. Lỗi dự kiến trả mã/thông điệp có thể máy đọc được; không được xuất nghiệm khả thi giả khi input không hợp lệ.
