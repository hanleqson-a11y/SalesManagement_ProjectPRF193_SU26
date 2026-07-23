#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "Utility.h"
using namespace std;

// ============================================================
//  PRODUCT.H
//  Lop Product: ProductID, ProductName, Price, Quantity, Category
//  Lop ProductManager: quan ly danh sach san pham (doc/ghi file product.txt)
// ============================================================

class Product {
public:
    string id;
    string name;
    double price;
    int quantity;
    string category;

    Product() { price = 0; quantity = 0; }
    Product(string id, string name, double price, int quantity, string category) {
        this->id = id;
        this->name = name;
        this->price = price;
        this->quantity = quantity;
        this->category = category;
    }

    void inRa() {
        cout << left << setw(8) << id << setw(25) << name
             << setw(15) << formatMoney(price) << setw(10) << quantity
             << setw(15) << category << "\n";
    }
};

class ProductManager {
public:
    vector<Product> danhSach;
    string tenFile;

    ProductManager(string file = "product.txt") {
        tenFile = file;
        docFile();
    }

    // Moi dong trong file: ID Ten(co the nhieu tu) Gia SoLuong Loai
    // Vi du: P001 Laptop Asus 25000000 15 Laptop
    void docFile() {
        danhSach.clear();
        ifstream fin(tenFile);
        if (!fin.is_open()) return;

        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line == "") continue;
            vector<string> tu = splitBySpace(line);
            if ((int)tu.size() < 5) continue;

            int n = (int)tu.size();
            string id = tu[0];
            string category = tu[n - 1];
            int quantity = stoi(tu[n - 2]);
            double price = stod(tu[n - 3]);
            string name = joinWords(tu, 1, n - 4);

            danhSach.push_back(Product(id, name, price, quantity, category));
        }
        fin.close();
    }

    void ghiFile() {
        ofstream fout(tenFile);
        for (int i = 0; i < (int)danhSach.size(); i++) {
            fout << danhSach[i].id << " " << danhSach[i].name << " "
                 << fixed << setprecision(0) << danhSach[i].price << " "
                 << danhSach[i].quantity << " " << danhSach[i].category << "\n";
        }
        fout.close();
    }

    // Tim san pham theo ID, tra ve chi so trong danhSach (-1 neu khong co)
    int timTheoId(string id) {
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].id == id) return i;
        }
        return -1;
    }

    bool tonTaiId(string id) {
        return timTheoId(id) != -1;
    }

    bool themSanPham(Product p) {
        if (tonTaiId(p.id)) return false;
        danhSach.push_back(p);
        ghiFile();
        return true;
    }

    bool suaSanPham(string id, string name, double price, int quantity, string category) {
        int i = timTheoId(id);
        if (i == -1) return false;
        danhSach[i].name = name;
        danhSach[i].price = price;
        danhSach[i].quantity = quantity;
        danhSach[i].category = category;
        ghiFile();
        return true;
    }

    bool xoaSanPham(string id) {
        int i = timTheoId(id);
        if (i == -1) return false;
        danhSach.erase(danhSach.begin() + i);
        ghiFile();
        return true;
    }

    // Tim tat ca san pham co ten chua "tenCanTim"
    vector<int> timTheoTen(string tenCanTim) {
        vector<int> ketQua;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (containsText(danhSach[i].name, tenCanTim)) ketQua.push_back(i);
        }
        return ketQua;
    }

    // Tim tat ca san pham co loai chua "loaiCanTim"
    vector<int> timTheoLoai(string loaiCanTim) {
        vector<int> ketQua;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (containsText(danhSach[i].category, loaiCanTim)) ketQua.push_back(i);
        }
        return ketQua;
    }

    // Tim tat ca san pham co so luong nho hon n
    vector<int> timSoLuongThapHon(int n) {
        vector<int> ketQua;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].quantity < n) ketQua.push_back(i);
        }
        return ketQua;
    }

    // Giam so luong ton kho khi ban hang. Tra ve false neu khong du hang.
    bool giamSoLuong(string id, int soLuongBan) {
        int i = timTheoId(id);
        if (i == -1) return false;
        if (danhSach[i].quantity < soLuongBan) return false;
        danhSach[i].quantity -= soLuongBan;
        ghiFile();
        return true;
    }

    void inTieuDe() {
        cout << left << setw(8) << "ID" << setw(25) << "Ten san pham"
             << setw(15) << "Gia" << setw(10) << "So luong" << setw(15) << "Loai" << "\n";
        cout << string(73, '-') << "\n";
    }

    void inTatCa() {
        if (danhSach.empty()) { cout << "Khong co san pham nao.\n"; return; }
        inTieuDe();
        for (int i = 0; i < (int)danhSach.size(); i++) danhSach[i].inRa();
    }

    // In ra danh sach san pham theo danh sach chi so (dung sau khi tim kiem)
    void inDanhSach(vector<int>& chiSo) {
        if (chiSo.empty()) { cout << "Khong tim thay san pham nao.\n"; return; }
        inTieuDe();
        for (int i = 0; i < (int)chiSo.size(); i++) danhSach[chiSo[i]].inRa();
    }
};

#endif
