# Backend

Backend dự kiến dùng FastAPI để kiểm tra input, quản lý vùng tạm riêng theo request, gọi executable solver qua `subprocess` với danh sách đối số và trả kết quả JSON. stdout chỉ dành cho kết quả máy đọc được; stderr dành cho log/chẩn đoán. Kích thước input và timeout phải được giới hạn.

Trạng thái: **chưa triển khai**. Chưa có ứng dụng FastAPI, dependency hay API đang hoạt động.
