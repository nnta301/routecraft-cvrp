# Kiến trúc dự kiến

## Thành phần và trách nhiệm

`solver` có thư viện lõi C++ (`routecraft_core`) và `solver/cli` là lớp dòng lệnh mỏng liên kết thư viện đó. Hiện lõi đã cung cấp mô hình dữ liệu CVRP tối thiểu gồm `Node`, `Instance`, `Route`, `Solution` và cấu trúc dữ liệu `ValidationResult` dùng cho validator sau này; depot dùng chỉ số nội bộ `0` và mỗi node giữ `original_id` để khôi phục ID nguồn. Lõi cũng có tiện ích tính khoảng cách `EUC_2D`, tải tuyến, độ dài tuyến và tổng độ dài nghiệm.

Parser `.vrp` đã hỗ trợ phạm vi CVRPLIB của Sprint 1: một depot, `TYPE: CVRP`, `EDGE_WEIGHT_TYPE: EUC_2D`, chuẩn hóa depot nội bộ `0`, giữ ID nguồn và kiểm tra dữ liệu đầu vào. Heuristic, bước cải thiện và validator độc lập vẫn chưa được triển khai. `backend` dự kiến kiểm tra request, quản lý thư mục tạm theo request, chạy CLI qua danh sách đối số và chuyển kết quả JSON. `frontend` dự kiến hiển thị dữ liệu, tuyến và chỉ số.

Luồng dự kiến: người dùng hoặc CLI cung cấp instance → parser/kiểm tra dữ liệu → solver tạo nghiệm → validator tính lại độc lập → CLI xuất TXT/JSON hoặc backend trả JSON → frontend trực quan hóa. Log và chẩn đoán đi stderr; dữ liệu kết quả máy đọc được đi stdout.

## Cấu trúc đích

```text
solver/
  include/      # API C++ công khai
  src/          # hiện thực lõi
  cli/          # executable CLI
  tests/        # unit/integration tests
backend/
  app/          # FastAPI application
frontend/
  src/          # React + TypeScript
data/
  toy/          # instance nhỏ tự tạo
  benchmarks/   # benchmark có nguồn gốc
experiments/    # cấu hình, kết quả được chọn để lưu và phân tích
```
