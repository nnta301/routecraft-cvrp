# Yêu cầu RouteCraft CVRP

## Yêu cầu bài toán

Input có một depot và dưới 100 khách, tọa độ 2D, nhu cầu không âm và capacity `Q` dương. Depot dùng chỉ số nội bộ `0`, trong khi ID gốc phải được giữ để xuất cho người dùng. `DIMENSION` tính cả depot. Một khách có nhu cầu lớn hơn `Q` là instance không khả thi trong mô hình hiện tại và phải bị từ chối/báo lỗi rõ ràng.

Mỗi khách được phục vụ đúng một lần. Tuyến chứa depot ở đầu và cuối, tải không quá `Q`. Tổng khoảng cách là tổng các cạnh `EUC_2D` đã làm tròn riêng từng cạnh; đơn vị là đơn vị tọa độ, không tự gọi là km.

## Solver và CLI

Solver phải cung cấp NN và về sau CW parallel; 2-opt first-improvement là hậu xử lý trong từng tuyến. CLI dự kiến nhận instance và lựa chọn method, xuất TXT đọc được cho người và JSON máy đọc được. Lõi thuật toán phải dùng chung giữa CLI và web. Validator phải tính lại độc lập khách, tải, depot, khoảng cách và tổng thay vì tin giá trị solver báo cáo.

## Web dự kiến

FastAPI dự kiến nhận dữ liệu đã kiểm tra, tạo vùng tạm riêng cho từng request và gọi executable bằng `subprocess` với danh sách đối số, không ghép shell command. Backend giới hạn kích thước input và timeout; stdout dành cho kết quả máy đọc được, stderr cho log/chẩn đoán. Frontend dự kiến trực quan hóa tuyến và so sánh cấu hình. Chưa có database, tài khoản hay lịch sử dài hạn.

## Phạm vi toàn dự án và Sprint 1

| Hạng mục | Sprint 1 | Toàn dự án |
| --- | --- | --- |
| Parser `.vrp`, mô hình, NN, validator, CLI TXT/JSON, tests | Bắt buộc | Có |
| CW, 2-opt, thực nghiệm so sánh | Chưa triển khai | Tiếp theo |
| API, giao diện, Docker/deploy | Chưa triển khai | Dự kiến sau |

Sprint 1 chưa đồng nghĩa ứng dụng web vận hành. Các yêu cầu web và cấu hình nâng cao là định hướng toàn dự án.
