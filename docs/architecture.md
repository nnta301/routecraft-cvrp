# Kiến trúc dự kiến

## Thành phần và trách nhiệm

`solver` có scaffold thư viện lõi C++ (`routecraft_core`) và `solver/cli` là lớp dòng lệnh mỏng liên kết thư viện đó. Hiện lõi chỉ cung cấp phiên bản; chưa có mô hình instance, parser, khoảng cách, heuristic, cải thiện hay validator. `backend` dự kiến kiểm tra request, quản lý thư mục tạm theo request, chạy CLI qua danh sách đối số và chuyển kết quả JSON. `frontend` dự kiến hiển thị dữ liệu, tuyến và chỉ số.

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

Scaffold hiện đã tạo các thư mục C++ nêu trên; các cấu trúc backend, frontend, data và experiments vẫn là cấu trúc đích. CLI cần hoạt động độc lập nhưng dùng chung lõi với web. Docker chỉ được xem xét sau; binary trong container phải tương thích môi trường container. Một dịch vụ có thể phục vụ frontend đã build và API ở giai đoạn đầu. Render là lựa chọn hosting dự kiến, chưa chốt cấu hình hay chi phí.
