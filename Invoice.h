#ifndef INVOICE_H
#define INVOICE_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "Utility.h"
using namespace std;

// ============================================================
//  INVOICE.H
//  Lop InvoiceItem : 1 dong san pham trong hoa don (ProductID, Quantity, Price)
//  Lop Invoice     : InvoiceID, CustomerID, Date, danh sach InvoiceItem, TotalAmount
//  Lop InvoiceManager : quan ly danh sach hoa don (doc/ghi file invoice.txt)
//
//  Dinh dang file invoice.txt do nhom tu de xuat, moi hoa don gom:
//     #MaHoaDon|MaKhachHang|Ngay|TongTien       <- dong tieu de, bat dau bang #
//     MaSanPham|SoLuong|DonGia                   <- 1 dong cho moi san pham
//     ... (co the nhieu dong san pham)
//     #END                                       <- dong ket thuc 1 hoa don
// ============================================================

class InvoiceItem {
public:
    string productId;
    int quantity;
    double price; // don gia tai thoi diem ban

    InvoiceItem() { quantity = 0; price = 0; }
    InvoiceItem(string productId, int quantity, double price) {
        this->productId = productId;
        this->quantity = quantity;
        this->price = price;
    }

    double thanhTien() {
        return quantity * price;
    }
};

class Invoice {
public:
    string id;
    int customerId; // 0 = khach vang lai
    string date;    // dinh dang DD-MM-YYYY
    vector<InvoiceItem> items;
    double totalAmount;

    Invoice() { customerId = 0; totalAmount = 0; }
    Invoice(string id, int customerId, string date) {
        this->id = id;
        this->customerId = customerId;
        this->date = date;
        totalAmount = 0;
    }

    void themSanPham(InvoiceItem item) {
        items.push_back(item);
        tinhLaiTong();
    }

    void tinhLaiTong() {
        totalAmount = 0;
        for (int i = 0; i < (int)items.size(); i++) totalAmount += items[i].thanhTien();
    }

    void inRa() {
        cout << "Hoa don: " << id << " | Khach hang ID: " << customerId
             << " | Ngay: " << date << " | Tong tien: " << formatMoney(totalAmount) << "\n";
        cout << left << "   " << setw(10) << "MaSP" << setw(10) << "SoLuong"
             << setw(15) << "DonGia" << "\n";
        for (int i = 0; i < (int)items.size(); i++) {
            cout << "   " << setw(10) << items[i].productId << setw(10) << items[i].quantity
                 << setw(15) << formatMoney(items[i].price) << "\n";
        }
    }
};

class InvoiceManager {
public:
    vector<Invoice> danhSach;
    string tenFile;

    InvoiceManager(string file = "invoice.txt") {
        tenFile = file;
        docFile();
    }

    void docFile() {
        danhSach.clear();
        ifstream fin(tenFile);
        if (!fin.is_open()) return;

        string line;
        Invoice hienTai;
        bool dangDocHoaDon = false;

        while (getline(fin, line)) {
            line = trim(line);
            if (line == "") continue;

            if (line[0] == '#') {
                if (line == "#END") {
                    if (dangDocHoaDon) danhSach.push_back(hienTai);
                    dangDocHoaDon = false;
                    continue;
                }
                // dong tieu de: #MaHoaDon|MaKhachHang|Ngay|TongTien
                vector<string> phan = tachChuoi(line.substr(1), '|');
                if (phan.size() < 3) continue;
                hienTai = Invoice();
                hienTai.id = phan[0];
                hienTai.customerId = stoi(phan[1]);
                hienTai.date = phan[2];
                dangDocHoaDon = true;
            } else if (dangDocHoaDon) {
                vector<string> phan = tachChuoi(line, '|');
                if (phan.size() < 3) continue;
                InvoiceItem item(phan[0], stoi(phan[1]), stod(phan[2]));
                hienTai.themSanPham(item);
            }
        }
        fin.close();
    }

    void ghiFile() {
        ofstream fout(tenFile);
        for (int i = 0; i < (int)danhSach.size(); i++) {
            Invoice &hd = danhSach[i];
            fout << "#" << hd.id << "|" << hd.customerId << "|" << hd.date << "|"
                 << fixed << setprecision(0) << hd.totalAmount << "\n";
            for (int j = 0; j < (int)hd.items.size(); j++) {
                fout << hd.items[j].productId << "|" << hd.items[j].quantity << "|"
                     << fixed << setprecision(0) << hd.items[j].price << "\n";
            }
            fout << "#END\n";
        }
        fout.close();
    }

    // Sinh ma hoa don moi, dang INV0001, INV0002, ...
    string idTiepTheo() {
        int maxSo = 0;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            string so = "";
            for (int k = 0; k < (int)danhSach[i].id.size(); k++) {
                if (isdigit((unsigned char)danhSach[i].id[k])) so += danhSach[i].id[k];
            }
            if (so != "" && stoi(so) > maxSo) maxSo = stoi(so);
        }
        char buf[16];
        sprintf(buf, "INV%04d", maxSo + 1);
        return string(buf);
    }

    void themHoaDon(Invoice hd) {
        danhSach.push_back(hd);
        ghiFile();
    }

    bool xoaHoaDon(string id) {
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].id == id) {
                danhSach.erase(danhSach.begin() + i);
                ghiFile();
                return true;
            }
        }
        return false;
    }

    vector<int> timTheoNgay(string date) {
        vector<int> ketQua;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].date == date) ketQua.push_back(i);
        }
        return ketQua;
    }

    vector<int> timTheoKhachHang(int customerId) {
        vector<int> ketQua;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].customerId == customerId) ketQua.push_back(i);
        }
        return ketQua;
    }

    // Tinh tong doanh thu trong 1 ngay
    double doanhThuNgay(string date) {
        double tong = 0;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].date == date) tong += danhSach[i].totalAmount;
        }
        return tong;
    }

    void inDanhSach(vector<int>& chiSo) {
        if (chiSo.empty()) { cout << "Khong tim thay hoa don nao.\n"; return; }
        for (int i = 0; i < (int)chiSo.size(); i++) {
            danhSach[chiSo[i]].inRa();
            cout << string(50, '-') << "\n";
        }
    }

private:
    // Ham noi bo: tach chuoi theo 1 ky tu phan cach (dung cho dinh dang file hoa don)
    vector<string> tachChuoi(string s, char delim) {
        vector<string> ketQua;
        string tu = "";
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == delim) {
                ketQua.push_back(tu);
                tu = "";
            } else {
                tu += s[i];
            }
        }
        ketQua.push_back(tu);
        return ketQua;
    }
};

#endif
