# Research log

## 2026-11-26 — Review toàn repo

- Đã làm: review lõi C++, test/CMake/CI và tài liệu ở commit `7845677`; build và chạy ctest Debug/Release trên Windows.
- Kết quả: `docs/research/review-2026-11-26.md`. Log build/test thô ở `runtime-output/review/` (không commit).
- Tóm tắt: lõi Sprint 1 về cơ bản đúng, build 0 warning, 8/8 test pass khi PATH đúng.
  Từ Git Bash, 4 test fail do nạp nhầm `libstdc++-6.dll` của Git for Windows.
  Cần sửa trước Sprint 2: validator chấp nhận route rỗng, hai đường tính khoảng cách, link tĩnh runtime MinGW,
  tài liệu Sprint 1 lỗi thời, chưa có giao thức thực nghiệm (gap/BKS/số xe).
- Bước tiếp theo: chờ người dùng chọn sửa nhóm nào (code / tài liệu / giao thức nghiên cứu).

## 2026-11-26 — Chọn hướng nghiên cứu, khảo sát tài liệu

- Bối cảnh: sau buổi trình bày đầu, giảng viên đề nghị tìm hướng mới thay vì chỉ cài thuật toán đã có.
  Người làm đề tài chọn hướng A: Clarke–Wright với điểm học được thay cho savings. Nếu kết quả tốt sẽ tăng dần ràng buộc
  (giới hạn độ dài tuyến → time window → nhiều depot).
- Khảo sát tài liệu: `docs/research/lit-learned-savings.md`. Ý tưởng gốc không còn mới:
  - Legrand et al. (ROADEF 2021) đã dùng mạng nơ-ron dự đoán cạnh để cải thiện CW, nhưng chưa đọc được toàn văn;
  - Hamada & Kashima (arXiv:2609.35066) dùng transformer học thứ tự ghép, có thiên lệch theo savings, chỉ so với CW thuần.
  Khoảng trống có thể còn (chưa kiểm chứng): baseline CW tham số tune công bằng, đánh giá trên CVRPLIB A/B/P,
  phân tích đặc trưng, mở rộng ràng buộc.
- Phản biện sâu: brief `docs/research/brief-novelty-learned-cw.md`, kết quả sẽ ghi ở `docs/research/novelty-review-learned-cw.md`.
  Lý do: hướng A trùng một phần với công trình đã có, cần chọn biến thể có tuyên bố tính mới trung thực trước khi trình giảng viên.

## 2026-11-26 — Quy trình và các nhánh sửa lỗi Sprint 1

- Quy trình: issue → nhánh `<type>/<số-issue>-<mô-tả>` → PR tiếng Việt → CI Debug/Release → squash merge
  (quy ước đầy đủ trong `CONTRIBUTING.md`).
- Issue đang làm:
  - #17: đưa quy ước làm việc vào `CONTRIBUTING.md`.
  - #18: validator từ chối tuyến `[0]` / `[0, 0]`, kèm test.
  - #19: link tĩnh libgcc/libstdc++ khi dùng MinGW; ctest 8/8 pass từ Git Bash mà không cần chỉnh PATH.
  - #20: thư mục `docs/research` này.
  - #27: đóng Sprint 1.
- Tồn đọng từ bản review: #21 BOM UTF-8, #22 `solve --help`, #23 ma trận khoảng cách, #24 kiểm thử chặt hơn,
  #25 CI Windows/sanitizer, #26 giao thức thực nghiệm.
- Thứ tự merge: #17, #18, #19, #20 (không phụ thuộc nhau) → #27 (liên kết tới báo cáo review và các issue sửa lỗi).
- Bước tiếp theo: đọc kết quả phản biện tính mới, chọn biến thể, soạn đề cương trình giảng viên, rồi lập kế hoạch Sprint 2.
