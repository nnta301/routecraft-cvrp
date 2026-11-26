# Sprint 2 — Hạ tầng thực nghiệm

## Trạng thái

Đang thực hiện, bắt đầu 26-11-2026.

## Bối cảnh

Hướng nghiên cứu đã chốt là Clarke–Wright với điểm học được, theo phương án PA1 (đánh giá có kiểm soát) trong [phản biện tính mới](../research/novelty-review-learned-cw.md). Clarke–Wright giữ nguyên thủ tục ghép, chỉ thay điểm dùng để xếp thứ tự phép ghép. Các cách chấm điểm được so trên cùng dữ liệu, theo cùng [giao thức thực nghiệm](../experiment-protocol.md).

Sprint 2 ứng với tuần 1–2 của kế hoạch PA1: dựng hạ tầng để mọi cách chấm điểm được đo như nhau.

## Mục tiêu

Một lệnh chạy được Nearest Neighbor và Clarke–Wright (W0, W1) trên 73 instance CVRPLIB A, B, P và ra bảng kết quả đo theo giao thức.

## Phạm vi

- Ma trận khoảng cách dùng chung, truy cập node O(1).
- Giao thức thực nghiệm.
- Dữ liệu CVRPLIB A, B, P kèm BKS.
- Script chạy benchmark (tiếp tục được khi bị ngắt) và tổng hợp kết quả.
- Clarke–Wright parallel với hàm điểm thay thế được: W0 (savings gốc) và W1 (savings tham số).
- Kết quả baseline Nearest Neighbor.

## Ngoài phạm vi

- Bộ sinh instance, gán nhãn, huấn luyện mô hình (W2, W4, SubLists): Sprint 3.
- Local search, time window, web app.
- Các việc tồn đọng từ Sprint 1: #21, #22, #24, #25.

## Các issue đã cam kết

| Issue | Nội dung | Phụ thuộc |
| --- | --- | --- |
| [#37](https://github.com/nnta301/routecraft-cvrp/issues/37) | Kế hoạch Sprint 2 | — |
| [#26](https://github.com/nnta301/routecraft-cvrp/issues/26) | Giao thức thực nghiệm | — |
| [#23](https://github.com/nnta301/routecraft-cvrp/issues/23) | Ma trận khoảng cách dùng chung | — |
| [#38](https://github.com/nnta301/routecraft-cvrp/issues/38) | CVRPLIB A, B, P kèm BKS | — |
| [#39](https://github.com/nnta301/routecraft-cvrp/issues/39) | Script chạy benchmark và tổng hợp | Định dạng `bks.csv` của #38 |
| [#40](https://github.com/nnta301/routecraft-cvrp/issues/40) | Clarke–Wright với hàm điểm thay thế được (W0, W1) | #23 |
| [#41](https://github.com/nnta301/routecraft-cvrp/issues/41) | Kết quả baseline Nearest Neighbor | #26, #38, #39 |

## Tiêu chí hoàn thành

- [ ] `experiments/run_benchmark.py` chạy Nearest Neighbor và Clarke–Wright (W0, W1) trên cả 73 instance; mọi nghiệm qua validator.
- [ ] Bảng tổng hợp theo set có gap so với BKS kèm `m − k`, đúng giao thức.
- [ ] CI Debug, Release và unittest Python đều đạt.

## Đã hoàn thành

Cập nhật khi kết thúc sprint.

## Bằng chứng

Cập nhật khi kết thúc sprint.

## Tổng kết

Cập nhật khi kết thúc sprint.
