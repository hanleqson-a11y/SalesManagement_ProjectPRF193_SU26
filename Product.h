#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Utility.h"
using namespace std;

class Product {
public:
    string productID;
    string productName;
    double price;
    int quantity;
    string category;

    Product() : price(0), quantity(0) {}
    Product(string id, string name, double p, int q, string cat)
        : productID(id), productName(name), price(p), quantity(q), category(cat) {}
};

class ProductManager {
private:
    vector<Product> products;
    string filename;

public:
    ProductManager(string file = "product.txt") : filename(file) {
        loadFromFile();
    }

    void loadFromFile() {
        products.clear();
        ifstream fin(filename);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line.empty()) continue;
            vector<string> tok = splitTokens(line);
            // dinh dang: ID <ten... co the nhieu tu> gia so_luong loai
            if (tok.size() < 5) continue;
            string id = tok[0];
            string category = tok[tok.size() - 1];
            int qty = 0;
            double price = 0;
            try {
                qty = stoi(tok[tok.size() - 2]);
                price = stod(tok[tok.size() - 3]);
            } catch (...) { continue; }
            string name;
            for (size_t i = 1; i + 4 <= tok.size(); i++) {
                if (i > 1) name += " ";
                name += tok[i];
            }
            products.push_back(Product(id, name, price, qty, category));
        }
        fin.close();
    }

    void saveToFile() {
        ofstream fout(filename);
        for (auto &p : products) {
            fout << p.productID << " " << p.productName << " "
                 << (long long)p.price << " " << p.quantity << " " << p.category << "\n";
        }
        fout.close();
    }

    int findIndexByID(const string &id) {
        for (size_t i = 0; i < products.size(); i++)
            if (products[i].productID == id) return (int)i;
        return -1;
    }

    vector<Product>& getAll() { return products; }

    // Tra ve con tro toi san pham theo ID, nullptr neu khong tim thay (dung cho luong Ban hang)
    Product* getProductPtr(const string &id) {
        int idx = findIndexByID(id);
        if (idx == -1) return nullptr;
        return &products[idx];
    }

    bool decreaseQuantity(const string &id, int qty) {
        int idx = findIndexByID(id);
        if (idx == -1) return false;
        if (products[idx].quantity < qty) return false;
        products[idx].quantity -= qty;
        saveToFile();
        return true;
    }

    void displayHeader() {
        cout << left;
        cout << "-----------------------------------------------------------------------\n";
        cout.width(8);  cout << "ID";
        cout.width(20); cout << "Ten SP";
        cout.width(15); cout << "Gia";
        cout.width(10); cout << "SL";
        cout.width(15); cout << "Loai";
        cout << "\n-----------------------------------------------------------------------\n";
    }

    void printProduct(const Product &p) {
        cout << left;
        cout.width(8);  cout << p.productID;
        cout.width(20); cout << p.productName;
        cout.width(15); cout << (long long)p.price;
        cout.width(10); cout << p.quantity;
        cout.width(15); cout << p.category;
        cout << "\n";
    }

    void displayAll() {
        if (products.empty()) { cout << "Danh sach san pham dang trong!\n"; return; }
        displayHeader();
        for (auto &p : products) printProduct(p);
    }

    void addProduct() {
        string id = readInput("Nhap ID san pham (hoac 'exit' de quay lai): ");
        if (findIndexByID(id) != -1) {
            cout << "ID da ton tai! Vui long thu lai voi ID khac.\n";
            return;
        }
        string name = readInput("Nhap ten san pham: ");
        double price = readDouble("Nhap gia san pham: ");
        int qty = readInt("Nhap so luong: ");
        string category = readInput("Nhap loai san pham: ");
        products.push_back(Product(id, name, price, qty, category));
        saveToFile();
        cout << "Them san pham thanh cong!\n";
    }

    void editProduct() {
        string id = readInput("Nhap ID san pham can sua: ");
        int idx = findIndexByID(id);
        if (idx == -1) { cout << "Khong tim thay san pham co ID nay!\n"; return; }
        Product &p = products[idx];
        cout << "Thong tin hien tai:\n";
        displayHeader();
        printProduct(p);
        string name = readInput("Nhap ten moi (Enter de giu nguyen): ");
        if (!name.empty()) p.productName = name;
        string priceStr = readInput("Nhap gia moi (Enter de giu nguyen): ");
        if (!priceStr.empty()) {
            try { p.price = stod(priceStr); } catch (...) { cout << "Gia khong hop le, giu nguyen gia cu.\n"; }
        }
        string qtyStr = readInput("Nhap so luong moi (Enter de giu nguyen): ");
        if (!qtyStr.empty()) {
            try { p.quantity = stoi(qtyStr); } catch (...) { cout << "So luong khong hop le, giu nguyen.\n"; }
        }
        string cat = readInput("Nhap loai moi (Enter de giu nguyen): ");
        if (!cat.empty()) p.category = cat;
        saveToFile();
        cout << "Cap nhat san pham thanh cong!\n";
    }

    void deleteProduct() {
        string id = readInput("Nhap ID san pham can xoa: ");
        int idx = findIndexByID(id);
        if (idx == -1) { cout << "Khong tim thay san pham co ID nay!\n"; return; }
        string confirm = readInput("Ban co chac muon xoa san pham " + products[idx].productName + "? (Y/N): ");
        if (toLower(confirm) == "y") {
            products.erase(products.begin() + idx);
            saveToFile();
            cout << "Xoa san pham thanh cong!\n";
        } else {
            cout << "Da huy xoa.\n";
        }
    }

    void searchByName() {
        string name = readInput("Nhap ten san pham can tim: ");
        bool found = false;
        displayHeader();
        for (auto &p : products) {
            if (toLower(p.productName).find(toLower(name)) != string::npos) {
                printProduct(p);
                found = true;
            }
        }
        if (!found) cout << "Khong tim thay san pham nao!\n";
    }

    void searchByID() {
        string id = readInput("Nhap ID san pham can tim: ");
        int idx = findIndexByID(id);
        if (idx == -1) { cout << "Khong tim thay san pham co ID nay!\n"; return; }
        displayHeader();
        printProduct(products[idx]);
    }

    void searchByCategory() {
        string cat = readInput("Nhap loai san pham can tim: ");
        bool found = false;
        displayHeader();
        for (auto &p : products) {
            if (toLower(p.category).find(toLower(cat)) != string::npos) {
                printProduct(p);
                found = true;
            }
        }
        if (!found) cout << "Khong tim thay san pham nao!\n";
    }

    void searchMenu() {
        while (true) {
            cout << "\n----- TIM KIEM SAN PHAM -----\n";
            cout << "1.4.1. Tim theo ten\n";
            cout << "1.4.2. Tim theo ID\n";
            cout << "1.4.3. Tim theo loai\n";
            cout << "(Nhap 'exit' de quay lai)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") searchByName();
                else if (choice == "2") searchByID();
                else if (choice == "3") searchByCategory();
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }

    void lowStockCheckInteractive() {
        int n = readInt("Nhap so luong n: ");
        bool found = false;
        displayHeader();
        for (auto &p : products) {
            if (p.quantity < n) { printProduct(p); found = true; }
        }
        if (!found) cout << "Khong co san pham nao co so luong thap hon " << n << "!\n";
    }

    void runMenu() {
        while (true) {
            cout << "\n===== 1. QUAN LY SAN PHAM =====\n";
            cout << "1.1. Them san pham\n";
            cout << "1.2. Sua san pham\n";
            cout << "1.3. Xoa san pham\n";
            cout << "1.4. Tim kiem san pham\n";
            cout << "1.5. Hien thi tat ca san pham\n";
            cout << "(Nhap 'exit' de quay lai menu truoc do)\n";
            string choice;
            try {
                choice = readInput("Chon muc: ");
            } catch (ExitToPreviousMenu&) { return; }

            try {
                if (choice == "1") addProduct();
                else if (choice == "2") editProduct();
                else if (choice == "3") deleteProduct();
                else if (choice == "4") searchMenu();
                else if (choice == "5") displayAll();
                else cout << "Lua chon khong hop le!\n";
            } catch (ExitToPreviousMenu&) { continue; }
        }
    }
};

#endif
