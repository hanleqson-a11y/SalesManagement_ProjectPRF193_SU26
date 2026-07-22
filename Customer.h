#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Utility.h"
using namespace std;

class Customer {
public:
    int customerID;
    string customerName;
    string phoneNumber;

    Customer() : customerID(0) {}
    Customer(int id, string name, string phone)
        : customerID(id), customerName(name), phoneNumber(phone) {}
};

class CustomerManager {
private:
    vector<Customer> customers;
    string filename;

public:
    CustomerManager(string file = "customer.txt") : filename(file) {
        loadFromFile();
    }

    void loadFromFile() {
        customers.clear();
        ifstream fin(filename);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line.empty()) continue;
            vector<string> tok = splitTokens(line);
            // dinh dang: ID <ten... co the nhieu tu> SDT
            if (tok.size() < 3) continue;
            int id;
            try { id = stoi(tok[0]); } catch (...) { continue; }
            string phone = tok[tok.size() - 1];
            string name;
            for (size_t i = 1; i + 1 < tok.size(); i++) {
                if (i > 1) name += " ";
                name += tok[i];
            }
            customers.push_back(Customer(id, name, phone));
        }
        fin.close();
    }

    void saveToFile() {
        ofstream fout(filename);
        for (auto &c : customers) {
            fout << c.customerID << " " << c.customerName << " " << c.phoneNumber << "\n";
        }
        fout.close();
    }

    int findIndexByPhone(const string &phone) {
        for (size_t i = 0; i < customers.size(); i++)
            if (customers[i].phoneNumber == phone) return (int)i;
        return -1;
    }

    int findIndexByID(int id) {
        for (size_t i = 0; i < customers.size(); i++)
            if (customers[i].customerID == id) return (int)i;
        return -1;
    }

    int getMaxID() {
        int maxID = 0;
        for (auto &c : customers) if (c.customerID > maxID) maxID = c.customerID;
        return maxID;
    }

    vector<Customer>& getAll() { return customers; }

    void displayHeader() {
        cout << left;
        cout << "-----------------------------------------------------------\n";
        cout.width(8);  cout << "ID";
        cout.width(25); cout << "Ten khach hang";
        cout.width(15); cout << "SDT";
        cout << "\n-----------------------------------------------------------\n";
    }

    void printCustomer(const Customer &c) {
        cout << left;
        cout.width(8);  cout << c.customerID;
        cout.width(25); cout << c.customerName;
        cout.width(15); cout << c.phoneNumber;
        cout << "\n";
    }

    void displayAll() {
        if (customers.empty()) { cout << "Danh sach khach hang dang trong!\n"; return; }
        displayHeader();
        for (auto &c : customers) printCustomer(c);
    }

    // Them khach hang moi (dung cho muc 2.1), tra ve ID khach hang vua them
    int addCustomerInteractive() {
        string phone;
        while (true) {
            phone = readInput("Nhap so dien thoai: ");
            if (findIndexByPhone(phone) != -1) {
                cout << "So dien thoai da ton tai! Vui long nhap lai.\n";
                continue;
            }
            break;
        }
        string name = readInput("Nhap ten khach hang: ");
        int newID = getMaxID() + 1;
        customers.push_back(Customer(newID, name, phone));
        saveToFile();
        cout << "Them khach hang thanh cong! (ID: " << newID << ")\n";
        return newID;
    }

    void editCustomer() {
        string phone = readInput("Nhap so dien thoai khach hang can sua: ");
        int idx = findIndexByPhone(phone);
        if (idx == -1) { cout << "Khong tim thay khach hang co SDT nay!\n"; return; }
        Customer &c = customers[idx];
        cout << "Thong tin hien tai:\n";
        displayHeader();
        printCustomer(c);
        string name = readInput("Nhap ten moi (Enter de giu nguyen): ");
        if (!name.empty()) c.customerName = name;
        string newPhone = readInput("Nhap so dien thoai moi (Enter de giu nguyen): ");
        if (!newPhone.empty()) {
            if (findIndexByPhone(newPhone) != -1 && newPhone != phone) {
                cout << "So dien thoai moi da ton tai, giu nguyen so cu.\n";
            } else {
                c.phoneNumber = newPhone;
            }
        }
        saveToFile();
        cout << "Cap nhat khach hang thanh cong!\n";
    }

    void deleteCustomer() {
        string phone = readInput("Nhap so dien thoai khach hang can xoa: ");
        int idx = findIndexByPhone(phone);
        if (idx == -1) { cout << "Khong tim thay khach hang co SDT nay!\n"; return; }
        string confirm = readInput("Ban co chac muon xoa khach hang " + customers[idx].customerName + "? (Y/N): ");
        if (toLower(confirm) == "y") {
            customers.erase(customers.begin() + idx);
            saveToFile();
            cout << "Xoa khach hang thanh cong!\n";
        } else {
            cout << "Da huy xoa.\n";
        }
    }

    void searchByName() {
        string name = readInput("Nhap ten khach hang can tim: ");
        bool found = false;
        displayHeader();
        for (auto &c : customers) {
            if (toLower(c.customerName).find(toLower(name)) != string::npos) {
                printCustomer(c);
                found = true;
            }
        }
        if (!found) cout << "Khong tim thay khach hang nao!\n";
    }

    void searchByID() {
        int id = readInt("Nhap ID khach hang can tim: ");
        int idx = findIndexByID(id);
        if (idx == -1) { cout << "Khong tim thay khach hang co ID nay!\n"; return; }
        displayHeader();
        printCustomer(customers[idx]);
    }

    void searchByPhone() {
        string phone = readInput("Nhap so dien thoai can tim: ");
        int idx = findIndexByPhone(phone);
        if (idx == -1) { cout << "Khong tim thay khach hang co SDT nay!\n"; return; }
        displayHeader();
        printCustomer(customers[idx]);
    }

    void searchMenu() {
        while (true) {
            cout << "\n----- TIM KIEM KHACH HANG -----\n";
            cout << "2.4.1. Tim theo ten\n";
            cout << "2.4.2. Tim theo ID\n";
            cout << "2.4.3. Tim theo so dien thoai\n";
            cout << "(Nhap 'exit' de quay lai)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") searchByName();
                else if (choice == "2") searchByID();
                else if (choice == "3") searchByPhone();
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }

    void runMenu() {
        while (true) {
            cout << "\n===== 2. QUAN LY KHACH HANG =====\n";
            cout << "2.1. Them thong tin khach hang\n";
            cout << "2.2. Sua thong tin khach hang\n";
            cout << "2.3. Xoa thong tin khach hang\n";
            cout << "2.4. Tim kiem thong tin khach hang\n";
            cout << "2.5. Hien thi tat ca thong tin khach hang\n";
            cout << "(Nhap 'exit' de quay lai menu truoc do)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") addCustomerInteractive();
                else if (choice == "2") editCustomer();
                else if (choice == "3") deleteCustomer();
                else if (choice == "4") searchMenu();
                else if (choice == "5") displayAll();
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }
};

#endif
