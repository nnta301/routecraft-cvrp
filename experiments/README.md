# Thực nghiệm hàng loạt

Chỉ dùng thư viện chuẩn của Python (tương thích 3.12).

## Build Release

```
cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/Release
```

## Chạy

```
python experiments/run_benchmark.py --config "nn=--method nn"
python experiments/run_benchmark.py --instances "data/benchmarks/cvrplib/A/*.vrp" \
    --config "nn=--method nn" --config "cw=--method cw --lambda 1 --mu 1 --nu 1" \
    --jobs 6 --timeout 60 --out runtime-output/experiments/results.csv
python experiments/summarize.py runtime-output/experiments/results.csv [--out bang.md]
```

Tham số: `--exe`, `--instances` (glob, lặp được), `--bks`, `--config NAME=ARGS`
(lặp được; ARGS tách bằng `shlex` rồi chèn vào lệnh `solve`), `--out`,
`--jobs` (tối đa 6), `--timeout` (giây mỗi lần chạy), `--no-pin`.
Thiếu `bks.csv` hoặc thiếu dòng của instance thì `bks` và `gap_pct` để trống.

## Cột CSV

`instance, set, customers, k, config, status, total_distance, num_routes, bks,
gap_pct, vehicles_minus_k, construct_ms, total_ms, git_sha, timestamp`

- `status`: `ok`, `error`, `timeout`, `infeasible`.
- `gap_pct` = (total - bks) / bks * 100, làm tròn 4 chữ số.
- `vehicles_minus_k` = `num_routes` - `k`.
- `total_ms` = tổng các pha trong `timings_ms`.
- `git_sha` thêm `-dirty` nếu working tree bẩn; `timestamp` là ISO 8601 UTC.

## Resume

Khóa là (`set`, `instance`, `config`). Khóa đã có dòng `ok` thì bị bỏ qua.
Khóa chỉ có dòng lỗi thì chạy lại và ghi thêm dòng mới; dòng sau cùng của mỗi
khóa là dòng có hiệu lực (`summarize.py` cũng làm vậy). Mỗi dòng được ghi và
flush ngay khi xong, nên ngắt giữa chừng không mất kết quả đã có.

## Ghim CPU

Mặc định ghim vào P-core (CPU logic 0-11, mask `0xFFF`) khi máy có từ 12 CPU
logic trở lên: Windows dùng `SetProcessAffinityMask`, Linux dùng
`os.sched_setaffinity`; tiến trình con kế thừa. Nơi khác chỉ in cảnh báo.
Tắt bằng `--no-pin`.

## Lưu kết quả

- Kết quả tạm: `runtime-output/experiments/` (không commit).
- Kết quả đã chọn để tái lập: `experiments/results/` (commit, kèm bảng tổng hợp).
