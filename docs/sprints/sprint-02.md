# Sprint 2 — Hạ tầng thực nghiệm

## Trạng thái

Đã đóng (26-11-2026). Mọi tiêu chí hoàn thành đạt; một số việc được thêm vào phạm vi trong sprint (xem bên dưới).

## Bối cảnh

Hướng nghiên cứu đã chốt là Clarke–Wright với điểm học được, theo phương án PA1 (đánh giá có kiểm soát) trong [phản biện tính mới](../research/novelty-review-learned-cw.md). Clarke–Wright giữ nguyên thủ tục ghép, chỉ thay điểm dùng để xếp thứ tự phép ghép. Các cách chấm điểm được so trên cùng dữ liệu, theo cùng [giao thức thực nghiệm](../experiment-protocol.md).

Sprint 2 ứng với tuần 1–2 của kế hoạch PA1: dựng hạ tầng để mọi cách chấm điểm được đo như nhau.

## Thay đổi phạm vi

- [#50](https://github.com/nnta301/routecraft-cvrp/issues/50): chạy cả lưới W1 trong một tiến trình (`--method cw-grid`), vì chạy 8.820 bộ tham số qua CLI riêng lẻ quá chậm.
- [#51](https://github.com/nnta301/routecraft-cvrp/issues/51): kết quả W1-grid trên A, B, P.
- [#52](https://github.com/nnta301/routecraft-cvrp/issues/52): môi trường Python (numpy, scikit-learn, PyVRP) và kiểm tra PyVRP đo cùng quy ước, chuẩn bị cho Sprint 3.
- [#56](https://github.com/nnta301/routecraft-cvrp/issues/56): sửa quy tắc đọc gap âm sau phát hiện ở P-n55-k8 (BKS của A, B, P tối ưu khi dùng đúng k xe).
- [#41](https://github.com/nnta301/routecraft-cvrp/issues/41) mở rộng thành baseline gồm cả Nearest Neighbor và Clarke–Wright W0.

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

- [x] `experiments/run_benchmark.py` chạy Nearest Neighbor và Clarke–Wright (W0, W1) trên cả 73 instance; mọi nghiệm qua validator.
- [x] Bảng tổng hợp theo set có gap so với BKS kèm `m − k`, đúng giao thức.
- [x] CI Debug, Release và unittest Python đều đạt.

## Đã hoàn thành

| Issue | PR | Kết quả |
| --- | --- | --- |
| [#23](https://github.com/nnta301/routecraft-cvrp/issues/23) | [#42](https://github.com/nnta301/routecraft-cvrp/pull/42) | Ma trận khoảng cách có kiểm tra tràn số, truy cập node O(1) |
| [#26](https://github.com/nnta301/routecraft-cvrp/issues/26) | [#45](https://github.com/nnta301/routecraft-cvrp/pull/45), [#57](https://github.com/nnta301/routecraft-cvrp/pull/57) | [Giao thức thực nghiệm](../experiment-protocol.md) |
| [#37](https://github.com/nnta301/routecraft-cvrp/issues/37) | [#47](https://github.com/nnta301/routecraft-cvrp/pull/47) | Kế hoạch Sprint 2 |
| [#38](https://github.com/nnta301/routecraft-cvrp/issues/38) | [#43](https://github.com/nnta301/routecraft-cvrp/pull/43) | CVRPLIB A, B, P: 73 instance (loại `P-n101-k4`), BKS đều tối ưu, chi phí `.sol` tính lại khớp |
| [#39](https://github.com/nnta301/routecraft-cvrp/issues/39) | [#44](https://github.com/nnta301/routecraft-cvrp/pull/44) | Script benchmark chạy tiếp được khi bị ngắt, tổng hợp, unittest trong CI |
| [#40](https://github.com/nnta301/routecraft-cvrp/issues/40) | [#46](https://github.com/nnta301/routecraft-cvrp/pull/46) | Clarke–Wright parallel với hàm điểm thay thế được (W0, W1) |
| [#41](https://github.com/nnta301/routecraft-cvrp/issues/41) | [#48](https://github.com/nnta301/routecraft-cvrp/pull/48) | [Baseline NN và CW W0](../../experiments/results/baseline-nn-cw.md) |
| [#50](https://github.com/nnta301/routecraft-cvrp/issues/50) | [#53](https://github.com/nnta301/routecraft-cvrp/pull/53) | `--method cw-grid`: 8.820 bộ trong khoảng 0,5–1,4 s mỗi instance |
| [#51](https://github.com/nnta301/routecraft-cvrp/issues/51) | [#55](https://github.com/nnta301/routecraft-cvrp/pull/55) | [Kết quả W1-grid](../../experiments/results/w1-grid.md) |
| [#52](https://github.com/nnta301/routecraft-cvrp/issues/52) | [#54](https://github.com/nnta301/routecraft-cvrp/pull/54) | `.venv` với numpy, scikit-learn, PyVRP; PyVRP cho đúng BKS trên A-n32-k5 và B-n78-k10 |
| [#56](https://github.com/nnta301/routecraft-cvrp/issues/56) | [#57](https://github.com/nnta301/routecraft-cvrp/pull/57) | Gap âm chỉ là dấu hiệu lỗi khi m = k |

Kết quả chính trên 73 instance (gap so với BKS, trung bình / trung vị):

| Cấu hình | Gap trung bình | Gap trung vị | Ghi chú |
| --- | --- | --- | --- |
| Nearest Neighbor | 38,05% | 37,81% | — |
| Clarke–Wright W0 | 5,42% | 5,23% | Tốt hơn NN trên 73/73 instance |
| Clarke–Wright W1-grid | 2,00% | 2,16% | Tốt hơn W0 trên 72/73, hòa 1; bộ tham số tốt nhất rất phân tán |

## Bằng chứng

Mỗi issue đóng bằng PR ở bảng trên; CI Debug và Release đều đạt trên mọi PR và trên từng commit của `main`. Kết quả thực nghiệm chạy từ commit sạch, có ghi môi trường trong từng file kết quả.

## Chuyển tiếp

Tồn đọng: [#21](https://github.com/nnta301/routecraft-cvrp/issues/21) BOM UTF-8, [#22](https://github.com/nnta301/routecraft-cvrp/issues/22) `solve --help`, [#24](https://github.com/nnta301/routecraft-cvrp/issues/24) kiểm thử chặt hơn, [#25](https://github.com/nnta301/routecraft-cvrp/issues/25) CI Windows và sanitizer, [#49](https://github.com/nnta301/routecraft-cvrp/issues/49) `--help` của script Python trên console cp1252.

## Tổng kết

### Điều làm tốt

- Chốt giao thức và tiêu chí trước khi có số liệu.
- Mọi con số đều được kiểm chứng chéo: chi phí BKS tính lại, validator độc lập, test W1 trùng W1-grid.
- Các việc độc lập chạy song song trên worktree riêng, merge nhỏ và đều.

### Trở ngại hoặc khó khăn

- Quy tắc đọc gap âm ban đầu sai vì giả định BKS cho phép ít hơn k xe; phát hiện nhờ P-n55-k8.
- Console Windows cp1252 làm lỗi in tiếng Việt của script Python.

### Bài học

- Kiểm tra giả định về dữ liệu chuẩn (quy ước số xe của BKS) bằng chính dữ liệu trước khi viết thành quy tắc.
- Giữ một thủ tục ghép dùng chung cho mọi cách chấm điểm, kèm test trùng khớp, để so sánh luôn công bằng.
