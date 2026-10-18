# Hướng dẫn review cho RouteCraft CVRP

## Nguồn thông tin và phạm vi

- Dùng issue được liên kết trong PR để xác định mục tiêu, phạm vi, acceptance criteria và dependencies.
- Dùng `docs/requirements.md` và `docs/data-contract.md` làm nguồn tham chiếu cho ràng buộc mô hình và contract đã tài liệu hóa.
- Nếu code, test và tài liệu mâu thuẫn, hãy chỉ rõ mâu thuẫn; không tự chọn một cách hiểu mà không nêu lý do.
- Không coi tính năng được đề xuất hoặc lên kế hoạch là đã được triển khai.

## Kiểm tra theo issue

- Kiểm tra thay đổi trong PR có đáp ứng đầy đủ acceptance criteria của issue liên kết hay không.
- Báo thiếu sót khi một acceptance criterion bắt buộc chưa được đáp ứng hoặc chưa có kiểm thử/chứng minh phù hợp.
- Báo scope creep khi PR thêm behavior, dependency, refactor hoặc file không cần thiết cho issue.
- Chỉ báo code dư thừa, trùng lặp hoặc dead code khi nó tạo rủi ro correctness hay maintenance rõ ràng.
- Không tạo comment chỉ về style hoặc refactor tùy chọn ngoài phạm vi issue.

## Tính nhất quán của tài liệu và contract

- Kiểm tra PR có làm README, tài liệu kỹ thuật, hướng dẫn build/test, trạng thái triển khai, output example hoặc giới hạn hiện tại trở nên lỗi thời không.
- Với CLI, API, TXT và JSON, kiểm tra field name, type, unit, quy ước ID, feasibility result và error behavior có khớp contract được tài liệu hóa không.
- Không yêu cầu cập nhật tài liệu cho refactor nội bộ không làm thay đổi behavior đã tài liệu hóa.

## Tính đúng CVRP

- Mỗi customer phải được phục vụ đúng một lần.
- Mỗi route phải bắt đầu và kết thúc tại depot nội bộ `0`.
- Tổng demand của mỗi route không được vượt capacity `Q`.
- EUC_2D phải được làm tròn theo từng cạnh trước khi cộng.
- Clarke-Wright chỉ được merge các endpoint hợp lệ của hai route khác nhau và phải giữ nghiệm khả thi.
- Local search không được làm mất/trùng customer hoặc báo nghiệm chi phí cao hơn là một improvement.

## Chất lượng review

- Chỉ comment vấn đề actionable về correctness, contract, documentation, scope, security hoặc maintenance.
- Mỗi comment nên nêu vị trí, vấn đề và hậu quả cụ thể.