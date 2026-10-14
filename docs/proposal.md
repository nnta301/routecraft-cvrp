# Đề xuất: RouteCraft CVRP

## Lý do chọn đề tài

CVRP là bài toán tối ưu tổ hợp gần với bài toán giao nhận thực tế, nhưng đủ gọn để khảo sát cách lựa chọn heuristic, chất lượng nghiệm và chi phí tính toán. Đề tài tận dụng nền tảng C++ và thuật toán để tạo một solver có thể kiểm chứng, sau đó dùng web để trình bày và đối chiếu kết quả dễ hơn.

## Mục tiêu nghiên cứu

1. Mô hình hóa CVRP một depot với xe đồng nhất và nhu cầu không chia nhỏ.
2. Tự cài đặt NN và Clarke–Wright Savings parallel bằng C++.
3. Đánh giá ảnh hưởng của 2-opt trong từng tuyến đối với nghiệm ban đầu.
4. So sánh có kiểm soát giữa bốn cấu hình: NN, NN + 2-opt, CW, CW + 2-opt.
5. Cung cấp công cụ trực quan hóa hỗ trợ sử dụng và minh họa kết quả.

## Cách tiếp cận

Khoảng cách dùng `EUC_2D`: làm tròn từng cạnh theo số nguyên gần nhất rồi cộng. NN chọn khách chưa phục vụ gần nhất còn vừa tải và đóng tuyến khi không thể thêm khách. CW ghép hai tuyến khác nhau tại các đầu/cuối khách, đảo chiều khi cần và luôn kiểm tra tải. 2-opt first-improvement chỉ cải thiện thứ tự trong một tuyến, nên không đổi nhóm khách, tải hay số tuyến.

## Kế hoạch thực nghiệm

Thực nghiệm dự kiến bắt đầu với dữ liệu tự tạo 5–8 khách để kiểm tra hành vi. Sau đó dùng khoảng 10–15 instance nhỏ/vừa từ CVRPLIB, ưu tiên so sánh A/B từng thành phần. Mỗi lần chạy cần ghi instance, cấu hình, tổng khoảng cách, số tuyến, tính khả thi và thời gian solver. So sánh với tham chiếu chỉ có ý nghĩa khi thống nhất cách tính khoảng cách và quy ước số xe; BKS phải được phân biệt với nghiệm tối ưu đã chứng minh.

## Sản phẩm dự kiến

Solver/CLI C++, bộ kiểm thử và validator độc lập; API FastAPI; giao diện React + TypeScript; dữ liệu thí nghiệm có nguồn gốc; báo cáo phân tích. Đây là sản phẩm dự kiến, chưa phải thành phần đã hoàn thành.

## Giới hạn

Không mở rộng mặc định sang ML, ACO, GA, Tabu Search hay relocate giữa tuyến. Web không là trọng tâm đánh giá học thuật. Không giải quyết time windows, nhiều depot, xe khác tải trọng, split delivery hay định tuyến đường bộ.
