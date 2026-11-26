# Khảo sát tài liệu: học điểm savings cho Clarke–Wright (CVRP)

Quy ước: "đã xác nhận" = đã mở được nguồn (trang web/bản tóm tắt) và đọc nội dung liên quan. "Chưa xác nhận" = chỉ có trích dẫn gián tiếp hoặc từ trí nhớ, cần kiểm tra lại trước khi đưa vào bài.

## 1. Tính mới

### 1.1 Kết luận
Ý tưởng cốt lõi đã có tiền lệ: **Legrand và cộng sự (ROADEF 2021)** dùng MLP dự đoán cạnh tốt rồi sắp lại savings trong từng đoạn của danh sách (đã đọc toàn văn ngày 2026-11-26, xem 1.2). Đề tài không tuyên bố ý tưởng là mới. Đóng góp là một đánh giá có kiểm soát: so với savings tham số đã tune, cùng ngân sách thời gian, trên A/B/P (xem `novelty-review-learned-cw.md`).

### 1.2 Công trình gần nhất
1. **Legrand, Accorsi, Cattaruzza, Jourdan, Kessaci, Vigo (2021), "Using machine learning to enhance Clarke and Wright heuristic", ROADEF 2021, Mulhouse (HAL hal-03472209; tên file PDF ghi "Improving Clarke and Wright Heuristic with Machine Learning"). Cũng có talk EURO 2021 cùng tên "Improving Clarke and Wright Heuristic with Machine Learning".**
   - Trạng thái: **đã đọc toàn văn** bản HAL (2 trang) ngày 2026-11-26; bản PDF lưu cục bộ, không commit.
   - Mô hình: MLP (scikit-learn), phân loại nhị phân cạnh tốt hoặc xấu.
   - Đặc trưng (theo luận án Arnold 2018): chi phí cạnh d(i,j); nhu cầu của i và j; khoảng cách từ i (j) tới láng giềng gần nhất và nhu cầu của láng giềng đó; khoảng cách từ i (j) tới depot; góc i–depot–j; có chuẩn hóa.
   - Nhãn: trên bộ X (Uchoa et al. 2017), một nửa để huấn luyện, nửa còn lại để kiểm định. Sinh nhiều lời giải bằng cách nhiễu BKS, mỗi lời giải có trọng số chất lượng 0–1; trọng số cạnh là tổng chuẩn hóa trọng số các lời giải chứa nó. Cạnh "tốt" nếu trọng số > 0,5, "xấu" nếu < 0,2, bỏ khoảng giữa; giảm mẫu cạnh xấu với p ∈ {0,005; 0,01; 0,02; 0,05}.
   - Tích hợp (SubLists): sắp savings gốc, chia thành các đoạn dài l nhưng giữ thứ tự giữa các đoạn, trong mỗi đoạn sắp theo xác suất giảm dần (l = 1 là CW gốc). Best SubLists thử một danh sách giá trị l và lấy nghiệm tốt nhất.
   - Đánh giá: chạy hill climbing 2-opt sau mỗi phương pháp. SubLists nhỉnh hơn CW gốc; Best SubLists tốt hơn khoảng 1,5% trung bình nhưng tốn nhiều tính toán hơn. Chỉ so với CW gốc; không có savings tham số, không dùng A/B/P, không bàn về số xe.
   - Khác đề tài: (1) đối thủ chính là savings tham số đã tune (W1, W2), không phải CW gốc; (2) so cùng ngân sách thời gian, trong khi Best SubLists là lấy tốt nhất trong nhiều lần chạy; (3) kiểm tra ngoài miền trên A/B/P; (4) điểm học được thay hoàn toàn savings (W4), còn SubLists chỉ là một đối chứng. Đặc trưng gần như trùng, nên không tuyên bố đặc trưng mới.
2. **Hamada & Kashima (2026), "Depot-Closed Multi-Component Construction for Neural Vehicle Routing", arXiv:2609.35066.** https://arxiv.org/abs/2609.35066 (đã xác nhận trang tóm tắt).
   - Chính sách nơ-ron giữ nhiều thành phần tuyến mở và gộp theo thứ tự tùy ý; mức lợi khi gộp đúng bằng saving Clarke–Wright. Huấn luyện RL trên CVRP100, đánh giá CVRP100-500.
   - Đã xác nhận chi tiết (bản HTML https://arxiv.org/html/2609.35066, tác giả Shinichiro Hamada, Hisashi Kashima, nộp 28/9/2026): Transformer kiểu LEHD ~1.56M tham số, chấm điểm cặp đầu mút, cộng thiên lệch logit theo saving CW (alpha x saving chuẩn hóa), softmax có mặt nạ khả thi dung lượng. Huấn luyện: giai đoạn giám sát bắt chước (1M instance CVRP100 đồng nhất giải bằng HGS, C=50) rồi REINFORCE. Đánh giá: CVRP100/200/500/1000 sinh theo giao thức LEHD (tham chiếu LKH3) và thí nghiệm độ chặt dung lượng C=10..500; KHÔNG dùng CVRPLIB. Baseline CW là parallel CW thuần (gap trung bình 7.83% so LKH3 trên CVRP100, greedy), không có CW tham số. Nêu rõ Legrand 2021 là "điều chỉnh thứ tự ưu tiên trong danh sách savings" và đối lập với cách của họ.
   - Khác: mô hình nơ-ron end-to-end học chính sách gộp, không phải mô hình nhỏ xếp hạng lại savings dựa trên nhãn từ lời giải HGS; không phải heuristic CW giữ nguyên.
3. **Herdianto, Billot, Lucas, Sevaux, Vigo (2025), "Edge-Selector Model Applied for Local Search Neighborhood for Solving Vehicle Routing Problems", arXiv:2508.14071.** https://arxiv.org/html/2508.14071 (đã xác nhận phần đã đọc).
   - Bộ phân loại cạnh (gradient boosting / MLP / GNN) gán nhãn cạnh của lời giải ban đầu là bị cấm trong local search (như tabu), áp dụng cho FILO2 và HGS; dữ liệu XML100 (sinh theo kiểu bộ X) và các instance X, B, Gehring–Homberger. Có dùng savings để tạo lời giải không tối ưu làm dữ liệu huấn luyện.
   - Đặc trưng (Phụ lục A.1, bản tabular): % tải (q_i+q_j)/tổng cầu; mức dùng tải của tuyến chứa cạnh (q_i+q_j)/cầu tuyến; hạng láng giềng R_ij nếu trong ngưỡng granularity Gamma, ngược lại -1; khoảng cách d_ij chuẩn hóa. GNN: đỉnh (x,y,q/Q), cạnh d_ij làm tròn. Chỉ thấy báo cáo precision (ví dụ 22 TP, 4 FP, 0.846 trên instance 22 khách); accuracy/recall/AUC chưa thấy trong phần đã đọc (chưa đọc 31k ký tự cuối). Ngưỡng quyết định 0.75-0.85 (cắt cạnh tin cậy cao), có biến thể ngẫu nhiên (t=0.8, p=0.9/0.75). Lưu ý: đặc trưng là của cạnh trong lời giải có sẵn, cần điều chỉnh để dùng cho cạnh ứng viên (i,j) trước khi dựng.
   - Khác: học để cắt hàng xóm của local search, không thay savings trong bước dựng. Nhưng gần về phương pháp (đặc trưng cạnh, GBT, nhãn từ lời giải tốt, instance kiểu X).
4. **Tuning a parametric Clarke–Wright heuristic via a genetic algorithm** (Altınel–Öncan mở rộng; chỉ thấy trang ResearchGate, https://www.researchgate.net/publication/31941405). Chưa xác nhận tác giả, năm, venue. Tối ưu tham số công thức savings bằng GA trên từng instance/tập, không dự đoán xác suất cạnh.
5. **Learnheuristics** (Juan và cộng sự; ví dụ "On the Use of Learnheuristics in Vehicle Routing Optimization Problems with Dynamic Inputs", https://www.researchgate.net/publication/329746698). Chỉ thấy tên trang; chưa xác nhận nội dung chi tiết. Hướng: kết hợp ML với heuristic dựa CW cho đầu vào động, không phải học điểm savings.
6. **Machine Learning Constructives and Local Searches for the Travelling Salesman Problem, arXiv:2108.00938** (https://arxiv.org/pdf/2108.00938). Chỉ xem mô tả trong kết quả tìm kiếm: ML dự đoán xác suất cạnh thuộc tour tối ưu, ngưỡng để chèn cạnh, sau đó CW hoàn thiện. Thuộc TSP, chưa xác nhận tác giả/năm. Gần về ý tưởng "xác suất cạnh + CW".

### 1.3 Khoảng trống còn lại (giả thuyết, cần kiểm chứng)
- Đối chiếu công khai, tái lập được: mô hình nhỏ (LR/GBT) làm điểm thay savings trong CW parallel, đánh giá trên CVRPLIB A/B/P với baseline CW gốc, CW tham số, NN+2-opt, nhãn từ PyVRP/HGS.
- Phân tích đặc trưng nào đóng góp (ablation), tính khái quát sang instance khác phân phối, mở rộng sang ràng buộc khác (độ dài tuyến, TW, nhiều depot).
- Các điểm này chưa được tìm thấy trong nguồn đã mở, nhưng việc không tìm thấy không chứng minh chưa ai làm. Nên thử thêm: Google Scholar/HAL/ResearchGate cho tên tác giả Legrand, Accorsi, Vigo; liên hệ tác giả.

### 1.4 Các cách tìm đã dùng
"machine learning Clarke-Wright", "learned savings", "learning to rank savings", "neural Clarke Wright", edge prediction constructive heuristic CVRP. Các tìm kiếm cho kết quả giống nhau (Legrand 2021, Hamada 2026). Chưa thử riêng "data-driven savings", "ML-guided construction heuristic VRP" bằng cơ sở dữ liệu học thuật (Scopus, Web of Science); công cụ tìm hiện tại giới hạn.

## 2. Hướng liền kề

- **NeuroLKH** (Xin, Song, Cao, Zhang, NeurIPS 2021, vol. 34, tr. 7472-7483). https://proceedings.neurips.cc/paper/2021/file/3d863b367aa379f71c7afc0c9cdca41d-Paper.pdf (đã xác nhận qua tìm kiếm/repo https://github.com/liangxinedu/neurolkh). Mạng đồ thị thưa học điểm cạnh (giám sát) và phạt nút (không giám sát) để tạo tập cạnh ứng viên cho LKH; trên TSP100 tập ứng viên học được bỏ sót 0.05% cạnh tối ưu so với 0.68% của LKH (số lấy từ kết quả tìm kiếm, chưa đối chiếu bản PDF). Chỉ TSP.
- **Problem reduction / candidate set cho VRP:** Herdianto và cộng sự 2025 (mục 1.2). Một bài khác nhắc tới: "Learning to Reduce Search Space for Generalizable Neural Routing Solver", arXiv:2503.03137 (chỉ thấy tiêu đề, chưa xác nhận nội dung).
- **Savings có tham số:** (chưa mở được nguồn gốc từng bài, ghi theo trích dẫn kinh điển, cần kiểm tra lại)
  - Gaskell (1967), "Bases for vehicle fleet scheduling", Operational Research Quarterly 18, và Yellow (1970), "A computational modification to the savings method of vehicle scheduling", Operational Research Quarterly 21: thêm tham số λ nhân d(i,j) và μ nhân |d(0,i)−d(0,j)|.
  - Paessens (1988), "The savings algorithm for the vehicle routing problem", EJOR 34. Chưa xác nhận chi tiết.
  - Altınel & Öncan (2005), "A new enhancement of the Clarke and Wright savings heuristic for the capacitated vehicle routing problem", JORS 56, 954-961, DOI 10.1057/palgrave.jors.2601916 (https://link.springer.com/article/10.1057/palgrave.jors.2601916; đã xác nhận trích dẫn): thêm số hạng theo nhu cầu, bộ tham số (λ, μ, ν).
  - Altinkemer & Gavish (1991), "Parallel savings based heuristics for the delivery problem", Operations Research 39. Chưa xác nhận.
- **CW ngẫu nhiên hóa:** Juan, Faulin, Jorba, Riera, Masip, Barrios (2011), "On the use of Monte Carlo simulation, cache and splitting techniques to improve the Clarke and Wright savings heuristics", JORS 62, 1085-1097, DOI 10.1057/jors.2010.29 (https://link.springer.com/article/10.1057/jors.2010.29; đã xác nhận). SR-GCWS-CS: chọn cặp gộp ngẫu nhiên theo phân phối hình học trên danh sách savings. Ngoài ra Pichpibul & Kawtummachai ghi nhận xếp hạng ngẫu nhiên (chưa xác nhận trích dẫn).
- **Savings cho VRPTW:** Solomon (1987), "Algorithms for the vehicle routing and scheduling problems with time window constraints", Operations Research 35(2), 254-265. Chưa xác nhận qua nguồn mở; trích dẫn kinh điển.

## 3. Mức kết quả tham chiếu

- Không tìm được nguồn đáng tin có con số gap CW gốc / CW tham số trên CVRPLIB set A, B, P. Một nguồn thứ cấp (chương InTechOpen 72140, https://www.intechopen.com/chapters/72140) cho thấy CW thuần có gap khoảng 3-10% trên một số instance A-n (ví dụ 5.80% A-n37-k5, 9.73% A-n39-k5; chưa xác nhận bài nguồn và cách tính).
- Kiến thức chung (chưa xác nhận, nên tự đo): CW thuần thường cách BKS vài phần trăm đến khoảng 5-8% trên instance nhỏ-vừa; CW tham số (Altınel-Öncan, tune bằng GA) cải thiện được vài phần trăm. Khuyến nghị: tự đo baseline trên A/B/P bằng chính code CW của đề tài, vì BKS các set này đều là tối ưu.
- Với instance này, khoảng cách làm tròn (TSPLIB EUC_2D) cần dùng thống nhất khi so BKS.

## 4. Công cụ

- **PyVRP** (Wouda, Lan, Kool, "PyVRP: A High-Performance VRP Solver Package", INFORMS Journal on Computing 36(4), 943-955, 2024, DOI 10.1287/ijoc.2023.0055; arXiv:2403.13795). Trang https://github.com/PyVRP/PyVRP (đã xác nhận qua WebFetch).
  - Biến thể: time window + service duration + release time; nhiều depot; multi-trip với reload depot; đội xe không đồng nhất (khác dung lượng, chi phí, shift duration, routing profile, giới hạn khoảng cách/thời gian); pickup-delivery/backhaul; khách tùy chọn có prize (prize-collecting, team orienteering); nhóm loại trừ lẫn nhau. Mục "Enterprise" (break planning, tương thích, công bằng, clustering mềm) là bản trả phí.
  - Giấy phép: MIT (đã xác nhận; bản R `vrpr` cũng MIT + file LICENSE).
  - CVRP không ấn định số xe: trang chính không giải thích rõ cách mô hình. Chưa xác nhận trong tài liệu; theo hiểu biết chung, đặt số xe khả dụng (num_available của vehicle type) đủ lớn và chi phí cố định xe bằng 0 là cách thường làm. Cần kiểm tra trên docs https://pyvrp.org (trang introduction_to_vrp trả 404 khi thử đường dẫn /stable/setup/..., nhưng bản 0.13.4 tồn tại).
  - Hiệu năng: bài báo nêu kết quả hàng đầu cho VRPTW và CVRP (đã xác nhận qua tóm tắt).
- **BKS CVRPLIB** (đã xác nhận): https://galgos.inf.puc-rio.br/cvrplib/index.php/en/ . Set A, B, P ghi tác giả Augerat và cộng sự 1995 (osti.gov biblio/289002), tất cả đánh dấu Opt = yes. Tải: instance `/download/instance/<id>`; file lời giải cả bộ `/download/instance-set-bks/4` (A), `/5` (B), `/12` (P); từng instance `/download/bks/<id>`. Ví dụ UB: A-n32-k5 = 784, B-n31-k5 = 672, P-n16-k8 = 450. Bộ X: Uchoa và cộng sự (2017), "New benchmark instances for the Capacitated Vehicle Routing Problem", EJOR 257(3) (chưa xác nhận trong lần tra này).
  - Lưu ý: số xe trong tên instance (k5...) là gợi ý; đề tài không ấn định số xe nên cần so BKS theo tổng quãng đường, và nêu rõ số xe của lời giải tối ưu (có thể khác k).

## 5. Rủi ro và việc tiếp theo
1. Tính mới trùng với Legrand 2021: cần tìm bản đầy đủ/liên hệ tác giả trước khi viết kết luận.
2. Baseline CW tham số mạnh có thể thu hẹp lợi ích của mô hình học; cần tune λ công bằng.
3. Nhãn từ HGS với tập nhỏ (<100 khách) và A/B/P có phân phối khác bộ X, nguy cơ lệch miền khi huấn luyện/đánh giá.
