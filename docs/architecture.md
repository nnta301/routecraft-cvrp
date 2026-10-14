# Kiến trúc dự kiến

## Thành phần và trách nhiệm

`solver` là thư viện lõi C++: mô hình instance, parser, khoảng cách, heuristic, cải thiện và validator. `solver/cli` là lớp giao diện dòng lệnh mỏng dùng lõi này. `backend` dự kiến kiểm tra request, quản lý thư mục tạm theo request, chạy CLI qua danh sách đối số và chuyển kết quả JSON. `frontend` dự kiến hiển thị dữ liệu, tuyến và chỉ số. Không thành phần nào trong số này đã được triển khai.

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

Đây là cấu trúc đích được ghi nhận, không phải các thư mục đã tạo trong foundation. CLI cần hoạt động độc lập nhưng dùng chung lõi với web. Docker chỉ được xem xét sau; binary trong container phải tương thích môi trường container. Một dịch vụ có thể phục vụ frontend đã build và API ở giai đoạn đầu. Render là lựa chọn hosting dự kiến, chưa chốt cấu hình hay chi phí.
