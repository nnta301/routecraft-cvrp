# Brief: phản biện tính mới và chọn biến thể cho hướng "Clarke–Wright với điểm học được"

Ngày: 2026-11-26. Brief này tự đủ: người phản biện chỉ cần đọc file này, không cần chạy thử hay tra cứu thêm.

## 1. Bối cảnh

- Đây là đề tài Project 1 ở HUST: một sinh viên làm trong khoảng một học kỳ. Sau đó còn Project 2, Project 3 và đồ án tốt nghiệp, nên đề tài nên có đường phát triển tiếp.
- Bài toán: CVRP một depot, dưới 100 khách. Khoảng cách EUC_2D làm tròn từng cạnh theo TSPLIB (`nint`). Không ấn định trước số xe.
- Đã có: lõi C++17 gồm parser `.vrp`, Nearest Neighbor có xét tải, validator độc lập, CLI xuất JSON/TXT, CI. Trước đây dự kiến cài Clarke–Wright parallel, 2-opt rồi đo đối chiếu, sau đó làm web app trực quan hóa.
- Sau buổi trình bày đầu tiên, giảng viên hướng dẫn đề nghị tìm hướng mới thay vì chỉ cài các thuật toán đã có. Lý tưởng là một thuật toán mới cho kết quả tốt, hoặc thêm một ứng dụng.
- Sinh viên chọn hướng A (mục 2). Nếu kết quả tốt thì muốn tăng dần ràng buộc: giới hạn độ dài tuyến, time window, nhiều depot.
- Tài nguyên: một máy i5-14600KF. PyVRP (giấy phép MIT, dựa trên HGS) hỗ trợ CVRP, time window, nhiều depot, đội xe không đồng nhất, giới hạn khoảng cách và thời gian, dùng được để tạo nhãn. Có thể sinh instance theo cách của bộ X (Uchoa et al. 2017). Mọi BKS của CVRPLIB set A, B, P đều đã là tối ưu.

## 2. Hướng A như đang đề xuất

Clarke–Wright parallel giữ nguyên thủ tục ghép và kiểm tra khả thi. Chỉ thay savings `s(i,j) = d(0,i) + d(0,j) − d(i,j)` bằng một điểm học được.
- Mô hình nhỏ (logistic regression hoặc gradient boosting) dự đoán xác suất cạnh (i,j) nằm trong lời giải gần tối ưu.
- Đặc trưng rẻ: savings, góc giữa i và j nhìn từ depot, demand/Q, thứ hạng láng giềng gần.
- Nhãn lấy từ lời giải PyVRP trên instance sinh kiểu X.
- Đánh giá trên CVRPLIB A, B, P. So với CW gốc, CW tham số λ đã tune, NN + 2-opt. Thêm một lượt so khi mọi phương pháp đều chạy 2-opt phía sau. Đo gap so với BKS, số xe, thời gian.

## 3. Kết quả khảo sát tài liệu

Nguồn đã xác nhận hoặc ghi rõ mức xác nhận:

- **Legrand, Accorsi, Cattaruzza, Jourdan, Kessaci, Vigo (2021).** "Using machine learning to enhance Clarke and Wright heuristic", ROADEF 2021, HAL hal-03472209. Chưa đọc được toàn văn. Theo abstract: mạng nơ-ron dự đoán cạnh, dùng để cải thiện CW, kết quả "hứa hẹn".
  Có một báo cáo thực tập M1 liên quan của Legrand, làm với Vigo và Accorsi: "About the prediction of good edges for the CVRP and its applications". Trong đó một mạng phân loại nhị phân dự đoán cạnh có nằm trong lời giải tốt không, rồi dùng xác suất để cải thiện CW và dẫn hướng local search. Chưa rõ đặc trưng, nhãn, cách tích hợp vào savings, tập đánh giá, có so với CW tham số không.
- **Hamada & Kashima (2026, arXiv:2609.35066), đã xác nhận.** Transformer 1,56 triệu tham số học chính sách thứ tự ghép, cộng thiên lệch savings CW vào logit. Huấn luyện bắt chước HGS rồi REINFORCE, trên CVRP100 phân bố đều. Đánh giá trên instance sinh theo giao thức LEHD, không dùng CVRPLIB. Baseline chỉ là CW thuần, không có CW tham số.
- **Herdianto et al. (2025, arXiv:2508.14071).** Bộ phân loại cạnh (gradient boosting, GNN) huấn luyện trên instance kiểu X, dùng để cấm move trong local search chứ không thay savings. Đặc trưng: tỉ lệ tải, mức dùng tải của tuyến, hạng láng giềng, khoảng cách chuẩn hóa. Đặc trưng tính trên cạnh của lời giải có sẵn. Precision khoảng 0,85 trên một instance 22 khách.
- **NeuroLKH (Xin et al., NeurIPS 2021).** Học điểm cạnh và tập ứng viên cho LKH.
- **Kinh điển** (theo trí nhớ, chưa xác nhận chi tiết):
  - savings có tham số: Gaskell 1967 và Yellow 1970 (λ), Paessens 1988 (λ, μ), Altinkemer & Gavish 1991;
  - CW ngẫu nhiên hóa có định hướng: Juan et al., SR-GCWS;
  - savings cho VRPTW: Solomon 1987.
- Không tìm được số liệu gap đáng tin của CW gốc hay CW tham số trên A/B/P, nên phải tự đo.

## 4. Câu hỏi cần trả lời

1. **Mức trùng.** Hướng A trùng với Legrand 2021 và Hamada & Kashima 2026 đến đâu? Tuyên bố tính mới nào còn bảo vệ được một cách trung thực trước giảng viên? Tuyên bố nào phải tránh?
2. **Biến thể khác biệt.** Đề xuất 3–5 biến thể khác biệt rõ với các công trình trên, vẫn vừa sức Project 1. Với mỗi biến thể, nêu:
   - ý tưởng và vì sao khác các công trình đã biết;
   - giả thuyết kiểm chứng được;
   - thực nghiệm tối thiểu: baseline, tập instance, chỉ số, kiểm định thống kê, ngân sách thời gian công bằng;
   - khối lượng ước tính (tuần công);
   - rủi ro và phương án dự phòng nếu kết quả âm;
   - đường mở rộng sang ràng buộc khác cho Project 2/3/đồ án.

   Dưới đây là ý tưởng thô của người viết brief. Chỉ là gợi ý, cứ phản biện, loại bỏ hoặc thay bằng ý tốt hơn:
   - (a) điểm học được nhẹ, giải thích được, có ablation đặc trưng, so công bằng với CW tham số đã tune trên cùng ngân sách;
   - (b) học cấu hình thay vì học điểm cạnh: dự đoán bộ tham số savings (λ, μ, ν) tốt nhất cho từng instance từ đặc trưng instance;
   - (c) dùng xác suất cạnh học được làm phân phối cho CW ngẫu nhiên hóa có định hướng trong multi-start, so với SR-GCWS cùng số lần chạy;
   - (d) khi không ấn định số xe: học khi nào dừng ghép hoặc đánh đổi số xe với quãng đường;
   - (e) chuyển sớm sang VRPTW, nơi savings không biết gì về thời gian nên điểm học được có nhiều chỗ để thắng hơn.
3. **Khuyến nghị.** Chọn một biến thể. Nêu tiêu chí thành công định lượng cần chốt trước khi chạy, và điều kiện để dừng hoặc chuyển hướng.
4. **Kiểm tra tính đúng.**
   - CW parallel với điểm tùy ý thay cho savings có còn luôn cho lời giải khả thi không? Có bẫy nào không?
   - Khi không ấn định số xe, CW gốc dừng ghép khi savings ≤ 0. Với điểm học được thì quy tắc dừng nên là gì để không ghép những cặp làm tăng tổng quãng đường?
   - Rủi ro lệch miền và rò rỉ dữ liệu giữa instance sinh kiểu X và CVRPLIB A/B/P.
   - BKS của A/B/P gắn với số xe k, còn đề tài không ấn định số xe, nên gap có thể âm. Nên quy ước báo cáo thế nào?
5. **Trình bày với giảng viên.** Gợi ý 2–3 câu mô tả trung thực đóng góp và tính mới của biến thể được chọn.

## 5. Yêu cầu kết quả

- Ghi vào `docs/research/novelty-review-learned-cw.md`, tiếng Việt, tối đa khoảng 1.800 từ, có mục cho từng câu hỏi ở mục 4.
- Chỗ nào là suy luận chưa kiểm chứng thì ghi rõ.
- Không nhắc tới công cụ hay trợ lý đã dùng để soạn.
