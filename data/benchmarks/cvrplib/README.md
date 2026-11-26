# CVRPLIB: set A, B, P

Bản sao các instance CVRP của CVRPLIB (set A, B, P) cùng lời giải tốt nhất đã biết (BKS), dùng để kiểm tra và đối chiếu thuật toán.

## Nguồn

- Trang chính: http://vrp.galgos.inf.puc-rio.br/index.php/en/ (nay chuyển hướng sang https://galgos.inf.puc-rio.br/cvrplib/index.php/en/).
- Tải ngày 2026-11-26, mỗi set hai gói `.7z`:
  - Set A: `.../download/instance-set/4` (`.vrp`), `.../download/instance-set-bks/4` (`.sol`)
  - Set B: `.../download/instance-set/5`, `.../download/instance-set-bks/5`
  - Set P: `.../download/instance-set/12`, `.../download/instance-set-bks/12`
  - Tiền tố đầy đủ: `https://galgos.inf.puc-rio.br/cvrplib/index.php/en/download/`
- Nội dung file giữ nguyên bản gốc, chỉ chuẩn hóa xuống dòng về LF.

## Trích dẫn gốc

Theo trang CVRPLIB, cả ba set đều ghi "Augerat et al., 1995" (A, B, P). Trang không liệt kê thêm thông tin xuất bản đầy đủ trong bảng instance.

## Quy ước

- Khoảng cách: EUC_2D, làm tròn từng cạnh theo TSPLIB (`(int)(sqrt(dx*dx+dy*dy) + 0.5)`) rồi mới cộng. Chi phí là tổng quãng đường.
- Số xe: CVRPLIB ghi "Number of Routes: Fixed" cho các set này, nhưng đề tài không ấn định trước số xe. Số `k` trong tên instance và cột `k` của `bks.csv` chỉ để tham chiếu, không phải ràng buộc.
- Depot là node 1 trong `.vrp`. Trong `.sol`, khách được đánh số 1..n-1 (không tính depot), nên khách `i` trong `.sol` ứng với node `i+1` trong `.vrp`.
- Cột `optimal` của `bks.csv` lấy từ cột "Opt" trên trang CVRPLIB ngày tải (`true` = đã chứng minh tối ưu).

## Số instance

| Set | Tải về | Giữ lại | Loại |
|-----|--------|---------|------|
| A   | 27     | 27      | 0    |
| B   | 23     | 23      | 0    |
| P   | 24     | 23      | 1    |
| Tổng| 74     | 73      | 1    |

Instance bị loại: `P-n101-k4` (DIMENSION 101, tức 100 khách). Đề tài giới hạn dưới 100 khách và parser nhận tối đa 100 node kể cả depot (99 khách).

## Kiểm tra

`bks.csv` có header `instance,set,customers,k,bks,optimal,source`, sắp theo set rồi tên. Chạy từ gốc repo:

```
python experiments/tools/check_bks.py
```

Script (chỉ dùng thư viện chuẩn) kiểm tra mỗi khách xuất hiện đúng một lần, tải mỗi tuyến không vượt CAPACITY, tính lại chi phí EUC_2D và so với dòng `Cost` trong `.sol` và cột `bks`; đồng thời kiểm tra `.vrp` trên đĩa khớp với `bks.csv`. Thoát mã khác 0 nếu có sai lệch.
