# Lộ trình

## Sprint 1

Scaffold C++ build được, CLI help/version và smoke test đã có, nhưng Sprint 1 chưa hoàn thành. Sprint hoàn thành khi có mô hình dữ liệu, parser `.vrp`, NN, validator độc lập, CLI xuất TXT/JSON và tests. Validator phải tự tính lại tập khách, depot, tải, khoảng cách và tổng khoảng cách; không tin trực tiếp số liệu từ solver.

## Tiếp theo

Thêm CW parallel, 2-opt và thực nghiệm đối chiếu. Điều kiện hoàn thành là mỗi cấu hình có thể chạy trên cùng input, xuất metadata đủ để đối chiếu và được validator kiểm tra.

## Sau đó

Xây API, frontend, Docker/deploy, phân tích và báo cáo. Các phần này hoàn thành khi có ranh giới dữ liệu rõ, kiểm tra lỗi/giới hạn request phù hợp và tài liệu triển khai đã được xác nhận; hiện chưa triển khai.

## Thực nghiệm dự kiến

Khởi đầu bằng dữ liệu tự tạo 5–8 khách, rồi khoảng 10–15 instance CVRPLIB nhỏ/vừa. Ưu tiên A/B, thay đổi một yếu tố tại một thời điểm. Chỉ so sánh tham chiếu khi đã thống nhất khoảng cách và quy ước số xe; phải phân biệt BKS với nghiệm tối ưu đã được chứng minh.

Lộ trình không gán thời hạn hoặc thành viên phụ trách.
