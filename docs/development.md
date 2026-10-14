# Phát triển và môi trường

## Yêu cầu dự kiến

| Thời điểm | Công cụ |
| --- | --- |
| Cần ngay cho scaffold solver | Git, compiler hỗ trợ C++17, CMake từ 3.24 |
| Backend sau này | Python 3.12.x và `venv` |
| Frontend sau này | Node.js 24 LTS và npm |
| Đóng gói sau này | Docker |

Các mục backend, frontend và Docker chưa được cài đặt/cấu hình trong repository. Phiên bản dependency FastAPI và React chưa được pin vì ứng dụng chưa tồn tại.

## Kiểm tra phiên bản

Các lệnh sau là kiểm tra khả năng tương thích dự kiến, không xác nhận môi trường đã được thử nghiệm trên mọi hệ điều hành/compiler:

```text
git --version
cmake --version
<cpp-compiler> --version
python --version          # khi bắt đầu backend
node --version && npm --version  # khi bắt đầu frontend
docker --version          # khi bắt đầu đóng gói
```

## Build và test scaffold

Từ root repository, dùng build directory riêng cho mỗi cấu hình:

```text
cmake -S . -B build/Debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/Debug
ctest --test-dir build/Debug --output-on-failure

cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/Release
ctest --test-dir build/Release --output-on-failure
```

`configure` tạo cấu hình build, `build` biên dịch, CTest chạy test đã đăng ký. `Debug` dành cho phát triển/gỡ lỗi; `Release` dự kiến phục vụ thực nghiệm sau này. Có thể kiểm tra CLI tại `build/Debug/bin/routecraft` với generator một cấu hình; generator nhiều cấu hình thêm thư mục cấu hình dưới `bin`.

Môi trường cục bộ đã kiểm tra ở bước scaffold này là Windows với CMake 3.31.5 và MinGW g++; chưa khẳng định hỗ trợ mọi OS/compiler. Backend/frontend/Docker chưa được cài đặt hay kiểm thử. Tham khảo: [CMake](https://cmake.org/documentation/), [Python venv](https://docs.python.org/3/library/venv.html), [Node.js](https://nodejs.org/en/download), [Docker](https://docs.docker.com/).
