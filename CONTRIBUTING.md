# Đóng góp

Quy trình dự kiến: tạo branch theo phạm vi → thay đổi nhỏ, có thể review → commit → mở PR → review → merge. Không giả định repository đã có branch protection, reviewer bắt buộc hay project board.

- Branch: `docs/<chu-de>`, `feature/<chu-de>`, `fix/<chu-de>`.
- Commit: dùng mệnh lệnh ngắn, ví dụ `docs: describe CVRP data contract` hoặc `feat(solver): add parser validation`.
- PR nêu thay đổi chính, lý do, phạm vi không thực hiện, cách kiểm tra và hạn chế còn lại.
- Giữ nguyên comment, note và thay đổi hiện có của người khác; tách thay đổi không liên quan sang PR khác.
- PR nhỏ cần có acceptance criteria có thể kiểm tra; với tài liệu, có thể ghi kiểm tra tự đọc/liên kết thay vì test tự động.
