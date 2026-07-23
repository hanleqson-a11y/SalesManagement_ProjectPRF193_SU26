#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "Utility.h"
using namespace std;

// ============================================================
//  CUSTOMER.H
//  Lop Customer: CustomerID, CustomerName, PhoneNumber
//  Lop CustomerManager: quan ly danh sach khach hang (doc/ghi file customer.txt)
// ============================================================

class Customer {
public:
    int id;
    string name;
    string phone;

    Customer() { id = 0; }
    Customer(int id, string name, string phone) {
        this->id = id;
        this->name = name;
        this->phone = phone;
    }

    void inRa() {
        cout << left << setw(6) << id << setw(25) << name << setw(15) << phone << "\n";
    }
};

class CustomerManager {
public:
    vector<Customer> danhSach;
    string tenFile;

    CustomerManager(string file = "customer.txt") {
        tenFile = file;
        docFile();
    }

    // Moi dong trong file: ID Ten(co the nhieu tu) SDT
    // Vi du: 1 Tran Thi B 0987654321
    // Dong "0 0 0" la dong dai dien cho khach vang lai, khong can luu vao danh sach
    void docFile() {
        danhSach.clear();
        ifstream fin(tenFile);
        if (!fin.is_open()) return;

        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line == "") continue;
            vector<string> tu = splitBySpace(line);
            if ((int)tu.size() < 3) continue;

            int n = (int)tu.size();
            int id = stoi(tu[0]);
            string phone = tu[n - 1];
            string name = joinWords(tu, 1, n - 2);

            if (id == 0 && phone == "0") continue; // bo qua dong khach vang lai
            danhSach.push_back(Customer(id, name, phone));
        }
        fin.close();
    }

    void ghiFile() {
        ofstream fout(tenFile);
        fout << "0 0 0\n"; // giu dong dai dien khach vang lai theo dinh dang file mau
        for (int i = 0; i < (int)danhSach.size(); i++) {
            fout << danhSach[i].id << " " << danhSach[i].name << " " << danhSach[i].phone << "\n";
        }
        fout.close();
    }

    int timTheoSdt(string phone) {
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].phone == phone) return i;
        }
        return -1;
    }

    int timTheoId(int id) {
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].id == id) return i;
        }
        return -1;
    }

    bool tonTaiSdt(string phone) {
        return timTheoSdt(phone) != -1;
    }

    // Tinh ID moi = ID lon nhat hien co + 1
    int idTiepTheo() {
        int maxId = 0;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].id > maxId) maxId = danhSach[i].id;
        }
        return maxId + 1;
    }

    // Them khach hang moi, tra ve chi so vua them (-1 neu sdt da ton tai)
    int themKhachHang(string phone, string name) {
        if (tonTaiSdt(phone)) return -1;
        int idMoi = idTiepTheo();
        danhSach.push_back(Customer(idMoi, name, phone));
        ghiFile();
        return (int)danhSach.size() - 1;
    }

    bool suaKhachHang(string phoneCu, string nameMoi, string phoneMoi) {
        int i = timTheoSdt(phoneCu);
        if (i == -1) return false;
        if (phoneMoi != phoneCu && tonTaiSdt(phoneMoi)) return false;
        danhSach[i].name = nameMoi;
        danhSach[i].phone = phoneMoi;
        ghiFile();
        return true;
    }

    bool xoaKhachHang(string phone) {
        int i = timTheoSdt(phone);
        if (i == -1) return false;
        danhSach.erase(danhSach.begin() + i);
        ghiFile();
        return true;
    }

    vector<int> timTheoTen(string tenCanTim) {
        vector<int> ketQua;
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (containsText(danhSach[i].name, tenCanTim)) ketQua.push_back(i);
        }
        return ketQua;
    }

    void inTieuDe() {
        cout << left << setw(6) << "ID" << setw(25) << "Ten khach hang"
             << setw(15) << "So dien thoai" << "\n";
        cout << string(46, '-') << "\n";
    }

    void inTatCa() {
        if (danhSach.empty()) { cout << "Khong co khach hang nao.\n"; return; }
        inTieuDe();
        for (int i = 0; i < (int)danhSach.size(); i++) danhSach[i].inRa();
    }

    void inDanhSach(vector<int>& chiSo) {
        if (chiSo.empty()) { cout << "Khong tim thay khach hang nao.\n"; return; }
        inTieuDe();
        for (int i = 0; i < (int)chiSo.size(); i++) danhSach[chiSo[i]].inRa();
    }
};

#endif
