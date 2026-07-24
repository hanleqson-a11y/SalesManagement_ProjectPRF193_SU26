# Chương trình Quản lý Cửa hàng (C++)

## Danh sách file
```
main.cpp       - Chương trình chính: đăng nhập + toàn bộ menu, gọi tới các file .h bên dưới
Utility.h      - Các hàm dùng chung: đọc nhập (hỗ trợ "exit"), xử lý chuỗi, ngày tháng, tiền
Account.h      - Lớp Account + AccountManager (tài khoản, đăng nhập, đổi tài khoản/mật khẩu)
Product.h      - Lớp Product + ProductManager (sản phẩm)
Customer.h     - Lớp Customer + CustomerManager (khách hàng)
Invoice.h      - Lớp InvoiceItem, Invoice + InvoiceManager (hóa đơn)

account.txt    - Danh sách tài khoản
product.txt    - Danh sách sản phẩm
customer.txt   - Danh sách khách hàng (dòng "0 0 0" là khách vãng lai)
invoice.txt    - Danh sách hóa đơn
```

- Mỗi lớp (`class`) chỉ có các biến `public` và các hàm thành viên viết luôn phần thân
  ngay trong file `.h` (không tách file `.cpp` riêng, không dùng con trỏ phức tạp).
- Dùng **chỉ số (index)** trong `vector` thay vì con trỏ để tìm/sửa/xóa dữ liệu
  (ví dụ `timTheoId()` trả về số nguyên, -1 nếu không tìm thấy).
- Tên hàm/biến bằng tiếng Việt không dấu, đặt tên rõ nghĩa theo đúng yêu cầu đề bài.

## Biên dịch (chỉ cần 1 file main.cpp vì các .h đã có sẵn code)
```bash
g++ -std=c++17 -o quanlycuahang main.cpp
```

## Chạy chương trình
```bash
./quanlycuahang
```
(Trên Windows dùng Code::Blocks / Dev-C++ / Visual Studio: tạo project console, thêm đủ
5 file `.h` và `main.cpp` vào project rồi build & run bình thường.)

## Tài khoản mẫu
| Username | Password | Role  |
|----------|----------|-------|
| admin    | 123456   | Admin (chủ cửa hàng - 8 mục, có thêm mục Tạo/Xóa tài khoản NV) |
| staff1   | 111111   | Staff (nhân viên - 7 mục) |
| staff2   | 222222   | Staff |

## Quy ước "exit"
Ở bất kỳ chỗ nhập dữ liệu nào, gõ đúng chữ thường `exit` sẽ hủy thao tác đang làm và
quay về menu/màn hình trước đó.

## Định dạng các file dữ liệu
- `account.txt`  : `Username Password Role`
- `product.txt`  : `ID Tên(có thể nhiều từ) Giá SốLượng Loại`
- `customer.txt` : `ID Tên(có thể nhiều từ) SốĐiệnThoại`
- `invoice.txt`  : mỗi hóa đơn 1 khối, dòng đầu bắt đầu bằng `#`, các dòng sau là
  từng sản phẩm, kết thúc bằng dòng `#END`:
  ```
  #INV0001|1|23-07-2026|25000000
  P001|1|25000000
  #END
  ```
