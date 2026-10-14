# Dữ liệu

`toy/` dự kiến chứa instance nhỏ tự tạo; `benchmarks/` dự kiến chứa bản sao hoặc tham chiếu benchmark có nguồn ghi rõ. Không tải benchmark trong foundation này.

Mỗi dữ liệu cần ghi nguồn, định danh instance, quy ước depot/ID, đơn vị tọa độ, capacity và biến thể số xe nếu nguồn nêu. Với CVRPLIB, không tự suy ra phải dùng đúng `K` từ tên instance: mô hình cơ sở không giới hạn trước số xe. Khi lưu kết quả tham chiếu/thực nghiệm, cần ghi method, cấu hình, quy ước khoảng cách, ngày tạo và phân biệt BKS với optimum đã chứng minh. Dữ liệu và kết quả đã chọn để tái lập có thể commit; output tạm tự sinh không nên commit.
