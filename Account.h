#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Utility.h"
using namespace std;

// ============================================================
//  ACCOUNT.H
//  Lop Account: 1 tai khoan dang nhap gom Username, Password, Role
//  Lop AccountManager: quan ly danh sach tai khoan (doc/ghi file account.txt)
// ============================================================

class Account {
public:
    string username;
    string password;
    string role; // "Admin" (chu cua hang) hoac "Staff" (nhan vien)

    Account() {}
    Account(string u, string p, string r) {
        username = u;
        password = p;
        role = r;
    }

    bool isAdmin() {
        return toLowerStr(role) == "admin";
    }
};

class AccountManager {
public:
    vector<Account> danhSach;
    string tenFile;

    AccountManager(string file = "account.txt") {
        tenFile = file;
        docFile();
    }

    // Doc danh sach tai khoan tu file
    // Moi dong trong file co dang: username password role
    void docFile() {
        danhSach.clear();
        ifstream fin(tenFile);
        if (!fin.is_open()) return;

        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line == "") continue;
            vector<string> tu = splitBySpace(line);
            if (tu.size() < 3) continue;
            danhSach.push_back(Account(tu[0], tu[1], tu[2]));
        }
        fin.close();
    }

    // Ghi lai toan bo danh sach tai khoan xuong file
    void ghiFile() {
        ofstream fout(tenFile);
        for (int i = 0; i < (int)danhSach.size(); i++) {
            fout << danhSach[i].username << " " << danhSach[i].password << " "
                 << danhSach[i].role << "\n";
        }
        fout.close();
    }

    // Tim tai khoan theo username, tra ve chi so trong danhSach (-1 neu khong co)
    int timTheoUsername(string username) {
        for (int i = 0; i < (int)danhSach.size(); i++) {
            if (danhSach[i].username == username) return i;
        }
        return -1;
    }

    bool tonTaiUsername(string username) {
        return timTheoUsername(username) != -1;
    }

    // Kiem tra dang nhap, tra ve chi so tai khoan neu dung, -1 neu sai
    int dangNhap(string username, string password) {
        int i = timTheoUsername(username);
        if (i != -1 && danhSach[i].password == password) return i;
        return -1;
    }

    bool themTaiKhoan(string username, string password, string role) {
        if (tonTaiUsername(username)) return false;
        danhSach.push_back(Account(username, password, role));
        ghiFile();
        return true;
    }

    bool xoaTaiKhoan(string username) {
        int i = timTheoUsername(username);
        if (i == -1) return false;
        danhSach.erase(danhSach.begin() + i);
        ghiFile();
        return true;
    }

    bool doiUsername(string usernameCu, string password, string usernameMoi) {
        int i = timTheoUsername(usernameCu);
        if (i == -1 || danhSach[i].password != password) return false;
        if (tonTaiUsername(usernameMoi)) return false;
        danhSach[i].username = usernameMoi;
        ghiFile();
        return true;
    }

    bool doiPassword(string username, string passwordCu, string passwordMoi) {
        int i = timTheoUsername(username);
        if (i == -1 || danhSach[i].password != passwordCu) return false;
        danhSach[i].password = passwordMoi;
        ghiFile();
        return true;
    }
};

#endif
