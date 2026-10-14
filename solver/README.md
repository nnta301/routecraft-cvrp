# Solver

Thành phần này chứa lõi C++ dùng chung và CLI mỏng gọi lõi đó. Scaffold hiện có thư viện tĩnh `routecraft_core`, chỉ cung cấp `routecraft::version()`, cùng CLI `routecraft` hỗ trợ help/version và smoke test CTest. Tách core khỏi CLI để API sau này có thể gọi executable mà không sao chép logic.

Trạng thái: build scaffold, CLI và tests đã có. Mô hình CVRP, parser, khoảng cách `EUC_2D`, heuristic, validator, đọc `.vrp` và xuất nghiệm TXT/JSON **chưa triển khai**.
