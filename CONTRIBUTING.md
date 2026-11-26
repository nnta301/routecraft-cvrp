# Quy ước làm việc — RouteCraft CVRP

> Áp dụng cho toàn bộ repository và các sprint của RouteCraft CVRP.
> Cập nhật lần cuối: 19-10-2026.

## 1. Mục đích và phạm vi

Quy ước này giúp công việc có thể theo dõi, kiểm tra và mở rộng nhất quán. RouteCraft CVRP là dự án nghiên cứu và triển khai heuristic cho bài toán Capacitated Vehicle Routing Problem (CVRP) với một depot và dưới 100 khách hàng.

Mọi thay đổi cần phục vụ một issue hoặc một quyết định đã được ghi nhận. Không mở rộng phạm vi một cách ngầm định sang API, giao diện web, Docker hoặc heuristic mới khi sprint/issue hiện tại chưa bao gồm chúng.

## 2. Quy ước ngôn ngữ

Toàn bộ nội dung diễn giải của dự án dùng **tiếng Việt**, gồm:

- README, tài liệu kỹ thuật, tài liệu sprint và mô tả phát hành.
- Tiêu đề, mô tả, bình luận và cập nhật tiến độ của GitHub Issue/PR.
- Tên label, milestone và trạng thái Project board khi có thể cấu hình.
- Comment trong mã nguồn, mô tả kiểm thử, thông báo CLI do dự án tạo ra và nội dung review.

Chỉ có hai ngoại lệ bắt buộc dùng tiếng Anh:

- **Tên nhánh**.
- **Commit message**.

Tên lệnh, từ khóa Git/GitHub, tên tệp, tên thư viện và mã nguồn giữ nguyên khi đó là cú pháp hoặc tên kỹ thuật cần thiết, ví dụ `main`, `Closes #42`, `CMake` và `.vrp`.

## 3. Mỗi loại thông tin được quản lý ở đâu?

| Thành phần | Nơi quản lý chính |
|---|---|
| GitHub Issue | Mục tiêu, phạm vi, tiêu chí hoàn thành, phụ thuộc, quyết định và tiến độ |
| Project board | Trạng thái thực tế của công việc |
| Branch | Các thay đổi đang được triển khai cho một issue |
| Pull Request (PR) | Diff, kết quả kiểm tra, tự review và lịch sử merge |
| Tài liệu sprint | Mục tiêu sprint, danh sách issue, thay đổi phạm vi, kết quả và retrospective |

Không sao chép toàn bộ trạng thái issue vào tài liệu sprint. Tài liệu sprint chỉ liên kết tới issue liên quan và ghi các quyết định/tổng kết ở cấp sprint.

## 4. Issue và Sprint

Mỗi việc có giá trị triển khai cần có một GitHub Issue. Một issue tối thiểu phải nêu rõ:

- Mục tiêu và kết quả mong đợi.
- Phạm vi xử lý và phần ngoài phạm vi.
- Acceptance criteria có thể kiểm chứng.
- Phụ thuộc, nếu có.
- Cách kiểm tra hoặc bằng chứng cần có.

Trước khi bắt đầu, đọc toàn bộ issue, các quyết định liên quan và PR đang mở. Nếu yêu cầu chưa rõ, ghi câu hỏi và quyết định trong issue trước khi triển khai; không tự giả định các chi tiết ảnh hưởng đến thiết kế.

Mỗi sprint được ghi tại `docs/sprints/sprint-XX.md`; `docs/sprints/README.md` là chỉ mục/quy ước sprint; `README.md` phải liên kết đến sprint hiện tại. Khi sprint kết thúc, cập nhật kết quả, phần chưa hoàn thành, thay đổi phạm vi và retrospective ngắn.

## 5. Trạng thái Project board

| Trạng thái | Ý nghĩa |
|---|---|
| **Tồn đọng** | Việc được đề xuất nhưng chưa cam kết cho sprint hiện tại hoặc còn cần làm rõ |
| **Cần làm** | Đã vào sprint, có phạm vi rõ ràng nhưng chưa bắt đầu |
| **Đang làm** | Đang thực sự triển khai hoặc điều tra |
| **Bị chặn** | Đã bắt đầu nhưng không thể tiếp tục phần việc có ý nghĩa |
| **Đang review** | PR đã sẵn sàng, tự review hoàn tất và các kiểm tra cần thiết đã chạy |
| **Hoàn thành** | Đạt tiêu chí hoàn thành và đã được merge vào `main` |

Quy tắc chuyển trạng thái:

1. Thêm **issue thật** vào Project board, không tạo card nháp trùng lặp.
2. Chuyển `Cần làm` → `Đang làm` khi bắt đầu thực hiện.
3. Chuyển sang `Bị chặn` khi không thể tiếp tục, đồng thời ghi nguyên nhân và điều kiện để tiếp tục trong issue.
4. Chuyển sang `Đang review` khi PR đã sẵn sàng và có kết quả kiểm tra.
5. Chuyển về `Đang làm` nếu sau review cần sửa đáng kể.
6. Chỉ chuyển sang `Hoàn thành` sau khi merge.

Được assign hoặc đã tạo branch không đồng nghĩa với `Đang làm`; viết code xong hoặc đã push cũng chưa phải `Hoàn thành`.

## 6. Quy trình branch

Mặc định áp dụng **một issue – một branch – một PR**. Không push trực tiếp vào `main`.

Tên branch dùng tiếng Anh theo mẫu:

```text
<type>/<issue-number>-<short-description>
```

Ví dụ:

```text
feat/1-add-cvrp-domain-model
fix/12-handle-invalid-demand
docs/8-update-sprint-guide
```

| Type | Mục đích |
|---|---|
| `feat` | Tính năng mới |
| `fix` | Sửa lỗi |
| `docs` | Tài liệu |
| `test` | Kiểm thử |
| `refactor` | Cải thiện cấu trúc, không chủ ý thay đổi hành vi |
| `chore` | Cấu hình hoặc công việc hỗ trợ |

Trước khi tạo branch, kiểm tra working tree và cập nhật `main` theo cách an toàn:

```bash
git status --short
git switch main
git pull --ff-only origin main
git switch -c feat/42-short-description
```

Nếu có thay đổi chưa commit, xác định rõ chúng thuộc issue nào và bảo toàn chúng trước khi đổi branch. Không xóa thay đổi chỉ để làm working tree sạch.

## 7. Trong quá trình triển khai

- Giữ thay đổi trong phạm vi issue.
- Không sửa hoặc xóa code, comment, tài liệu không liên quan.
- Cập nhật tài liệu khi thay đổi cách chạy, đầu vào/đầu ra, cấu hình hoặc hành vi.
- Không commit credential, dữ liệu riêng tư, file build, cache hoặc kết quả sinh cục bộ.
- Kết quả kiểm tra phải phản ánh thực tế; nếu chưa chạy được, ghi rõ lý do.

Với thay đổi ảnh hưởng đến CVRP, cần ghi rõ tác động đến mô hình dữ liệu, parser `.vrp`, quy tắc khoảng cách, ràng buộc tải trọng, hàm mục tiêu, validator hoặc định dạng xuất nghiệm. Nếu thay đổi hành vi, thêm hoặc cập nhật fixture/test tương ứng.

Cuối mỗi buổi làm việc có tiến triển đáng kể, cập nhật issue ngắn gọn bằng tiếng Việt:

```text
Tiến độ: Đã kiểm tra đầu vào không hợp lệ.
Tiếp theo: Bổ sung kiểm thử cho các trường hợp lỗi.
Trở ngại: Không có.
Kiểm tra: Unit test đã đạt; kiểm thử tích hợp đang chờ.
```

## 8. Commit và push

Trước khi commit:

1. Xem danh sách file thay đổi.
2. Đọc diff và nội dung file mới.
3. Chạy kiểm tra phù hợp.
4. Stage đúng các file của issue.
5. Xem lại phần đã stage.

```bash
git status --short
git diff
git diff --check
git diff --cached
```

Commit message dùng tiếng Anh và mô tả kết quả cụ thể:

```text
feat(solver): add CVRP domain model
fix(parser): reject malformed demand section
test(cli): cover JSON solution export
docs(sprint): record Sprint 1 outcome
```

Một issue có thể có nhiều commit, nhưng mỗi commit phải là một nhóm thay đổi có ý nghĩa, dễ đọc và không trộn công việc không liên quan. Push branch để lưu tiến độ; chỉ merge vào `main` thông qua PR.

## 9. Pull Request và tự review

Tiêu đề và mô tả PR dùng tiếng Việt. PR tập trung vào một issue chính và phải có:

- Vấn đề được giải quyết.
- Thay đổi chính và hành vi sau thay đổi.
- Cách kiểm tra, kết quả và phần chưa kiểm tra.
- Hạn chế còn lại, nếu có.
- Liên kết issue.

Dùng `Closes #<số-issue>` khi PR hoàn thành toàn bộ issue; dùng `Refs #<số-issue>` khi chỉ liên quan hoặc mới hoàn thành một phần.

Vì hiện tại dự án do một người phụ trách, không bắt buộc có reviewer thứ hai. Trước khi merge, tác giả phải tự review PR như một reviewer độc lập:

- Diff có giải quyết đúng acceptance criteria không?
- Có làm sai ràng buộc CVRP, parser, validator, CLI hay định dạng xuất không?
- Test/CI cần thiết đã đạt chưa?
- README, tài liệu sprint và các tài liệu liên quan đã còn đúng chưa?
- Có file hoặc thông tin không nên đưa vào repository không?

Nếu GitHub/Copilot được cấu hình review tự động, xử lý các phản hồi bắt buộc trước khi merge. Với thay đổi lớn, nên mở Draft PR sớm và ghi rõ phần cần xem xét.

## 10. Kiểm tra, merge và Definition of Done

Trước khi merge, cần có:

- PR liên kết đúng issue.
- Tự review hoàn tất.
- Các acceptance criteria đạt.
- Kiểm tra phù hợp đã chạy và có kết quả thực tế.
- Hai kiểm tra CI build/test cho cấu hình **Debug** và **Release** đều đạt.
- Không còn conflict và không có thay đổi ngoài phạm vi.

Một issue chỉ là **Hoàn thành** khi:

- Thay đổi đã được merge vào `main`.
- Acceptance criteria, kiểm thử và bằng chứng phù hợp đã hoàn tất.
- Tài liệu, README hoặc sprint document liên quan đã được cập nhật.
- Hạn chế hoặc phần chưa làm được ghi rõ trong issue/PR.
- Issue và Project board phản ánh đúng trạng thái cuối cùng.

Nếu PR chỉ hoàn thành một phần, giữ issue mở và ghi rõ phần còn lại.

## 11. Khi phát sinh thay đổi, lỗi hoặc blocker

| Tình huống | Cách xử lý |
|---|---|
| Lỗi nhỏ cần sửa để đạt acceptance criteria hiện có | Sửa trong cùng issue/PR |
| Yêu cầu còn mơ hồ | Ghi câu hỏi và quyết định trong issue trước khi code |
| Bug độc lập | Tìm issue trùng; nếu chưa có thì tạo bug riêng và liên kết nơi phát hiện |
| Tính năng/cải tiến ngoài phạm vi | Tạo issue ở `Tồn đọng` để ưu tiên trong sprint sau |
| Thiếu việc tiên quyết | Tạo hoặc liên kết dependency rồi xử lý theo thứ tự |
| Thay đổi contract CVRP dùng chung | Ghi rõ tác động, cập nhật test/tài liệu và quyết định trước khi triển khai |
| Bug phát hiện sau khi merge | Tạo bug mới và liên kết issue/PR gốc |

Khi bị chặn, cập nhật issue với: bước đang bị chặn, nguyên nhân/bằng chứng, thứ đang chờ, điều kiện để tiếp tục và phần độc lập còn có thể làm. Chỉ chuyển sang `Bị chặn` khi thực sự không còn phần việc có ý nghĩa để tiếp tục.

## 12. Cập nhật từ `main` và giải quyết conflict

Trước khi đưa thay đổi từ `main` vào branch đang làm, bảo toàn công việc hiện tại rồi chạy:

```bash
git fetch origin
git merge origin/main
```

Khi có conflict:

1. Đọc cả hai phía để hiểu hành vi cần giữ.
2. Không chọn toàn bộ một phía chỉ để hết conflict.
3. Nếu conflict cho thấy yêu cầu/thiết kế chưa rõ, ghi quyết định vào issue trước khi hoàn tất.
4. Chạy lại kiểm tra cho phần vừa hợp nhất và cập nhật PR.

## 13. Quy tắc ngắn cần nhớ

1. Mọi việc bắt đầu từ issue và được theo dõi trên board.
2. Một issue – một branch – một PR; không push trực tiếp vào `main`.
3. Mọi nội dung diễn giải bằng tiếng Việt; riêng branch và commit bằng tiếng Anh.
4. Không coi một việc là xong nếu chưa qua PR, kiểm tra và merge.
5. Khi phạm vi thay đổi, ghi lại quyết định trước khi mở rộng code.
