# Baseline Nearest Neighbor và Clarke–Wright W0 trên CVRPLIB A, B, P

Trên 73 instance (A, B, P của CVRPLIB), Clarke–Wright W0 đạt gap trung bình 5,42% và trung vị 5,23% so với BKS, còn Nearest Neighbor đạt gap trung bình 38,05% và trung vị 37,81%; trung vị Δ của CW so với NN là −23,96%, và CW tốt hơn NN trên cả 73/73 instance.

## Bảng tổng hợp

| set | config | instances | ok | gap TB (%) | gap trung vị (%) | số xe TB | m-k TB | total_ms trung vị |
|---|---|---|---|---|---|---|---|---|
| A | cw-w0 | 27 | 27 | 5.11 | 4.37 | 7.37 | 0.30 | 0.09 |
| A | nn | 27 | 27 | 39.14 | 39.80 | 7.11 | 0.04 | 0.03 |
| B | cw-w0 | 23 | 23 | 4.10 | 3.36 | 7.52 | 0.35 | 0.12 |
| B | nn | 23 | 23 | 41.78 | 38.28 | 7.26 | 0.09 | 0.04 |
| P | cw-w0 | 23 | 23 | 7.11 | 7.65 | 7.83 | 0.39 | 0.12 |
| P | nn | 23 | 23 | 33.06 | 32.84 | 7.57 | 0.13 | 0.04 |
| **tổng** | **cw-w0** | **73** | **73** | **5.42** | **5.23** | **7.56** | **0.34** | **0.12** |
| **tổng** | **nn** | **73** | **73** | **38.05** | **37.81** | **7.30** | **0.08** | **0.04** |

Gap tính theo `gap_pct` so với BKS; số xe TB là số tuyến trung bình, m − k TB là trung bình của (số tuyến − k), với k là số xe ghi trong tên instance (chỉ để tham chiếu, đề tài không giới hạn số xe). Thời gian (ms) là thời gian chạy của solver.

## So sánh theo cặp (CW W0 so với NN)

Δ = (D_cw − D_nn) / D_nn × 100%, tính theo từng instance.

| phạm vi | n | trung vị Δ (%) | trung bình Δ (%) | CW tốt hơn | hòa | CW kém hơn |
|---|---|---|---|---|---|---|
| toàn bộ | 73 | -23.96 | -22.92 | 73 | 0 | 0 |
| A | 27 | -24.75 | -24.21 | 27 | 0 | 0 |
| B | 23 | -25.00 | -25.37 | 23 | 0 | 0 |
| P | 23 | -21.01 | -18.94 | 23 | 0 | 0 |

## Cách tái lập

Build và test (cấu hình Release):

```
cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/Release
ctest --test-dir build/Release --output-on-failure
```

Chạy benchmark (chạy lại lệnh sẽ bỏ qua các ca đã có kết quả):

```
python experiments/run_benchmark.py --config "nn=--method nn" --config "cw-w0=--method cw" --out runtime-output/experiments/baseline-nn-cw.csv
```

Kết quả thô đầy đủ nằm ở `experiments/results/baseline-nn-cw.csv` (146 dòng dữ liệu = 73 instance × 2 cấu hình).

## Môi trường

- Commit: `16b177b` (không có thay đổi chưa commit khi chạy)
- Ngày chạy: 2026-11-26
- CPU: Intel Core i5-14600KF; ghim tiến trình vào CPU logic 0–11 (mask `0xFFF`)
- Hệ điều hành: Windows 11 Pro 10.0.26200
- Compiler: MinGW-w64 g++ 14.2.0 (`c++.exe`, GCC 14.2.0 UCRT POSIX SEH)
- Python: 3.14.4
- Số tiến trình song song: 6

Giao thức thí nghiệm: [`../../docs/experiment-protocol.md`](../../docs/experiment-protocol.md)
