# Lộ trình

## Sprint 1

Sprint 1 đã đóng. Đã có mô hình dữ liệu, parser `.vrp`, NN, validator độc lập, CLI và tests. CLI hiện hỗ trợ `routecraft --help` / `-h`, `routecraft --version` và `routecraft solve --input <file.vrp> --method nn --format json|txt`. Validator tự tính lại tập khách, depot, tải, khoảng cách và tổng khoảng cách; không tin trực tiếp số liệu từ solver. Chi tiết và các việc tồn đọng: [Sprint 1](sprints/sprint-01.md).

## Tiếp theo

Sprint 2 đang lập kế hoạch, sau khi chốt hướng nghiên cứu với giảng viên hướng dẫn. Các hướng đã dự kiến trước đây: CW parallel, 2-opt và thực nghiệm đối chiếu. Điều kiện hoàn thành là mỗi cấu hình có thể chạy trên cùng input, xuất metadata đủ để đối chiếu và được validator kiểm tra.

## Sau đó

Xây API, frontend, Docker/deploy, phân tích và báo cáo. Các phần này hoàn thành khi có ranh giới dữ liệu rõ, kiểm tra lỗi/giới hạn request phù hợp và tài liệu triển khai đã được xác nhận; hiện chưa triển khai.

## Thực nghiệm dự kiến

Khởi đầu bằng dữ liệu tự tạo 5–8 khách, rồi khoảng 10–15 instance CVRPLIB nhỏ/vừa. Ưu tiên A/B, thay đổi một yếu tố tại một thời điểm. Chỉ so sánh tham chiếu khi đã thống nhất khoảng cách và quy ước số xe; phải phân biệt BKS với nghiệm tối ưu đã được chứng minh.

Lộ trình không gán thời hạn hoặc thành viên phụ trách.
