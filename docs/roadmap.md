# Lộ trình

## Sprint 1

Sprint 1 đã đóng. Đã có mô hình dữ liệu, parser `.vrp`, NN, validator độc lập, CLI và tests. CLI hiện hỗ trợ `routecraft --help` / `-h`, `routecraft --version` và `routecraft solve --input <file.vrp> --method nn --format json|txt`. Validator tự tính lại tập khách, depot, tải, khoảng cách và tổng khoảng cách; không tin trực tiếp số liệu từ solver. Chi tiết và các việc tồn đọng: [Sprint 1](sprints/sprint-01.md).

## Hướng nghiên cứu

Clarke–Wright với điểm học được, phương án PA1 (đánh giá có kiểm soát): giữ nguyên thủ tục ghép, chỉ thay điểm xếp thứ tự phép ghép, rồi so savings gốc, savings tham số, tham số tune trực tiếp và mô hình học từ nhãn cạnh trên CVRPLIB A, B, P khi không ấn định số xe. Lập luận: [phản biện tính mới](research/novelty-review-learned-cw.md). Cách đo và tiêu chí thành công: [giao thức thực nghiệm](experiment-protocol.md).

## Sprint 2 (đang thực hiện)

Hạ tầng thực nghiệm: ma trận khoảng cách dùng chung, dữ liệu CVRPLIB A, B, P kèm BKS, script chạy benchmark, giao thức thực nghiệm, Clarke–Wright nhận hàm điểm thay thế được (W0, W1) và kết quả baseline. Chi tiết: [Sprint 2](sprints/sprint-02.md).

## Tiếp theo

Sprint 3: bộ sinh instance kiểu X, gán nhãn cạnh bằng lời giải PyVRP, huấn luyện mô hình (W4), tune trực tiếp (W2), SubLists; cổng dừng sau tuần 6 của PA1. Sau PA1: Clarke–Wright ngẫu nhiên hóa theo điểm học (PA2), rồi time window và local search ở Project 2.

## Sau đó

Xây API, frontend, Docker/deploy, phân tích và báo cáo. Các phần này hoàn thành khi có ranh giới dữ liệu rõ, kiểm tra lỗi/giới hạn request phù hợp và tài liệu triển khai đã được xác nhận; hiện chưa triển khai.

## Thực nghiệm dự kiến

Theo [giao thức thực nghiệm](experiment-protocol.md): 73 instance CVRPLIB A, B, P là tập kiểm tra ngoài miền, không tune trên đó; huấn luyện và kiểm định trên instance sinh kiểu X. Gap so với BKS luôn báo kèm số xe.

Lộ trình không gán thời hạn hoặc thành viên phụ trách.
