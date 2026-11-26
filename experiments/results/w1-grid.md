# Clarke–Wright W1-grid trên CVRPLIB A, B, P

Trên 73 instance (A, B, P của CVRPLIB), Clarke–Wright với lưới tham số W1 (8.820 bộ λ, μ, ν cho mỗi instance) đạt gap trung bình 2,00% và trung vị 2,16% so với BKS, so với 5,42% và 5,23% của W0 và 38,05% và 37,81% của Nearest Neighbor; quãng đường của W1-grid không xấu hơn W0 trên cả 73/73 instance (72 tốt hơn, 1 hòa), với trung vị Δ = −2,47%.

## Bảng tổng hợp

| set | config | instances | ok | gap TB (%) | gap trung vị (%) | số xe TB | m-k TB | total_ms trung vị |
|---|---|---|---|---|---|---|---|---|
| A | cw-grid | 27 | 27 | 2.16 | 2.12 | 7.15 | 0.07 | 404.53 |
| A | cw-w0 | 27 | 27 | 5.11 | 4.37 | 7.37 | 0.30 | 0.11 |
| A | nn | 27 | 27 | 39.14 | 39.80 | 7.11 | 0.04 | 0.03 |
| B | cw-grid | 23 | 23 | 1.87 | 1.83 | 7.35 | 0.17 | 484.64 |
| B | cw-w0 | 23 | 23 | 4.10 | 3.36 | 7.52 | 0.35 | 0.12 |
| B | nn | 23 | 23 | 41.78 | 38.28 | 7.26 | 0.09 | 0.03 |
| P | cw-grid | 23 | 23 | 1.94 | 2.30 | 7.74 | 0.30 | 458.88 |
| P | cw-w0 | 23 | 23 | 7.11 | 7.65 | 7.83 | 0.39 | 0.11 |
| P | nn | 23 | 23 | 33.06 | 32.84 | 7.57 | 0.13 | 0.04 |
| **tổng** | **cw-grid** | **73** | **73** | **2.00** | **2.16** | **7.40** | **0.18** | **458.88** |
| **tổng** | **cw-w0** | **73** | **73** | **5.42** | **5.23** | **7.56** | **0.34** | **0.11** |
| **tổng** | **nn** | **73** | **73** | **38.05** | **37.81** | **7.30** | **0.08** | **0.04** |

Gap tính theo `gap_pct` so với BKS. m − k là số tuyến trừ k, với k là số xe ghi trong tên instance (chỉ để tham chiếu). Đề tài không giới hạn số xe, nên k không phải số xe tối thiểu. Thời gian (ms) là `total_ms` của solver.

## So sánh theo cặp

Δ = (D_a − D_b) / D_b × 100%, tính theo từng instance trên tổng quãng đường.

| cặp (a vs b) | phạm vi | n | trung vị Δ (%) | trung bình Δ (%) | a tốt hơn | hòa | a kém hơn |
|---|---|---|---|---|---|---|---|
| cw-grid vs cw-w0 | toàn bộ | 73 | −2.47 | −3.18 | 72 | 1 | 0 |
| cw-grid vs cw-w0 | A | 27 | −2.27 | −2.77 | 27 | 0 | 0 |
| cw-grid vs cw-w0 | B | 23 | −1.13 | −2.09 | 23 | 0 | 0 |
| cw-grid vs cw-w0 | P | 23 | −4.57 | −4.73 | 22 | 1 | 0 |
| cw-w0 vs nn | toàn bộ | 73 | −23.96 | −22.92 | 73 | 0 | 0 |
| cw-w0 vs nn | A | 27 | −24.75 | −24.21 | 27 | 0 | 0 |
| cw-w0 vs nn | B | 23 | −25.00 | −25.37 | 23 | 0 | 0 |
| cw-w0 vs nn | P | 23 | −21.01 | −18.94 | 23 | 0 | 0 |

Hòa là trường hợp cw-grid và cw-w0 cho cùng tổng quãng đường; chỉ có 1 instance hòa, thuộc set P.

## Phân bố bộ tham số tốt nhất

Bộ tham số tốt nhất lấy từ trường `parameters` của `routecraft solve --method cw-grid` cho từng instance (xem `experiments/results/w1-grid.csv` để đối chiếu tổng quãng đường; không có lệch nào).

- λ hay gặp nhất: 1,3 (9 instance), 0,9 (7), 1,4 (7), 1,5 (6), 1,8 (6), 1,0 (6).
- μ hay gặp nhất: 0 (20 instance), 0,2 (11), 0,1 (9), 0,3 (6), 0,5 (6).
- ν hay gặp nhất: 0 (29 instance), 0,1 (7), 0,3 (5), 1,7 (5), 0,7 (4).
- Không có bộ (λ, μ, ν) nào thắng trên nhiều hơn 2 instance; phần lớn instance có bộ tốt nhất riêng.
- Cụ thể, ν = 0 là lựa chọn phổ biến nhất (29/73), nghĩa là lưới thường chọn ν = 0; μ = 0 cũng phổ biến nhất (20/73).

## Thời gian

- `construct_ms` trung vị: Nearest Neighbor 0,031 ms; CW W0 0,107 ms; CW W1-grid 458,9 ms (trung bình 512,0 ms, lớn nhất 1.436,2 ms).
- W1-grid tốn khoảng 4.000 lần thời gian của W0 để duyệt 8.820 bộ tham số trên mỗi instance.

## Vi phạm kiểm tra

- Số dòng ok: 219/219 (73 × 3), không có lỗi hay timeout.
- Không có `git_sha` mang hậu tố `-dirty`.
- Có 1 dòng `gap_pct < 0` với `m − k ≤ 0`: P-n55-k8, cw-grid, gap −0,51%, 7 tuyến so với k = 8 (quãng đường 585, BKS 588). Đã kiểm tra, đây không phải lỗi: Q = 160 và tổng nhu cầu 1042, nên 7 xe là đủ (⌈1042/160⌉ = 7); nghiệm 7 tuyến hợp lệ, chi phí tính lại độc lập đúng 585; nghiệm BKS trong `.sol` dùng đúng 8 tuyến. BKS của CVRPLIB A, B, P là tối ưu khi dùng đúng k xe, nên khi đề tài cho phép ít xe hơn thì gap âm với m < k là có thể. Trong 73 instance, P-n55-k8 là instance duy nhất có k lớn hơn cận dưới ⌈Σq/Q⌉.
- Không có trường hợp nào cw-grid có quãng đường lớn hơn cw-w0.

## Cách tái lập

Build và test (cấu hình Release):

```
cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/Release
ctest --test-dir build/Release --output-on-failure
```

Chạy benchmark (chạy lại lệnh sẽ bỏ qua các ca đã có kết quả):

```
PYTHONIOENCODING=utf-8 python experiments/run_benchmark.py --config "nn=--method nn" --config "cw-w0=--method cw" --config "cw-grid=--method cw-grid" --timeout 120 --out runtime-output/experiments/w1-grid.csv
```

Tổng hợp:

```
PYTHONIOENCODING=utf-8 python experiments/summarize.py runtime-output/experiments/w1-grid.csv --out runtime-output/experiments/w1-grid-summary.md
```

Kết quả thô đầy đủ nằm ở `experiments/results/w1-grid.csv` (219 dòng dữ liệu = 73 instance × 3 cấu hình).

## Môi trường

- Commit: `0ae2022`, cây làm việc sạch khi chạy
- Ngày chạy: 2026-11-26
- CPU: Intel Core i5-14600KF; ghim tiến trình vào CPU logic 0–11 (mask `0xFFF`)
- Hệ điều hành: Windows 11 Pro 10.0.26200
- Compiler: MinGW-w64 g++ 14.2.0 (`c++.exe`, GCC 14.2.0 UCRT POSIX SEH)
- Python: 3.14.4

Giao thức thí nghiệm: [`../../docs/experiment-protocol.md`](../../docs/experiment-protocol.md)
