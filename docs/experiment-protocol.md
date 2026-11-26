# Giao thức thực nghiệm

Giao thức này áp dụng cho mọi so sánh heuristic xây dựng trong hướng nghiên cứu "Clarke–Wright với điểm học được". Nó được chốt ngày 2026-11-26, trước khi chạy bất kỳ cấu hình nào trên bộ chuẩn. Muốn đổi điều gì sau ngày này thì phải ghi lý do vào `docs/research/LOG.md` trước khi chạy lại. Lập luận phía sau giao thức: [phản biện tính mới](research/novelty-review-learned-cw.md).

## 1. Bài toán và khoảng cách

- CVRP một depot, dưới 100 khách, không ấn định trước số xe. Số `k` trong tên instance CVRPLIB chỉ dùng để tham chiếu.
- Khoảng cách EUC_2D làm tròn từng cạnh theo TSPLIB.
- Mọi nghiệm phải qua validator độc lập. Nghiệm không qua validator không được tính vào kết quả.

## 2. Dữ liệu

| Tập | Vai trò | Nơi lưu |
| --- | --- | --- |
| CVRPLIB A, B, P (73 instance; loại `P-n101-k4`) | Kiểm tra ngoài miền, chạy một lần với cấu hình đã đóng băng | `data/benchmarks/cvrplib/` |
| Instance sinh theo cách của bộ X, 20–99 khách | Huấn luyện (khoảng 2.000), kiểm định (300), kiểm tra trong miền (500) | Sprint 3 |

- Không tune bất kỳ tham số nào trên A, B, P.
- Chỉ chạy lại A, B, P khi sửa lỗi, và phải ghi lý do vào LOG.

## 3. Chỉ số

- `D`: tổng quãng đường. `m`: số tuyến.
- Gap so với BKS: `gap_k = (D − BKS) / BKS × 100%`, BKS lấy từ `bks.csv` (với A, B, P đều là tối ưu đã chứng minh, ứng với `k` xe). Luôn báo kèm `m − k`.
  - BKS của CVRPLIB A, B, P là tối ưu khi dùng **đúng** k xe. Đề tài không giới hạn số xe, nên gap âm với `m ≠ k` là có thể. Ví dụ P-n55-k8: 7 xe là đủ (⌈1042/160⌉ = 7) và nghiệm 7 tuyến có chi phí 585 < BKS 588.
  - Gap âm khi `m = k` mâu thuẫn với tính tối ưu của BKS, nên là dấu hiệu lỗi (khoảng cách, parser hoặc validator) và phải điều tra trước khi dùng kết quả.
- So sánh giữa hai cấu hình A và B: chênh lệch tương đối theo từng instance, `Δ = (D_A − D_B) / D_B × 100%`. Hai nghiệm có cùng `D` (số nguyên) tính là hòa.
- Thời gian: `timings_ms.construct` trong JSON của CLI, bản Release. Gồm dựng ma trận khoảng cách, tính điểm, sắp xếp và ghép; không gồm đọc file và khởi động tiến trình. Thời gian chỉ dùng để mô tả, báo trung vị.

## 4. Cấu hình so sánh

| Mã | Điểm dùng để xếp thứ tự phép ghép | Sprint |
| --- | --- | --- |
| NN | Nearest Neighbor (tham chiếu, không phải Clarke–Wright) | 2 |
| W0 | Savings gốc `d(0,i) + d(0,j) − d(i,j)` | 2 |
| W1 | Savings tham số `d(0,i) + d(0,j) − λ·d(i,j) + μ·|d(0,i) − d(0,j)| + ν·(q_i + q_j)/q̄` | 2 |
| W1-grid | Chạy W1 với mọi bộ trong lưới G, lấy nghiệm tốt nhất cho từng instance | 2–3 |
| W2 | Trọng số tuyến tính tối ưu trực tiếp trên tổng quãng đường của tập huấn luyện | 3 |
| W4 | Gradient boosting nông học từ nhãn cạnh | 3 |
| SubLists | Cách tích hợp của Legrand et al. 2021 (sắp lại theo xác suất trong từng đoạn của danh sách savings), dùng cùng mô hình | 3 |

- Mọi cấu hình Clarke–Wright dùng cùng một thủ tục: duyệt một lượt, chỉ ghép khi khả thi và savings thật `d(0,i) + d(0,j) − d(i,j) > 0`. Điểm chỉ quyết định thứ tự.
- Lưới G: λ từ 0,1 đến 2,0, μ từ 0 đến 2,0, ν từ 0 đến 2,0, cùng bước 0,1 (8.820 bộ), theo cách dùng trong tài liệu về savings tham số. Nếu chạy cả lưới qua CLI quá chậm, thêm chế độ chạy lưới trong một tiến trình; không thu nhỏ lưới.
- Local search nằm ngoài phương án PA1.

## 5. Tiêu chí thành công (chốt trước)

1. **Chính:** W4 so với W2, không local search, trên toàn bộ A ∪ B ∪ P:
   - trung vị Δ ≤ −0,5% (W4 ngắn hơn);
   - Wilcoxon signed-rank một phía theo cặp instance, p < 0,05 sau hiệu chỉnh Holm cho các phép so sánh;
   - W4 thắng ít nhất 60% số instance không hòa.
2. **Giá trị thực tế:** trung vị Δ của W4 so với W1-grid, trong cùng ngân sách thời gian, không quá +0,2%. Nếu vượt ngưỡng thì kết luận rằng với dưới 100 khách, chạy cả lưới đã đủ tốt.
3. **Nhất quán:** kết luận 1 đúng cả trên 500 instance kiểm tra trong miền. 73 instance chuẩn không phải mẫu ngẫu nhiên, nên p-value trên A, B, P chỉ mang tính mô tả.

Ngưỡng 0,5% dựa trên suy luận rằng chênh lệch giữa các biến thể Clarke–Wright thường vào khoảng 1–3%. Legrand et al. 2021 báo cáo mức cải thiện khoảng 1,5% so với Clarke–Wright gốc.

## 6. Cổng dừng

- **Cổng 1** (sau tuần 6 của PA1, chỉ dùng tập kiểm định sinh): nếu trung vị cải thiện của W4 so với W2 dưới 0,3%, cho thêm tối đa một tuần thử đặc trưng. Sau đó khóa mô hình và viết kết quả âm kèm phân tích đặc trưng.
- **Cổng 2:** nếu kết quả tốt trong miền nhưng mất trên A, B, P, thử đúng một lần với bộ sinh khớp phân phối A, B, P (khai báo trước), không thử thêm.

## 7. Tính xác định

- Nearest Neighbor: khi khoảng cách hòa, chọn khách có chỉ số nội bộ nhỏ hơn.
- Clarke–Wright: sắp điểm giảm dần; khi hòa, sắp theo `(i, j)` tăng dần.
- Mọi thành phần ngẫu nhiên (sinh instance, chia tập, huấn luyện) dùng seed ghi trong cấu hình và trong kết quả.

## 8. Môi trường chạy

- Bản Release, build từ một commit sạch. Kết quả có `git_sha` mang hậu tố `-dirty` không được commit.
- Máy i5-14600KF; tối đa 6 tiến trình song song, ghim P-core (CPU logic 0–11, mask `0xFFF`). Phiên bản compiler và hệ điều hành ghi trong file tổng hợp kèm kết quả.

## 9. Lưu kết quả

- Chạy bằng `experiments/run_benchmark.py`; định dạng CSV và cách tiếp tục khi bị ngắt xem `experiments/README.md`.
- Kết quả tạm để ở `runtime-output/experiments/` (không commit).
- Kết quả chọn để tái lập để ở `experiments/results/<tên>.csv`, kèm `<tên>.md` gồm bảng tổng hợp, lệnh chạy, commit, ngày chạy và máy.
