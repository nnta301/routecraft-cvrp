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

Lệnh build/run chưa được cung cấp vì scaffold build và ứng dụng chưa tồn tại. Khi được thêm, chúng phải được ghi rõ là hướng dẫn đã kiểm chứng hoặc dự kiến. Tham khảo: [CMake](https://cmake.org/documentation/), [Python venv](https://docs.python.org/3/library/venv.html), [Node.js](https://nodejs.org/en/download), [Docker](https://docs.docker.com/).
