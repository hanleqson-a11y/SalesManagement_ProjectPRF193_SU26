#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Utility.h"
#include "Customer.h"  // can CustomerManager de tim hoa don theo SDT khach hang

using namespace std;

class InvoiceItem {
public:
    int invoiceID;
    string productID;
    int quantity;
    double price; // don gia tai thoi diem ban

    InvoiceItem() : invoiceID(0), quantity(0), price(0) {}
    InvoiceItem(int invID, string pid, int qty, double p)
        : invoiceID(invID), productID(pid), quantity(qty), price(p) {}
};

class Invoice {
public:
    int invoiceID;
    int customerID;
    string date; // dd-mm-yy
    vector<InvoiceItem> items;
    double totalAmount;

    Invoice() : invoiceID(0), customerID(0), totalAmount(0) {}
};

class InvoiceManager {
private:
    vector<Invoice> invoices;
    string filename;

public:
    InvoiceManager(string file = "invoice.txt") : filename(file) {
        loadFromFile();
    }

    // Dinh dang file invoice.txt:
    // InvoiceID CustomerID Date TotalAmount ItemCount
    // ProductID Quantity Price   (lap lai ItemCount dong)
    void loadFromFile() {
        invoices.clear();
        ifstream fin(filename);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line.empty()) continue;
            vector<string> tok = splitTokens(line);
            if (tok.size() < 5) continue;
            Invoice inv;
            try {
                inv.invoiceID = stoi(tok[0]);
                inv.customerID = stoi(tok[1]);
                inv.date = tok[2];
                inv.totalAmount = stod(tok[3]);
                int itemCount = stoi(tok[4]);
                for (int i = 0; i < itemCount; i++) {
                    string itemLine;
                    if (!getline(fin, itemLine)) break;
                    vector<string> itok = splitTokens(itemLine);
                    if (itok.size() < 3) continue;
                    InvoiceItem item;
                    item.invoiceID = inv.invoiceID;
                    item.productID = itok[0];
                    item.quantity = stoi(itok[1]);
                    item.price = stod(itok[2]);
                    inv.items.push_back(item);
                }
            } catch (...) { continue; }
            invoices.push_back(inv);
        }
        fin.close();
    }

    void saveToFile() {
        ofstream fout(filename);
        for (auto &inv : invoices) {
            fout << inv.invoiceID << " " << inv.customerID << " " << inv.date << " "
                 << (long long)inv.totalAmount << " " << inv.items.size() << "\n";
            for (auto &item : inv.items) {
                fout << item.productID << " " << item.quantity << " " << (long long)item.price << "\n";
            }
        }
        fout.close();
    }

    int getMaxID() {
        int maxID = 0;
        for (auto &inv : invoices) if (inv.invoiceID > maxID) maxID = inv.invoiceID;
        return maxID;
    }

    int createInvoice(int customerID, const string &date, const vector<InvoiceItem> &items, double total) {
        Invoice inv;
        inv.invoiceID = getMaxID() + 1;
        inv.customerID = customerID;
        inv.date = date;
        inv.items = items;
        for (auto &it : inv.items) it.invoiceID = inv.invoiceID;
        inv.totalAmount = total;
        invoices.push_back(inv);
        saveToFile();
        return inv.invoiceID;
    }

    void printInvoice(const Invoice &inv) {
        cout << "-------------------------------------------\n";
        cout << "Hoa don ID: " << inv.invoiceID << " | Khach hang ID: " << inv.customerID
             << " | Ngay: " << inv.date << "\n";
        cout << left;
        cout.width(10); cout << "SanPhamID";
        cout.width(10); cout << "SoLuong";
        cout.width(15); cout << "DonGia";
        cout << "\n";
        for (auto &item : inv.items) {
            cout.width(10); cout << item.productID;
            cout.width(10); cout << item.quantity;
            cout.width(15); cout << (long long)item.price;
            cout << "\n";
        }
        cout << "Tong tien: " << (long long)inv.totalAmount << "\n";
    }

    void findByDate() {
        string date = readInput("Nhap ngay can tim (dd-mm-yy): ");
        bool found = false;
        for (auto &inv : invoices) {
            if (inv.date == date) { printInvoice(inv); found = true; }
        }
        if (!found) cout << "Khong tim thay hoa don nao trong ngay nay!\n";
    }

    // Tim hoa don theo khach hang (nhap SDT), su dung CustomerManager de tra ve CustomerID
    void findByCustomerPhone(CustomerManager &custMgr) {
        string phone = readInput("Nhap so dien thoai khach hang: ");
        int cidx = custMgr.findIndexByPhone(phone);
        if (cidx == -1) { cout << "Khong tim thay khach hang co SDT nay!\n"; return; }
        int customerID = custMgr.getAll()[cidx].customerID;
        bool found = false;
        for (auto &inv : invoices) {
            if (inv.customerID == customerID) { printInvoice(inv); found = true; }
        }
        if (!found) cout << "Khach hang nay chua co hoa don nao!\n";
    }

    void deleteInvoiceInteractive() {
        int id = readInt("Nhap ID hoa don can xoa: ");
        int idx = -1;
        for (size_t i = 0; i < invoices.size(); i++)
            if (invoices[i].invoiceID == id) { idx = (int)i; break; }
        if (idx == -1) { cout << "Khong tim thay hoa don co ID nay!\n"; return; }
        string confirm = readInput("Ban co chac muon xoa hoa don nay? (Y/N): ");
        if (toLower(confirm) == "y") {
            invoices.erase(invoices.begin() + idx);
            saveToFile();
            cout << "Xoa hoa don thanh cong!\n";
        } else {
            cout << "Da huy xoa.\n";
        }
    }

    void revenueByDateInteractive() {
        string date;
        while (true) {
            date = readInput("Nhap ngay can tinh doanh thu (dd-mm-yy): ");
            if (!isValidDateFormat(date)) {
                cout << "Dinh dang ngay khong hop le! Vui long nhap lai theo dang dd-mm-yy.\n";
                continue;
            }
            break;
        }
        double total = 0;
        int count = 0;
        for (auto &inv : invoices) {
            if (inv.date == date) { total += inv.totalAmount; count++; }
        }
        cout << "So hoa don trong ngay " << date << ": " << count << "\n";
        cout << "Doanh thu ngay " << date << ": " << (long long)total << "\n";
    }

    void findMenu(CustomerManager &custMgr) {
        while (true) {
            cout << "\n----- TIM HOA DON -----\n";
            cout << "3.1.1. Tim theo ngay\n";
            cout << "3.1.2. Tim theo thong tin khach hang (SDT)\n";
            cout << "(Nhap 'exit' de quay lai)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") findByDate();
                else if (choice == "2") findByCustomerPhone(custMgr);
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }

    void runMenu(CustomerManager &custMgr) {
        while (true) {
            cout << "\n===== 3. QUAN LY HOA DON =====\n";
            cout << "3.1. Tim hoa don\n";
            cout << "3.2. Xoa hoa don\n";
            cout << "(Nhap 'exit' de quay lai menu truoc do)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") findMenu(custMgr);
                else if (choice == "2") deleteInvoiceInteractive();
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }

    vector<Invoice>& getAll() { return invoices; }
};

#endif
