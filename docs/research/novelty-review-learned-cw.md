# Phản biện tính mới: Clarke–Wright với điểm học được

Ngày 2026-11-26. Trả lời các câu hỏi ở mục 4 của `brief-novelty-learned-cw.md`. **[CKC]** = cần kiểm chứng (suy luận, hoặc trích dẫn theo trí nhớ chưa đối chiếu nguồn).

## 1. Mức trùng và tuyên bố được phép

- **Legrand et al. 2021:** trùng gần như hoàn toàn ở mức ý tưởng: phân loại cạnh tốt rồi dùng xác suất để cải thiện CW. Mọi khác biệt có thể nêu (mô hình nhẹ, đánh giá trên A/B/P, có baseline CW tham số) đều phụ thuộc vào chi tiết chưa đọc được [CKC]. Phải đọc toàn văn hoặc báo cáo M1, hoặc hỏi tác giả, trước khi viết bất kỳ câu so sánh nào.
- **Hamada & Kashima 2026:** trùng ở mục tiêu (học thứ tự ghép, giữ thủ tục ghép, savings làm thiên lệch). Khác ở phương pháp (transformer, bắt chước HGS rồi REINFORCE, chính sách động theo trạng thái) và ở đánh giá (không dùng CVRPLIB, baseline chỉ là CW thuần).
- **Điểm mấu chốt:** savings có tham số, ví dụ s = d0i + d0j − λ·dij + μ·|d0i − d0j| + ν·(qi + qj)/q̄ (dạng ba tham số thường gán cho Altinkemer & Gavish 1991 [CKC]), đã là một hàm tuyến tính của chính các đặc trưng mà hướng A định dùng. Logistic regression trên các đặc trưng đó là cùng họ hàm, chỉ khác cách đặt trọng số: theo nhãn cạnh thay vì theo hàm mục tiêu. Vì vậy so với CW tham số đã tune là phép thử trung tâm, không phải baseline phụ. Việc tune trực tiếp tham số savings cũng đã có tiền lệ (Battarra, Golden & Vigo 2008; Corominas et al. 2010) [CKC].

**Bảo vệ được** (nếu số liệu ủng hộ): một đánh giá có kiểm soát giữa điểm học được và savings tham số tune trên cùng họ hàm, cùng ngân sách, trên A/B/P, khi số xe tự do; tách được phần lợi do nhãn, do độ biểu diễn của mô hình và do tune; ablation đặc trưng; quy ước đo khi số xe tự do (mục 4.4).

**Phải tránh:** "thuật toán mới"; "lần đầu dùng học máy cho CW"; "đặc trưng mới" (Herdianto et al. 2025 dùng gần như cùng bộ); "tốt hơn phương pháp học sâu" (không so); "tổng quát hóa tốt" khi chưa kiểm tra ngoài miền; mọi câu so sánh với Legrand khi chưa đọc toàn văn.

## 2. Biến thể khác biệt

Gợi ý (a) giữ nhưng đổi trọng tâm (V1); (c) thành V2; (e) thành V3; thêm V4; (b) và (d) bị loại (cuối mục).

### V1. Một họ hàm điểm, bốn cách đặt trọng số (từ a)
- **Ý tưởng:** cố định CW parallel và quy tắc chấp nhận (mục 4.2), chỉ đổi điểm dùng để xếp thứ tự. W0: savings gốc. W1: (λ, μ, ν) theo lưới trong tài liệu. W2: trọng số tuyến tính tối ưu trực tiếp trên tổng quãng đường của tập huấn luyện (random search hoặc CMA-ES; mỗi lần chạy CW chỉ cỡ mili-giây). W3: logistic regression trên nhãn cạnh. W4: gradient boosting nông, thêm góc, hạng láng giềng, tương tác với đặc trưng instance (tùy chọn hàm mất mát xếp hạng). Khác các công trình đã biết ở chỗ tách được ba nguồn lợi: nhãn (W3 so với W2), độ biểu diễn (W4 so với W3), tune (W2 so với W1).
- **Giả thuyết:** H1: W4 cho quãng đường ngắn hơn W2 trên A/B/P. H2 (hai phía): W3 khác W2, tức là phân loại cạnh có phải mục tiêu thay thế tốt hay không. H3: lợi thế của H1 còn sau local search.
- **Thực nghiệm tối thiểu:** khoảng 2.000 instance sinh kiểu X (n từ 20 đến 99) để huấn luyện, 300 để kiểm định, 500 để kiểm tra trong miền. A/B/P (74 instance [CKC]) là tập kiểm tra ngoài miền, chỉ chạy một lần với cấu hình đã đóng băng. Baseline thêm: best-of-grid CW tham số trong cùng thời gian trực tuyến với W4 (tính cả đặc trưng và suy luận); NN + 2-opt. Chỉ số: quãng đường, chênh lệch tương đối theo cặp so với W2, gap (mục 4.4), số xe, thời gian. Kiểm định: Wilcoxon signed-rank theo cặp instance, hiệu chỉnh Holm, bootstrap CI cho trung vị, đếm thắng/hòa/thua. Ngân sách ngoại tuyến công bằng: W2 được số giờ CPU bằng thời gian gán nhãn và huấn luyện của W3/W4. Để kiểm tra H3 nên thêm relocate và swap giữa tuyến (khoảng 1 tuần), vì 2-opt trong tuyến quá yếu [suy luận].
- **Khối lượng:** 10–12 tuần công (CW + savings tham số 1,5; bộ sinh + gán nhãn 1,5; đặc trưng, huấn luyện, xuất mô hình sang C++ 2; tune baseline 1; local search 1–1,5; thực nghiệm 2; viết 1,5–2).
- **Rủi ro, dự phòng:** W4 ≈ W2 vẫn là kết quả có giá trị ("savings tham số đã tune là baseline mạnh mà các công trình học CW bỏ qua"), kèm ablation, rồi chuyển sang V2. Lợi thế mất sau local search: báo cáo trung thực. Lợi thế mất trên A/B/P: phân tích lệch miền (mục 4.3).
- **Mở rộng:** giới hạn độ dài tuyến (kiểm tra khả thi vẫn đơn điệu, mục 4.1); time window (V3); nhiều depot (gán khách cho depot rồi CW từng depot, học cả bước gán).

### V2. CW ngẫu nhiên hóa có định hướng theo điểm học được (từ c)
- **Ý tưởng:** SR-GCWS chọn phép ghép kế tiếp theo phân phối gần-hình-học trên vị trí trong danh sách savings [CKC]. V2 dùng danh sách xếp theo W4, hoặc lấy mẫu softmax trên logit với nhiệt độ T (lúc này độ hiệu chỉnh của xác suất mới có ý nghĩa). Khác Hamada: mô hình nhẹ, so với SR-GCWS đã tune. Cần xem Hamada có báo cáo kết quả lấy mẫu nhiều lần không [CKC].
- **Giả thuyết:** với K ∈ {10, 100, 1.000} lần chạy, best-of-K của V2 thấp hơn SR-GCWS (β đã tune) và thấp hơn ngẫu nhiên hóa trên danh sách W2.
- **Thực nghiệm:** cùng tập như V1; 10 seed mỗi instance, lấy trung bình best-of-K theo seed rồi Wilcoxon theo instance; vẽ đường anytime; cùng thời gian tường, có và không có local search.
- **Khối lượng:** thêm 3–4 tuần sau V1. **Rủi ro:** K lớn thì ngẫu nhiên hóa san bằng khác biệt về thứ tự; dự phòng là báo điểm giao nhau trên đường anytime. **Mở rộng:** multi-start + local search thành metaheuristic nhẹ cho VRPTW, nhiều depot.

### V3. Điểm học được cho VRPTW (từ e)
- **Ý tưởng, giả thuyết:** savings không biết thời gian; đặc trưng độ chồng cửa sổ, độ chùng, thời gian chờ giúp giảm số xe và quãng đường so với savings của Solomon và phép chèn I1 (heuristic xây dựng cổ điển mạnh nhất cho VRPTW).
- **Thực nghiệm:** Solomon bản 25 và 50 khách; mục tiêu từ vựng (số xe rồi quãng đường) chốt trước; nhãn PyVRP có TW; baseline savings-TW và I1.
- **Khối lượng:** 12–14 tuần nếu làm ngay (parser, kiểm tra khả thi TW có hướng, validator mới); 6–8 tuần nếu làm sau V1. **Rủi ro:** thắng savings-TW quá dễ, phải so với I1; tuyến không còn đảo chiều tự do. Hợp với Project 2, không hợp Project 1. **Mở rộng:** TW nhiều depot, đội xe không đồng nhất.

### V4. Điểm động bắt chước pilot method (mới)
- **Ý tưởng:** pilot method (Voß, Fink & Duin 2005 [CKC]) chọn phép ghép bằng cách hoàn tất thử bằng CW cho các ứng viên đầu danh sách; đắt cỡ m·n lần CW nhưng nhìn trước được. Học một điểm nhẹ với đặc trưng trạng thái (tải còn lại, độ dài tuyến hiện tại) để bắt chước lựa chọn của pilot: nhãn tự sinh, không cần PyVRP, nhất quán với chính thủ tục CW. Khác Hamada ở nguồn nhãn và kích thước mô hình.
- **Giả thuyết:** đóng được ít nhất 50% khoảng cách từ CW tới pilot-CW với thời gian không quá 5 lần CW. **Thực nghiệm:** như V1, thêm pilot-CW làm cận trên.
- **Khối lượng, rủi ro:** trên 12 tuần; dữ liệu phụ thuộc trạng thái nên cần thu thập kiểu DAgger. Pilot-CW tự nó là kết quả dự phòng. **Mở rộng:** hợp Project 2 hoặc đồ án.

### Loại bỏ
- **(b) Học cấu hình theo instance:** với n < 100, chạy cả lưới vài trăm cấu hình mất chưa tới một giây, nên dự đoán chỉ tiết kiệm vài mili-giây, lại trùng mảng tune CW tham số đã có. Giữ làm baseline best-of-grid.
- **(d) Học khi nào dừng ghép:** đặt sai bài toán khi mục tiêu chỉ là quãng đường. Mọi lời giải cuối còn một phép ghép khả thi có savings > 0 đều bị trội; theo bất đẳng thức tam giác thì savings ≥ 0 (≥ −1 do làm tròn), nên CW dừng vì tải chứ không vì dấu savings. Lợi ích thật của học nằm ở việc hoãn một phép ghép, tức là thứ tự, vốn đã thuộc V1. Chỉ có nghĩa khi mục tiêu có chi phí xe.

## 3. Khuyến nghị

Chọn **V1**; V2 là phần mở rộng nếu qua cổng 1 trước tuần 7. Lý do: tuyên bố tính mới trung thực (đóng góp là một đánh giá có kiểm soát, không phải ý tưởng); vừa sức; kết quả âm vẫn có giá trị; hạ tầng (bộ sinh instance, gán nhãn, CW nhận điểm tùy ý, quy ước đo) dùng lại được cho V2–V4.

**Tiêu chí thành công, chốt và ghi lại trước khi chạy A/B/P** (ngưỡng là đề xuất, có thể chỉnh sau pilot trên tập sinh):
1. Chính: W4 so với W2, không local search, trên A∪B∪P: trung vị cải thiện tương đối theo instance ≥ 0,5%, Wilcoxon một phía p < 0,05 sau Holm, thắng ≥ 60% số instance không hòa.
2. Giá trị thực tế: W4 không kém best-of-grid cùng thời gian quá 0,2% (trung vị). Nếu kém hơn thì kết luận là ở cỡ n < 100, chạy lưới đã đủ tốt.
3. Bền vững: sau 2-opt + relocate + swap, trung vị cải thiện ≥ 0,3%, p < 0,05.
4. Nhất quán trên 500 instance trong miền. Với 74 instance chuẩn (không phải mẫu ngẫu nhiên), p-value chỉ mang tính mô tả.

Ngưỡng 0,5% dựa trên suy luận rằng chênh lệch giữa các biến thể CW thường cỡ 1–3% [CKC].

**Dừng hoặc chuyển hướng:**
- Cổng 1 (tuần 5–6, chỉ dùng tập kiểm định sinh): W4 không hơn W2 ít nhất 0,3% thì cho tối đa một tuần thử thêm đặc trưng, rồi khóa mô hình, viết kết quả âm kèm ablation, chuyển sang V2 hoặc web app.
- Cổng 2: tốt trong miền nhưng mất trên A/B/P thì thử đúng một lần với bộ sinh khớp A/B/P (khai báo rõ), không thử thêm.
- Không bao giờ tune trên A/B/P. Chỉ chạy lại A/B/P khi sửa lỗi, có ghi log.

## 4. Kiểm tra tính đúng

### 4.1. Tính khả thi với điểm tùy ý
Luôn khả thi nếu: mọi qi ≤ Q (kiểm tra ngay khi đọc instance), khởi đầu mỗi khách một tuyến, và phép thử ghép giữ nguyên (khác tuyến, i và j đều là đầu mút, tổng tải ≤ Q). Điểm chỉ đổi thứ tự xét; quy nạp theo số phép ghép cho thấy mỗi bước giữ "mỗi khách đúng một lần" và "tải ≤ Q".

Tính không khả thi là đơn điệu: khách đã là điểm trong thì không thành đầu mút lại, tải chỉ tăng, đã cùng tuyến thì mãi cùng tuyến. Do đó một lượt duyệt danh sách đầy đủ, theo thứ tự bất kỳ, cho lời giải tối đa theo ghép: không còn phép ghép khả thi nào có savings > 0. Với giới hạn độ dài tuyến, tính đơn điệu vẫn đúng dưới bất đẳng thức tam giác (L1 + L2 − sij ≥ max(L1, L2) vì sij ≤ 2·min(d0i, d0j)); làm tròn `nint` có thể lệch 1 đơn vị [CKC bằng test].

Bẫy:
- Nhận mọi phép ghép khả thi bất kể savings: quãng đường có thể tăng (mục 4.2).
- Cắt danh sách ứng viên (k láng giềng, hoặc p > τ) vẫn khả thi nhưng mất tính tối đa theo ghép; cần nối phần bị cắt, xếp theo savings gốc, vào cuối danh sách.
- `std::sort` cần thứ tự yếu chặt; NaN gây hành vi không xác định. Dùng khóa toàn phần (điểm, savings, min(i,j), max(i,j)). Suy luận ở Python và C++ có thể lệch bit cuối làm đổi thứ tự: chỉ tính điểm ở một nơi, hoặc lượng tử hóa điểm.
- Tọa độ A/B cỡ [0, 100] nên savings nguyên hòa nhau rất nhiều; điểm học được có thể "thắng" chỉ nhờ phá hòa. Đo mức nhiễu bằng CW phá hòa ngẫu nhiên với nhiều seed, rồi so lợi thế với mức nhiễu đó.
- Đặc trưng bất đối xứng (hạng của j theo i khác hạng của i theo j): đối xứng hóa để mỗi cặp có một điểm.
- Khi sang TW: đảo chiều tuyến không còn miễn phí, phải chứng minh lại tính đơn điệu [CKC].

### 4.2. Quy tắc dừng khi không ấn định số xe
Điểm học được chỉ dùng để xếp thứ tự; nhận phép ghép khi khả thi và savings thật sij > 0, tính trên khoảng cách đã làm tròn đúng như hàm mục tiêu. Khi đó quãng đường giảm đơn điệu, kết quả không tệ hơn lời giải mỗi khách một tuyến, và theo 4.1 không bỏ sót phép ghép có lợi nào. sij = 0: nhận nếu quy ước ít xe là mục tiêu phụ (ghi rõ). sij = −1 (depot gần thẳng hàng giữa i và j, do làm tròn): từ chối. Không dùng ngưỡng xác suất làm quy tắc dừng: vừa có thể bỏ sót phép ghép có lợi, vừa có thể nhận phép ghép có hại. Muốn tận dụng độ tin cậy thì chạy hai pha: các cặp p ≥ τ theo điểm, rồi phần còn lại theo savings gốc, cả hai pha đều yêu cầu sij > 0. Hệ quả: CW chỉ dùng thứ tự nên mọi biến đổi đơn điệu của điểm không đổi kết quả; đặc trưng cấp instance trong mô hình tuyến tính là hằng số trong một instance nên vô dụng, chỉ tương tác (W4) mới khai thác được.

### 4.3. Lệch miền và rò rỉ dữ liệu
Lệch miền: bộ X dùng lưới [0, 1000], depot ở tâm, lệch góc hoặc ngẫu nhiên, khách ngẫu nhiên hoặc theo cụm, nhiều phân phối demand, Q suy ra từ kích thước tuyến trung bình; còn A (ngẫu nhiên), B (cụm), P (biến thể với Q khác) có lưới nhỏ hơn và độ chặt tải riêng [CKC]. Cách xử lý: đặc trưng không thứ nguyên (chia cho khoảng cách trung bình tới depot); so phân phối đặc trưng và độ chặt Σq/Q giữa hai miền trước khi chạy.

Rò rỉ cần tránh:
- Chia train/validation theo cạnh thay vì theo instance (cạnh cùng instance tương quan mạnh, làm AUC ảo).
- Tune siêu tham số, chọn đặc trưng hay tune λ trên A/B/P. W1, W2 tune trên cùng tập sinh với W3, W4; λ tốt nhất theo từng instance A/B/P chỉ là tham chiếu oracle, báo riêng.
- Dùng lời giải PyVRP hay BKS của chính A/B/P làm nhãn.
- Chỉnh bộ sinh sau khi đã thấy kết quả trên A/B/P.

Nhãn: PyVRP phải chạy với đội xe không giới hạn và ma trận khoảng cách nguyên đã làm tròn như đề tài, nếu không nhãn thuộc về bài toán khác. Lời giải gần tối ưu không duy nhất: chạy vài seed, lấy tần suất cạnh làm nhãn mềm. Lớp dương chỉ khoảng 1–2% số cặp: đo PR-AUC hoặc recall@k, nhưng quyết định bằng chi phí của CW.

### 4.4. Báo cáo gap khi số xe tự do
BKS của A/B/P là tối ưu cho bài toán gắn với k; khi số xe tự do, tối ưu có thể ngắn hơn với nhiều xe hơn, nên gap âm là hợp lệ [CKC: "đúng k" hay "tối đa k" theo từng tập]. Quy ước đề xuất:
- gap_k = (c − BKS_k)/BKS_k, cho phép âm, kèm số xe m, m − k và tỉ lệ instance có m > k.
- Gap âm khi m ≤ k là dấu hiệu lỗi (cách làm tròn, định nghĩa k). Trước hết chấm lại lời giải BKS đã công bố bằng validator để xác nhận khớp giá trị BKS.
- gap_free so với REF_free = min(BKS_k, lời giải tốt nhất của PyVRP chạy dài, đội xe tự do, nhiều seed); gọi là "tốt nhất đã biết khi số xe tự do" vì chỉ là cận trên. Dùng PyVRP trên A/B/P để đánh giá không phải rò rỉ, miễn không dùng để huấn luyện hay tune.
- Kết luận chính dựa trên so sánh theo cặp giữa các phương pháp, không dựa trên gap tuyệt đối.

## 5. Trình bày với giảng viên

"Ý tưởng dùng học máy để chấm điểm phép ghép của Clarke–Wright đã có (Legrand và cộng sự 2021; Hamada & Kashima 2026), nên em không tuyên bố ý tưởng này là mới. Đóng góp của em là một đánh giá có kiểm soát mà, theo những gì em tìm được, các công trình đó chưa làm: so một điểm học được nhẹ, giải thích được với savings có tham số được tune trực tiếp trên cùng họ hàm, cùng ngân sách thời gian, trên CVRPLIB A/B/P khi không ấn định số xe, để tách phần lợi do nhãn, do độ biểu diễn và do tune. Dù kết quả dương hay âm, bộ khung này dùng tiếp được cho CW ngẫu nhiên hóa và VRPTW ở các Project sau."
