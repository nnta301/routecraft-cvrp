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

Repository có nền tảng C++17 build ngoài source tree: thư viện tĩnh `routecraft_core`, CLI `routecraft` với `--help`/`-h`/`--version`, smoke test CTest và workflow GitHub Actions.

Lõi hiện đã có mô hình dữ liệu CVRP dùng chung, quy ước depot nội bộ `0`, lưu `original_id`, cùng tiện ích khoảng cách `EUC_2D`, tải tuyến, độ dài tuyến và tổng độ dài nghiệm. Parser `.vrp` hiện hỗ trợ một depot, `TYPE: CVRP` và `EDGE_WEIGHT_TYPE: EUC_2D`; parser kiểm tra dữ liệu đầu vào, chuẩn hóa depot về chỉ số nội bộ `0` và giữ ID nguồn.

Lõi cũng đã có heuristic Nearest Neighbor xác định, có xét tải trọng. Với instance hợp lệ, heuristic chọn khách chưa phục vụ gần nhất còn vừa tải; khi đồng khoảng cách, chọn chỉ số nội bộ nhỏ hơn. Mỗi tuyến bắt đầu/kết thúc tại depot `0`, và lõi đo riêng thời gian dựng nghiệm. Validator độc lập tính lại các số liệu từ instance và routes, phát hiện lỗi depot, chỉ số khách, phục vụ, tải trọng và tổng khoảng cách khai báo. CLI có lệnh `solve` cho Nearest Neighbor, xuất JSON hoặc TXT ra stdout. Backend, frontend, Docker, benchmark và API vẫn chưa triển khai.

Workflow `Build and test` kiểm tra cả cấu hình `Debug` và `Release`. Kết quả kiểm tra cho từng thay đổi được ghi tại PR tương ứng.

## Trạng thái hiện tại

Sprint hiện tại: [Sprint 1 — CVRP Baseline CLI](docs/sprints/sprint-01.md)

## Build và kiểm tra

Từ root repository, các lệnh sau cấu hình, biên dịch và chạy test đã đăng ký:

```text
cmake -S . -B build/Debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/Debug
ctest --test-dir build/Debug --output-on-failure

cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/Release
ctest --test-dir build/Release --output-on-failure
```

`configure` tạo cấu hình build, `build` biên dịch target, còn CTest chạy các test đã đăng ký. Dùng `Debug` cho phát triển/gỡ lỗi; `Release` là cấu hình dự kiến dùng cho thực nghiệm sau này. Với generator một cấu hình, executable nằm ở `build/<config>/bin/routecraft`; generator nhiều cấu hình thường thêm thư mục cấu hình bên dưới `bin`.

## Chạy solver Sprint 1

Sau khi build, chạy pipeline parser → Nearest Neighbor → validator → xuất kết quả:

```powershell
.\build\Debug\bin\Debug\routecraft.exe solve --input .\solver\tests\fixtures\cli-noncontiguous.vrp --method nn --format json
.\build\Debug\bin\Debug\routecraft.exe solve --input .\solver\tests\fixtures\cli-noncontiguous.vrp --method nn --format txt
```

Ví dụ dùng đường dẫn executable của cấu hình Debug với Ninja trên Windows; chọn đường dẫn output tương ứng nếu dùng generator hoặc cấu hình khác. Sprint 1 chỉ hỗ trợ `--method nn` và định dạng `json` hoặc `txt`. JSON dùng chỉ số nội bộ trong `routes`, đồng thời có `node_ids` để khôi phục ID nguồn; TXT hiển thị ID nguồn, tải và quãng đường từng tuyến. Dữ liệu kết quả chỉ được ghi ra stdout sau khi validator xác nhận nghiệm; lỗi được ghi ra stderr.

Các regression CTest của Sprint 1 dùng fixture nhỏ trong `solver/tests/fixtures/` và chạy luồng CLI thực tế cho JSON/TXT. Baseline chỉ có Nearest Neighbor, không giới hạn số xe `K`, không có pha cải thiện và không phải benchmark hoặc phép đo hiệu năng.

## Tài liệu

- [Đề xuất nghiên cứu](docs/proposal.md)
- [Yêu cầu](docs/requirements.md)
- [Kiến trúc dự kiến](docs/architecture.md)
- [Hợp đồng dữ liệu v0](docs/data-contract.md)
- [Phát triển và môi trường](docs/development.md)
- [Lộ trình](docs/roadmap.md)
- [Quy ước dữ liệu](data/README.md)
