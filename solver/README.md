# Solver

Thành phần này chứa lõi C++ dùng chung và CLI mỏng gọi lõi đó. `routecraft_core` hiện cung cấp mô hình dữ liệu CVRP tối thiểu gồm `Node`, `Instance`, `Route`, `Solution` và `ValidationResult`; depot dùng chỉ số nội bộ `0`, còn `original_id` được giữ để khôi phục ID nguồn.

Lõi cũng cung cấp tiện ích tính khoảng cách `EUC_2D`, tải tuyến, độ dài tuyến và tổng độ dài nghiệm. CLI `routecraft` hiện hỗ trợ `--help`, `-h`, `--version` cùng smoke test CTest. Việc tách core khỏi CLI giúp parser, heuristic, validator và API sau này dùng chung logic.

Trạng thái: mô hình dữ liệu CVRP, tiện ích khoảng cách và unit test đã có. Parser `.vrp`, Nearest Neighbor, validator hoàn chỉnh, đọc instance và xuất nghiệm TXT/JSON vẫn chưa triển khai.