#include <iostream>
#include <string>
#include <vector>

#include "Utility.h"
#include "Account.h"
#include "Product.h"
#include "Customer.h"
#include "Invoice.h"
using namespace std;

// ============================================================
//  MAIN.CPP
//  Chuong trinh Quan ly Cua hang
//  Quy uoc: tai bat ky buoc nhap du lieu nao, go dung chu "exit" se
//  HUY thao tac hien tai va QUAY VE menu/man hinh truoc do.
// ============================================================

// ---------- 1. QUAN LY SAN PHAM ----------

void themSanPham(ProductManager &pm) {
    bool exit1;
    string id = inputOrExit("Nhap ID san pham: ", exit1);
    if (exit1) return;
    if (pm.tonTaiId(id)) { cout << "ID san pham da ton tai!\n"; return; }

    string ten = inputOrExit("Nhap ten san pham: ", exit1);
    if (exit1) return;
    double gia = inputDoubleOrExit("Nhap gia: ", exit1);
    if (exit1) return;
    int soLuong = inputIntOrExit("Nhap so luong: ", exit1);
    if (exit1) return;
    string loai = inputOrExit("Nhap loai san pham: ", exit1);
    if (exit1) return;

    pm.themSanPham(Product(id, ten, gia, soLuong, loai));
    cout << "Them san pham thanh cong!\n";
}

void suaSanPham(ProductManager &pm) {
    bool exit1;
    string id = inputOrExit("Nhap ID san pham can sua: ", exit1);
    if (exit1) return;
    int i = pm.timTheoId(id);
    if (i == -1) { cout << "Khong tim thay san pham.\n"; return; }

    cout << "Thong tin hien tai: ";
    pm.danhSach[i].inRa();

    string ten = inputOrExit("Nhap ten moi: ", exit1);
    if (exit1) return;
    double gia = inputDoubleOrExit("Nhap gia moi: ", exit1);
    if (exit1) return;
    int soLuong = inputIntOrExit("Nhap so luong moi: ", exit1);
    if (exit1) return;
    string loai = inputOrExit("Nhap loai moi: ", exit1);
    if (exit1) return;

    pm.suaSanPham(id, ten, gia, soLuong, loai);
    cout << "Cap nhat san pham thanh cong!\n";
}

void xoaSanPham(ProductManager &pm) {
    bool exit1;
    string id = inputOrExit("Nhap ID san pham can xoa: ", exit1);
    if (exit1) return;
    if (pm.xoaSanPham(id)) cout << "Xoa san pham thanh cong!\n";
    else cout << "Khong tim thay san pham.\n";
}

void timKiemSanPham(ProductManager &pm) {
    while (true) {
        cout << "\n--- TIM KIEM SAN PHAM ---\n";
        cout << "1.4.1. Tim theo ten\n";
        cout << "1.4.2. Tim theo ID\n";
        cout << "1.4.3. Tim theo loai\n";
        bool exit1;
        string chon = inputOrExit("Chon (hoac go 'exit' de quay lai): ", exit1);
        if (exit1) return;

        if (chon == "1.4.1" || chon == "1") {
            string ten = inputOrExit("Nhap ten can tim: ", exit1);
            if (exit1) continue;
            vector<int> ketQua = pm.timTheoTen(ten);
            pm.inDanhSach(ketQua);
        } else if (chon == "1.4.2" || chon == "2") {
            string id = inputOrExit("Nhap ID can tim: ", exit1);
            if (exit1) continue;
            int i = pm.timTheoId(id);
            if (i != -1) pm.danhSach[i].inRa();
            else cout << "Khong tim thay san pham.\n";
        } else if (chon == "1.4.3" || chon == "3") {
            string loai = inputOrExit("Nhap loai can tim: ", exit1);
            if (exit1) continue;
            vector<int> ketQua = pm.timTheoLoai(loai);
            pm.inDanhSach(ketQua);
        } else {
            cout << "Lua chon khong hop le.\n";
        }
    }
}

void quanLySanPham(ProductManager &pm) {
    while (true) {
        cout << "\n===== 1. QUAN LY SAN PHAM =====\n";
        cout << "1.1. Them san pham\n";
        cout << "1.2. Sua san pham\n";
        cout << "1.3. Xoa san pham\n";
        cout << "1.4. Tim kiem san pham\n";
        cout << "1.5. Hien thi tat ca san pham\n";
        bool exit1;
        string chon = inputOrExit("Chon chuc nang (hoac go 'exit' de quay lai menu chinh): ", exit1);
        if (exit1) return;

        if (chon == "1.1" || chon == "1") themSanPham(pm);
        else if (chon == "1.2" || chon == "2") suaSanPham(pm);
        else if (chon == "1.3" || chon == "3") xoaSanPham(pm);
        else if (chon == "1.4" || chon == "4") timKiemSanPham(pm);
        else if (chon == "1.5" || chon == "5") pm.inTatCa();
        else cout << "Lua chon khong hop le.\n";
    }
}

// ---------- 2. QUAN LY KHACH HANG ----------

void themKhachHang(CustomerManager &cm) {
    bool exit1;
    string sdt;
    while (true) {
        sdt = inputOrExit("Nhap so dien thoai: ", exit1);
        if (exit1) return;
        if (cm.tonTaiSdt(sdt)) { cout << "So dien thoai da ton tai, vui long nhap lai.\n"; continue; }
        break;
    }
    string ten = inputOrExit("Nhap ten khach hang: ", exit1);
    if (exit1) return;

    int i = cm.themKhachHang(sdt, ten);
    if (i != -1) cout << "Them khach hang thanh cong! ID: " << cm.danhSach[i].id << "\n";
    else cout << "Co loi xay ra khi them khach hang.\n";
}

void suaKhachHang(CustomerManager &cm) {
    bool exit1;
    string sdt = inputOrExit("Nhap so dien thoai can sua: ", exit1);
    if (exit1) return;
    int i = cm.timTheoSdt(sdt);
    if (i == -1) { cout << "Khong tim thay khach hang.\n"; return; }

    cout << "Thong tin hien tai: ";
    cm.danhSach[i].inRa();

    string ten = inputOrExit("Nhap ten moi: ", exit1);
    if (exit1) return;
    string sdtMoi = inputOrExit("Nhap so dien thoai moi (nhap lai sdt cu neu khong doi): ", exit1);
    if (exit1) return;

    if (cm.suaKhachHang(sdt, ten, sdtMoi)) cout << "Cap nhat khach hang thanh cong!\n";
    else cout << "So dien thoai moi da ton tai hoac co loi xay ra.\n";
}

void xoaKhachHang(CustomerManager &cm) {
    bool exit1;
    string sdt = inputOrExit("Nhap so dien thoai can xoa: ", exit1);
    if (exit1) return;
    if (cm.xoaKhachHang(sdt)) cout << "Xoa khach hang thanh cong!\n";
    else cout << "Khong tim thay khach hang.\n";
}

void timKiemKhachHang(CustomerManager &cm) {
    while (true) {
        cout << "\n--- TIM KIEM KHACH HANG ---\n";
        cout << "2.4.1. Tim theo ten\n";
        cout << "2.4.2. Tim theo ID\n";
        cout << "2.4.3. Tim theo so dien thoai\n";
        bool exit1;
        string chon = inputOrExit("Chon (hoac go 'exit' de quay lai): ", exit1);
        if (exit1) return;

        if (chon == "2.4.1" || chon == "1") {
            string ten = inputOrExit("Nhap ten can tim: ", exit1);
            if (exit1) continue;
            vector<int> ketQua = cm.timTheoTen(ten);
            cm.inDanhSach(ketQua);
        } else if (chon == "2.4.2" || chon == "2") {
            int id = inputIntOrExit("Nhap ID can tim: ", exit1);
            if (exit1) continue;
            int i = cm.timTheoId(id);
            if (i != -1) cm.danhSach[i].inRa();
            else cout << "Khong tim thay khach hang.\n";
        } else if (chon == "2.4.3" || chon == "3") {
            string sdt = inputOrExit("Nhap so dien thoai can tim: ", exit1);
            if (exit1) continue;
            int i = cm.timTheoSdt(sdt);
            if (i != -1) cm.danhSach[i].inRa();
            else cout << "Khong tim thay khach hang.\n";
        } else {
            cout << "Lua chon khong hop le.\n";
        }
    }
}

void quanLyKhachHang(CustomerManager &cm) {
    while (true) {
        cout << "\n===== 2. QUAN LY KHACH HANG =====\n";
        cout << "2.1. Them thong tin khach hang\n";
        cout << "2.2. Sua thong tin khach hang\n";
        cout << "2.3. Xoa thong tin khach hang\n";
        cout << "2.4. Tim kiem thong tin khach hang\n";
        cout << "2.5. Hien thi tat ca thong tin khach hang\n";
        bool exit1;
        string chon = inputOrExit("Chon chuc nang (hoac go 'exit' de quay lai menu chinh): ", exit1);
        if (exit1) return;

        if (chon == "2.1" || chon == "1") themKhachHang(cm);
        else if (chon == "2.2" || chon == "2") suaKhachHang(cm);
        else if (chon == "2.3" || chon == "3") xoaKhachHang(cm);
        else if (chon == "2.4" || chon == "4") timKiemKhachHang(cm);
        else if (chon == "2.5" || chon == "5") cm.inTatCa();
        else cout << "Lua chon khong hop le.\n";
    }
}

// ---------- 3. QUAN LY HOA DON ----------

void timHoaDon(InvoiceManager &im, CustomerManager &cm) {
    while (true) {
        cout << "\n--- TIM HOA DON ---\n";
        cout << "3.1.1. Tim theo ngay\n";
        cout << "3.1.2. Tim theo thong tin khach hang (so dien thoai)\n";
        bool exit1;
        string chon = inputOrExit("Chon (hoac go 'exit' de quay lai): ", exit1);
        if (exit1) return;

        if (chon == "3.1.1" || chon == "1") {
            string date;
            while (true) {
                date = inputOrExit("Nhap ngay (DD-MM-YYYY): ", exit1);
                if (exit1) break;
                if (!isValidDate(date)) { cout << "Ngay khong hop le, vui long nhap lai.\n"; continue; }
                break;
            }
            if (exit1) continue;
            vector<int> ketQua = im.timTheoNgay(date);
            im.inDanhSach(ketQua);
        } else if (chon == "3.1.2" || chon == "2") {
            string sdt = inputOrExit("Nhap so dien thoai khach hang: ", exit1);
            if (exit1) continue;
            int i = cm.timTheoSdt(sdt);
            if (i == -1) { cout << "Khong tim thay khach hang co so dien thoai nay.\n"; continue; }
            vector<int> ketQua = im.timTheoKhachHang(cm.danhSach[i].id);
            im.inDanhSach(ketQua);
        } else {
            cout << "Lua chon khong hop le.\n";
        }
    }
}

void xoaHoaDon(InvoiceManager &im) {
    bool exit1;
    string id = inputOrExit("Nhap ma hoa don can xoa: ", exit1);
    if (exit1) return;
    if (im.xoaHoaDon(id)) cout << "Xoa hoa don thanh cong!\n";
    else cout << "Khong tim thay hoa don.\n";
}

void quanLyHoaDon(InvoiceManager &im, CustomerManager &cm) {
    while (true) {
        cout << "\n===== 3. QUAN LY HOA DON =====\n";
        cout << "3.1. Tim hoa don\n";
        cout << "3.2. Xoa hoa don\n";
        bool exit1;
        string chon = inputOrExit("Chon chuc nang (hoac go 'exit' de quay lai menu chinh): ", exit1);
        if (exit1) return;

        if (chon == "3.1" || chon == "1") timHoaDon(im, cm);
        else if (chon == "3.2" || chon == "2") xoaHoaDon(im);
        else cout << "Lua chon khong hop le.\n";
    }
}

// ---------- 4. BAN HANG ----------

void banHang(ProductManager &pm, CustomerManager &cm, InvoiceManager &im) {
    cout << "\n===== 4. BAN HANG =====\n";
    bool exit1;

    string coLuu = inputOrExit("Khach hang co muon luu thong tin khong (Y/N): ", exit1);
    if (exit1) return;

    int customerId = 0; // 0 = khach vang lai

    if (toLowerStr(coLuu) == "y") {
        string sdt = inputOrExit("Nhap so dien thoai: ", exit1);
        if (exit1) return;

        int i = cm.timTheoSdt(sdt);
        if (i != -1) {
            customerId = cm.danhSach[i].id;
        } else {
            string ten = inputOrExit("Khach hang moi. Nhap ten khach hang: ", exit1);
            if (exit1) return;
            int j = cm.themKhachHang(sdt, ten);
            if (j != -1) customerId = cm.danhSach[j].id;
        }
    }

    vector<InvoiceItem> gioHang;
    bool huy = false;

    while (true) {
        string idSp = inputOrExit("Nhap ID san pham: ", exit1);
        if (exit1) { huy = true; break; }

        int i = pm.timTheoId(idSp);
        if (i == -1) { cout << "San pham khong ton tai.\n"; continue; }

        // Tinh xem trong gio hang da co bao nhieu san pham nay roi
        int daCoTrongGio = 0;
        for (int k = 0; k < (int)gioHang.size(); k++) {
            if (gioHang[k].productId == idSp) daCoTrongGio += gioHang[k].quantity;
        }
        int conLai = pm.danhSach[i].quantity - daCoTrongGio;

        int soLuong;
        bool exitSL = false;
        while (true) {
            soLuong = inputIntOrExit("Nhap so luong: ", exitSL);
            if (exitSL) break;
            if (soLuong <= 0) { cout << "So luong phai lon hon 0, vui long nhap lai.\n"; continue; }
            if (conLai < soLuong) {
                cout << "Chi con " << conLai << " san pham vui long nhap lai\n";
                continue;
            }
            break;
        }
        if (exitSL) { huy = true; break; }

        gioHang.push_back(InvoiceItem(idSp, soLuong, pm.danhSach[i].price));

        string muaThem = inputOrExit("Mua them (Y/N): ", exit1);
        if (exit1) { huy = true; break; }
        if (toLowerStr(muaThem) != "y") break;
    }

    if (huy) { cout << "Da huy giao dich ban hang.\n"; return; }
    if (gioHang.empty()) { cout << "Khong co san pham nao duoc mua.\n"; return; }

    // Sau khi khach dong y mua, moi thuc su tru kho va luu hoa don
    for (int k = 0; k < (int)gioHang.size(); k++) {
        pm.giamSoLuong(gioHang[k].productId, gioHang[k].quantity);
    }

    Invoice hd(im.idTiepTheo(), customerId, getToday());
    for (int k = 0; k < (int)gioHang.size(); k++) hd.themSanPham(gioHang[k]);
    im.themHoaDon(hd);

    cout << "So tien can thanh toan la: " << formatMoney(hd.totalAmount) << "\n";
}

// ---------- 5. THONG KE ----------

void doanhThuTheoNgay(InvoiceManager &im) {
    bool exit1;
    string date;
    while (true) {
        date = inputOrExit("Nhap ngay (DD-MM-YYYY): ", exit1);
        if (exit1) return;
        if (!isValidDate(date)) { cout << "Ngay khong hop le, vui long nhap lai.\n"; continue; }
        break;
    }
    double doanhThu = im.doanhThuNgay(date);
    cout << "Doanh thu ngay " << date << " la: " << formatMoney(doanhThu) << "\n";
}

void kiemTraSoLuongThap(ProductManager &pm) {
    bool exit1;
    int n = inputIntOrExit("Nhap n (kiem tra san pham co so luong nho hon n): ", exit1);
    if (exit1) return;
    vector<int> ketQua = pm.timSoLuongThapHon(n);
    pm.inDanhSach(ketQua);
}

void thongKe(InvoiceManager &im, ProductManager &pm) {
    while (true) {
        cout << "\n===== 5. THONG KE =====\n";
        cout << "5.1. Doanh thu ngay (DD-MM-YYYY)\n";
        cout << "5.2. Kiem tra cac san pham co so luong thap hon n\n";
        bool exit1;
        string chon = inputOrExit("Chon chuc nang (hoac go 'exit' de quay lai menu chinh): ", exit1);
        if (exit1) return;

        if (chon == "5.1" || chon == "1") doanhThuTheoNgay(im);
        else if (chon == "5.2" || chon == "2") kiemTraSoLuongThap(pm);
        else cout << "Lua chon khong hop le.\n";
    }
}

// ---------- 6. DOI TEN TAI KHOAN / MAT KHAU ----------

void doiUsername(AccountManager &am, int &chiSoTaiKhoanDangNhap) {
    bool exit1;
    string usernameCu = inputOrExit("Nhap ten tai khoan hien tai: ", exit1);
    if (exit1) return;
    string password = inputOrExit("Nhap mat khau hien tai: ", exit1);
    if (exit1) return;
    string usernameMoi = inputOrExit("Nhap ten tai khoan moi: ", exit1);
    if (exit1) return;

    if (am.doiUsername(usernameCu, password, usernameMoi)) {
        cout << "Doi ten tai khoan thanh cong!\n";
        chiSoTaiKhoanDangNhap = am.timTheoUsername(usernameMoi);
    } else {
        cout << "Ten tai khoan/mat khau khong dung, hoac ten moi da ton tai.\n";
    }
}

void doiPassword(AccountManager &am, int &chiSoTaiKhoanDangNhap) {
    bool exit1;
    string username = inputOrExit("Nhap ten tai khoan hien tai: ", exit1);
    if (exit1) return;
    string passwordCu = inputOrExit("Nhap mat khau hien tai: ", exit1);
    if (exit1) return;
    string passwordMoi = inputOrExit("Nhap mat khau moi: ", exit1);
    if (exit1) return;

    if (am.doiPassword(username, passwordCu, passwordMoi)) {
        cout << "Doi mat khau thanh cong!\n";
        chiSoTaiKhoanDangNhap = am.timTheoUsername(username);
    } else {
        cout << "Ten tai khoan/mat khau khong dung.\n";
    }
}

void doiTaiKhoanMatKhau(AccountManager &am, int &chiSoTaiKhoanDangNhap) {
    while (true) {
        cout << "\n===== 6. DOI TEN TAI KHOAN / MAT KHAU =====\n";
        cout << "6.1. Doi ten tai khoan\n";
        cout << "6.2. Doi mat khau\n";
        bool exit1;
        string chon = inputOrExit("Chon chuc nang (hoac go 'exit' de quay lai menu chinh): ", exit1);
        if (exit1) return;

        if (chon == "6.1" || chon == "1") doiUsername(am, chiSoTaiKhoanDangNhap);
        else if (chon == "6.2" || chon == "2") doiPassword(am, chiSoTaiKhoanDangNhap);
        else cout << "Lua chon khong hop le.\n";
    }
}

// ---------- 7. (CHU CUA HANG) TAO/XOA TAI KHOAN NHAN VIEN ----------

void xoaTaiKhoanNhanVien(AccountManager &am) {
    bool exit1;
    string username = inputOrExit("Nhap ten tai khoan can xoa: ", exit1);
    if (exit1) return;
    if (am.xoaTaiKhoan(username)) cout << "Xoa tai khoan thanh cong!\n";
    else cout << "Khong tim thay tai khoan.\n";
}

void themTaiKhoanNhanVien(AccountManager &am) {
    bool exit1;
    string username = inputOrExit("Nhap ten tai khoan moi: ", exit1);
    if (exit1) return;
    if (am.tonTaiUsername(username)) { cout << "Ten tai khoan da ton tai!\n"; return; }
    string password = inputOrExit("Nhap mat khau: ", exit1);
    if (exit1) return;

    am.themTaiKhoan(username, password, "Staff");
    cout << "Them tai khoan nhan vien thanh cong!\n";
}

void quanLyTaiKhoanNhanVien(AccountManager &am) {
    while (true) {
        cout << "\n===== 7. TAO/XOA TAI KHOAN NHAN VIEN =====\n";
        cout << "7.1. Xoa tai khoan\n";
        cout << "7.2. Them tai khoan\n";
        bool exit1;
        string chon = inputOrExit("Chon chuc nang (hoac go 'exit' de quay lai menu chinh): ", exit1);
        if (exit1) return;

        if (chon == "7.1" || chon == "1") xoaTaiKhoanNhanVien(am);
        else if (chon == "7.2" || chon == "2") themTaiKhoanNhanVien(am);
        else cout << "Lua chon khong hop le.\n";
    }
}

// ---------- DANG NHAP ----------

// Tra ve chi so tai khoan trong am.danhSach neu dang nhap thanh cong
// Tra ve -1 neu nguoi dung go "exit" (nghia la muon thoat chuong trinh)
int dangNhap(AccountManager &am) {
    while (true) {
        bool exit1;
        string username = inputOrExit("Nhap ten tai khoan: ", exit1);
        if (exit1) return -1;
        string password = inputLine("Nhap mat khau: ");

        int i = am.dangNhap(username, password);
        if (i != -1) {
            cout << "Dang nhap thanh cong! Xin chao " << am.danhSach[i].username
                 << " (" << am.danhSach[i].role << ")\n";
            return i;
        } else {
            cout << "Sai ten tai khoan hoac mat khau, vui long nhap lai dang nhap.\n\n";
        }
    }
}

void inMenuChinh(bool laAdmin) {
    cout << "\n===== MENU CHINH =====\n";
    cout << "1. Quan ly san pham\n";
    cout << "2. Quan ly khach hang\n";
    cout << "3. Quan ly hoa don\n";
    cout << "4. Ban hang\n";
    cout << "5. Thong ke\n";
    cout << "6. Doi ten tai khoan/mat khau\n";
    int soDangXuat = 7;
    if (laAdmin) {
        cout << "7. Tao/Xoa tai khoan nhan vien\n";
        soDangXuat = 8;
    }
    cout << soDangXuat << ". Dang xuat\n";
}

// ============================================================
//  HAM MAIN
// ============================================================
int main() {
    AccountManager am("account.txt");
    ProductManager pm("product.txt");
    CustomerManager cm("customer.txt");
    InvoiceManager im("invoice.txt");

    cout << "========================================\n";
    cout << "  HE THONG QUAN LY CUA HANG - PRF193\n";
    cout << "========================================\n";
    cout << "* Trong chuong trinh ban co the nhap chinh xac tu \"exit\" de tro ve trang truoc do.\n";

    while (true) {
        int taiKhoan = dangNhap(am);
        if (taiKhoan == -1) { cout << "\nTam biet!\n"; break; }

        bool laAdmin = am.danhSach[taiKhoan].isAdmin();
        bool dangXuat = false;

        while (!dangXuat) {
            inMenuChinh(laAdmin);
            bool exit1;
            string chon = inputOrExit("Chon chuc nang: ", exit1);
            if (exit1) continue; // dang o menu chinh, khong co menu truoc do de quay lai

            int soDangXuat = laAdmin ? 8 : 7;

            if (chon == "1") quanLySanPham(pm);
            else if (chon == "2") quanLyKhachHang(cm);
            else if (chon == "3") quanLyHoaDon(im, cm);
            else if (chon == "4") banHang(pm, cm, im);
            else if (chon == "5") thongKe(im, pm);
            else if (chon == "6") doiTaiKhoanMatKhau(am, taiKhoan);
            else if (laAdmin && chon == "7") quanLyTaiKhoanNhanVien(am);
            else if (chon == to_string(soDangXuat)) {
                dangXuat = true;
                cout << "Da dang xuat.\n\n";
            } else {
                cout << "Lua chon khong hop le.\n";
            }
        }
    }

    return 0;
}
