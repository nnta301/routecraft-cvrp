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

## 2026-11-26 — Sửa CI trên `main`

- Merge #28–#32 liên tiếp làm CI của 4 commit trung gian bị hủy: mọi lần push lên `main` chung một concurrency group.
  Đã chạy lại cả 4 lần, đều đạt.
- #33 (PR #34): group theo số PR với `pull_request`, theo SHA với push lên `main`; chỉ hủy lần chạy cũ trong PR.

## 2026-11-26 — Kết quả phản biện tính mới

- Kết quả: `docs/research/novelty-review-learned-cw.md` (brief: `brief-novelty-learned-cw.md`).
- Không tuyên bố ý tưởng là mới: Legrand et al. 2021 trùng gần như hoàn toàn ở mức ý tưởng.
- Điểm mấu chốt: savings có tham số đã là hàm tuyến tính của chính các đặc trưng định dùng. Vì vậy phép thử trung tâm
  là so với savings tham số được tune trực tiếp, không phải với CW gốc.
- Khuyến nghị V1: giữ CW parallel, chỉ đổi điểm xếp thứ tự, so W0 savings gốc, W1 lưới (λ, μ, ν), W2 tune trực tiếp
  trên quãng đường, W3 logistic regression trên nhãn cạnh, W4 gradient boosting nông. Đóng góp là một đánh giá có kiểm soát;
  kết quả âm vẫn có giá trị. Tiêu chí thành công và cổng dừng ở mục 3 của file, phải chốt trước khi chạy A/B/P.
- Mở rộng: V2 (CW ngẫu nhiên hóa theo điểm học được) sau V1; V3 (VRPTW) cho Project 2; V4 (bắt chước pilot method) cho Project 2 hoặc đồ án.
- Cần cân nhắc: V1 là đánh giá có kiểm soát, không phải "thuật toán mới"; cần hỏi giảng viên có đáp ứng yêu cầu không.
  Khối lượng ước tính 10–12 tuần, có thể nặng với Project 1.
- Còn thiếu: đọc toàn văn Legrand et al. 2021 (HAL hal-03472209) trước khi trình bày.
- Bước tiếp theo: trao đổi hướng với giảng viên, soạn đề cương ngắn, rồi lập kế hoạch Sprint 2.

## 2026-11-26 — Chốt hướng, đọc Legrand 2021, lập Sprint 2

- Quyết định: làm theo PA1 (đánh giá có kiểm soát); PA2 (Clarke–Wright ngẫu nhiên hóa theo điểm học) là phần mở rộng.
  Người làm đề tài tự chốt, không chờ trao đổi thêm với giảng viên. Đề cương trình giảng viên soạn riêng, ngoài repo.
- Đã đọc toàn văn Legrand et al. 2021 (HAL hal-03472209); chi tiết trong `lit-learned-savings.md` mục 1.2.
  Họ dùng MLP, đặc trưng gần như trùng, chỉ sắp lại savings trong từng đoạn (SubLists), đánh giá trên bộ X và chỉ so với CW gốc
  (Best SubLists tốt hơn khoảng 1,5%). Khác biệt bảo vệ được: đối thủ là savings tham số đã tune, so cùng ngân sách thời gian,
  kiểm tra ngoài miền trên A/B/P. Thêm SubLists làm một đối chứng ở Sprint 3.
- Giao thức thực nghiệm chốt ở `docs/experiment-protocol.md` (#26).
- Sprint 2 (`docs/sprints/sprint-02.md`): #23 ma trận khoảng cách, #38 dữ liệu CVRPLIB A/B/P (73 instance, BKS đều tối ưu, kiểm tra lại
  chi phí khớp), #39 script benchmark, #40 Clarke–Wright W0/W1, #41 kết quả baseline.
- Bước tiếp theo: kết quả baseline NN và CW W0 trên A/B/P; chế độ chạy lưới W1 trong một tiến trình; lập Sprint 3.

## 2026-11-26 — Kết quả baseline Nearest Neighbor và Clarke–Wright W0

- Kết quả: `experiments/results/baseline-nn-cw.md` (CSV cùng tên), chạy trên commit `16b177b`, đúng giao thức.
- Clarke–Wright W0: gap trung bình 5,42%, trung vị 5,23%. Nearest Neighbor: 38,05% / 37,81%.
  CW tốt hơn NN trên 73/73 instance (trung vị Δ −24,0%). m − k trung bình: CW 0,34, NN 0,08.
- Không có gap âm khi m ≤ k; mọi nghiệm qua validator; resume xác nhận không chạy lại ca đã có.
- Bước tiếp theo: chế độ chạy lưới W1 trong một tiến trình (W1-grid), rồi lập Sprint 3 (sinh instance kiểu X, gán nhãn bằng PyVRP, huấn luyện W4, tune W2, SubLists).

## 2026-11-26 — Kết quả W1-grid; BKS của A/B/P dùng đúng k xe

- Kết quả: `experiments/results/w1-grid.md` (CSV cùng tên), commit `0ae2022`.
- Gap trung bình / trung vị: W1-grid 2,00% / 2,16%; W0 5,42% / 5,23%; NN 38,05% / 37,81%.
  W1-grid tốt hơn W0 trên 72/73 instance, hòa 1 (trung vị Δ −2,47%). Thời gian trung vị của W1-grid: 459 ms mỗi instance cho 8.820 bộ.
- Bộ tham số tốt nhất rất phân tán: không bộ nào thắng quá 2 instance (λ hay gặp nhất là 1,3; μ = 0 ở 20/73; ν = 0 ở 29/73).
  Vì vậy một bộ tham số cố định (W2) có thể kém xa W1-grid; đây là mức chuẩn W4 phải tiến gần theo tiêu chí 2 của giao thức.
- Phát hiện: P-n55-k8 cho 585 < BKS 588 với 7 tuyến (k = 8). Đã kiểm chứng: 7 xe là đủ (⌈1042/160⌉ = 7), nghiệm hợp lệ, chi phí tính lại đúng;
  nghiệm BKS dùng đúng 8 tuyến. Kết luận: BKS của CVRPLIB A/B/P là tối ưu với đúng k xe. Quy tắc trong giao thức "gap âm khi m ≤ k là lỗi"
  sai; phải là "khi m = k". Sửa ở issue riêng, không cần chạy lại.
