# RouteCraft CVRP

RouteCraft CVRP là đề tài nghiên cứu và cài đặt các heuristic cho **Capacitated Vehicle Routing Problem (CVRP)**, kết hợp một ứng dụng web để trực quan hóa và so sánh kết quả.

## Mục tiêu

Trọng tâm học thuật của dự án là xây dựng, kiểm chứng và thực nghiệm các heuristic trên mô hình CVRP một kho. Ứng dụng web là lớp hỗ trợ nhập dữ liệu, chạy solver và trình bày kết quả; không thay thế phần đánh giá thuật toán.

## Phạm vi mô hình

- Một depot, dưới 100 khách hàng; nhu cầu biết trước và không được chia nhỏ.
- Xe đồng nhất với tải trọng tối đa `Q`; mỗi tuyến xuất phát/kết thúc tại depot và không vượt `Q`.
- Tối thiểu hóa tổng khoảng cách với khoảng cách đối xứng `EUC_2D`: làm tròn từng cạnh đến số nguyên gần nhất trước khi cộng.
- Không ấn định trước số xe; số tuyến khả thi là một kết quả cần báo cáo.
- Chưa bao gồm time windows, nhiều depot, đội xe không đồng nhất, split delivery hoặc khoảng cách đường bộ.

## Hướng kỹ thuật dự kiến

Solver C++ dùng chung lõi cho CLI và web. Các cấu hình nghiên cứu dự kiến là Nearest Neighbor (NN), NN + 2-opt, Clarke–Wright Savings parallel (CW), và CW + 2-opt. Backend FastAPI dự kiến gọi executable bằng `subprocess` với danh sách đối số; frontend dùng React + TypeScript.

## Trạng thái thực tế

Hiện repository mới có nền tảng tài liệu và quy ước phát triển. Solver, CLI, backend, frontend, Docker, CI, benchmark và API **chưa triển khai**; chưa có hướng dẫn chạy ứng dụng.

## Tài liệu

- [Đề xuất nghiên cứu](docs/proposal.md)
- [Yêu cầu](docs/requirements.md)
- [Kiến trúc dự kiến](docs/architecture.md)
- [Hợp đồng dữ liệu v0](docs/data-contract.md)
- [Phát triển và môi trường](docs/development.md)
- [Lộ trình](docs/roadmap.md)
- [Quy ước dữ liệu](data/README.md)
