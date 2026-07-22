#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Utility.h"
using namespace std;

class Account {
public:
    string username;
    string password;
    string role; // "Admin" (chu cua hang) hoac "Staff" (nhan vien)

    Account() {}
    Account(string u, string p, string r) : username(u), password(p), role(r) {}
};

class AccountManager {
private:
    vector<Account> accounts;
    string filename;

public:
    AccountManager(string file = "account.txt") : filename(file) {
        loadFromFile();
    }

    void loadFromFile() {
        accounts.clear();
        ifstream fin(filename);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line.empty()) continue;
            vector<string> tok = splitTokens(line);
            if (tok.size() < 3) continue;
            accounts.push_back(Account(tok[0], tok[1], tok[2]));
        }
        fin.close();
    }

    void saveToFile() {
        ofstream fout(filename);
        for (auto &a : accounts) {
            fout << a.username << " " << a.password << " " << a.role << "\n";
        }
        fout.close();
    }

    int findIndexByUsername(const string &username) {
        for (size_t i = 0; i < accounts.size(); i++)
            if (accounts[i].username == username) return (int)i;
        return -1;
    }

    // Dang nhap. Tra ve con tro Account neu thanh cong, nullptr neu nguoi dung go "exit" de thoat chuong trinh.
    Account* loginInteractive() {
        while (true) {
            string username, password;
            try {
                username = readInput("Nhap ten tai khoan (hoac 'exit' de thoat chuong trinh): ");
                password = readInput("Nhap mat khau: ");
            } catch (ExitToPreviousMenu&) {
                return nullptr;
            }
            int idx = findIndexByUsername(username);
            if (idx != -1 && accounts[idx].password == password) {
                cout << "Dang nhap thanh cong! Xin chao " << accounts[idx].username
                     << " (" << accounts[idx].role << ")\n";
                return &accounts[idx];
            } else {
                cout << "Sai ten tai khoan hoac mat khau, vui long nhap lai!\n";
            }
        }
    }

    void changeUsernameInteractive(string &currentUsername) {
        string oldUser = readInput("Nhap ten tai khoan hien tai: ");
        string oldPass = readInput("Nhap mat khau hien tai: ");
        int idx = findIndexByUsername(oldUser);
        if (idx == -1 || accounts[idx].password != oldPass) {
            cout << "Ten tai khoan hoac mat khau khong dung!\n";
            return;
        }
        string newUser;
        while (true) {
            newUser = readInput("Nhap ten tai khoan moi: ");
            if (findIndexByUsername(newUser) != -1) {
                cout << "Ten tai khoan da ton tai, vui long nhap ten khac!\n";
                continue;
            }
            break;
        }
        accounts[idx].username = newUser;
        saveToFile();
        if (oldUser == currentUsername) currentUsername = newUser;
        cout << "Doi ten tai khoan thanh cong!\n";
    }

    void changePasswordInteractive(string &currentUsername) {
        string oldUser = readInput("Nhap ten tai khoan hien tai: ");
        string oldPass = readInput("Nhap mat khau hien tai: ");
        int idx = findIndexByUsername(oldUser);
        if (idx == -1 || accounts[idx].password != oldPass) {
            cout << "Ten tai khoan hoac mat khau khong dung!\n";
            return;
        }
        string newPass = readInput("Nhap mat khau moi: ");
        accounts[idx].password = newPass;
        saveToFile();
        cout << "Doi mat khau thanh cong!\n";
    }

    void addStaffAccountInteractive() {
        string username;
        while (true) {
            username = readInput("Nhap ten tai khoan nhan vien moi: ");
            if (findIndexByUsername(username) != -1) {
                cout << "Ten tai khoan da ton tai, vui long nhap ten khac!\n";
                continue;
            }
            break;
        }
        string password = readInput("Nhap mat khau: ");
        accounts.push_back(Account(username, password, "Staff"));
        saveToFile();
        cout << "Tao tai khoan nhan vien thanh cong!\n";
    }

    void deleteStaffAccountInteractive(const string &currentUsername) {
        string username = readInput("Nhap ten tai khoan can xoa: ");
        int idx = findIndexByUsername(username);
        if (idx == -1) { cout << "Khong tim thay tai khoan nay!\n"; return; }
        if (accounts[idx].role == "Admin") {
            cout << "Khong the xoa tai khoan Admin!\n";
            return;
        }
        if (username == currentUsername) {
            cout << "Khong the xoa tai khoan ban dang dang nhap!\n";
            return;
        }
        string confirm = readInput("Ban co chac muon xoa tai khoan " + username + "? (Y/N): ");
        if (toLower(confirm) == "y") {
            accounts.erase(accounts.begin() + idx);
            saveToFile();
            cout << "Xoa tai khoan thanh cong!\n";
        } else {
            cout << "Da huy xoa.\n";
        }
    }

    // Muc 6: Doi ten tai khoan / mat khau
    void runChangeAccountMenu(string &currentUsername) {
        while (true) {
            cout << "\n===== 6. DOI TEN TAI KHOAN / MAT KHAU =====\n";
            cout << "6.1. Doi ten tai khoan\n";
            cout << "6.2. Doi mat khau\n";
            cout << "(Nhap 'exit' de quay lai menu truoc do)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") changeUsernameInteractive(currentUsername);
                else if (choice == "2") changePasswordInteractive(currentUsername);
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }

    // Muc 7: Tao/Xoa tai khoan nhan vien (chi Admin)
    void runAdminAccountMenu(const string &currentUsername) {
        while (true) {
            cout << "\n===== 7. TAO/XOA TAI KHOAN NHAN VIEN =====\n";
            cout << "7.1. Xoa tai khoan\n";
            cout << "7.2. Them tai khoan\n";
            cout << "(Nhap 'exit' de quay lai menu truoc do)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") deleteStaffAccountInteractive(currentUsername);
                else if (choice == "2") addStaffAccountInteractive();
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }
};

#endif
