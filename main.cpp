#include <iostream>
#include <vector>
#include <string>
#include "Utility.h"
#include "Account.h"
#include "Product.h"
#include "Customer.h"
#include "Invoice.h"
using namespace std;

// ================= LUONG BAN HANG (muc 4) =================
void salesFlow(ProductManager &productMgr, CustomerManager &customerMgr, InvoiceManager &invoiceMgr) {
    int customerID = 0; // mac dinh: khach vang lai (id=0, ten=0, sdt=0)

    string wantSave = readInput("Khach hang co muon luu thong tin khong? (Y/N): ");
    if (toLower(wantSave) == "y") {
        string phone = readInput("Nhap so dien thoai: ");
        int idx = customerMgr.findIndexByPhone(phone);
        if (idx != -1) {
            customerID = customerMgr.getAll()[idx].customerID;
            cout << "Da tim thay khach hang: " << customerMgr.getAll()[idx].customerName << "\n";
        } else {
            string name = readInput("Nhap ten khach hang: ");
            int newID = customerMgr.getMaxID() + 1;
            customerMgr.getAll().push_back(Customer(newID, name, phone));
            customerMgr.saveToFile();
            customerID = newID;
            cout << "Da luu thong tin khach hang moi (ID: " << newID << ")\n";
        }
    } else {
        customerID = 0;
    }

    vector<InvoiceItem> cart;
    double total = 0;

    while (true) {
        string pid = readInput("Nhap ID san pham: ");
        Product *p = productMgr.getProductPtr(pid);
        if (!p) {
            cout << "Khong tim thay san pham co ID nay!\n";
        } else {
            // Tinh so luong da co trong gio hang cho san pham nay de biet ton kho con lai thuc te
            int reserved = 0;
            for (auto &it : cart) if (it.productID == pid) reserved += it.quantity;
            int available = p->quantity - reserved;

            int qty;
            while (true) {
                qty = readInt("Nhap so luong: ");
                if (qty <= 0) { cout << "So luong phai lon hon 0!\n"; continue; }
                if (qty > available) {
                    cout << "Chi con " << available << " san pham vui long nhap lai\n";
                    continue;
                }
                break;
            }
            InvoiceItem item;
            item.productID = pid;
            item.quantity = qty;
            item.price = p->price;
            cart.push_back(item);
            total += p->price * qty;
            cout << "Da them " << qty << " x " << p->productName << " vao hoa don.\n";
        }

        string more = readInput("Mua them san pham khac? (Y/N): ");
        if (toLower(more) != "y") break;
    }

    if (cart.empty()) {
        cout << "Khong co san pham nao duoc mua, huy hoa don.\n";
        return;
    }

    // Ap dung tru so luong ton kho thuc te va luu hoa don
    for (auto &item : cart) {
        productMgr.decreaseQuantity(item.productID, item.quantity);
    }
    string date = getCurrentDateStr();
    int invoiceID = invoiceMgr.createInvoice(customerID, date, cart, total);

    cout << "So tien can thanh toan la: " << (long long)total << "\n";
    cout << "Hoa don ID " << invoiceID << " da duoc luu thanh cong!\n";
}

// ================= LUONG THONG KE (muc 5) =================
void statisticsMenu(ProductManager &productMgr, InvoiceManager &invoiceMgr) {
    while (true) {
        cout << "\n===== 5. THONG KE =====\n";
        cout << "5.1. Doanh thu ngay (DD-MM-YY)\n";
        cout << "5.2. Kiem tra cac san pham co so luong thap hon n\n";
        cout << "(Nhap 'exit' de quay lai menu truoc do)\n";
        string choice;
        try {
            choice = readInput("Chon muc: ");
        } catch (ExitToPreviousMenu&) { return; }

        try {
            if (choice == "1") invoiceMgr.revenueByDateInteractive();
            else if (choice == "2") productMgr.lowStockCheckInteractive();
            else cout << "Lua chon khong hop le!\n";
        } catch (ExitToPreviousMenu&) { continue; }
    }
}

// ================= MENU CHINH (sau khi dang nhap) =================
// Neu 'exit' duoc nhap tai day, ham se return va vong lap main() se quay ve man hinh dang nhap.
void mainMenuLoop(string currentUsername, string currentRole, ProductManager &productMgr,
                   CustomerManager &customerMgr, InvoiceManager &invoiceMgr, AccountManager &accountMgr) {
    bool isAdmin = (currentRole == "Admin");

    while (true) {
        cout << "\n===================== MENU CHINH =====================\n";
        cout << "Tai khoan: " << currentUsername << " | Vai tro: " << currentRole << "\n";
        cout << "1. Quan ly san pham\n";
        cout << "2. Quan ly khach hang\n";
        cout << "3. Quan ly hoa don\n";
        cout << "4. Ban hang\n";
        cout << "5. Thong ke\n";
        cout << "6. Doi ten tai khoan/mat khau\n";
        if (isAdmin) {
            cout << "7. Tao/Xoa tai khoan nhan vien\n";
        }
        cout << "0. Dang xuat\n";
        cout << "(Nhap 'exit' de quay lai man hinh dang nhap)\n";

        string choice;
        try {
            choice = readInput("Chon muc: ");
        } catch (ExitToPreviousMenu&) { return; }

        try {
            if (choice == "1") productMgr.runMenu();
            else if (choice == "2") customerMgr.runMenu();
            else if (choice == "3") invoiceMgr.runMenu(customerMgr);
            else if (choice == "4") salesFlow(productMgr, customerMgr, invoiceMgr);
            else if (choice == "5") statisticsMenu(productMgr, invoiceMgr);
            else if (choice == "6") accountMgr.runChangeAccountMenu(currentUsername);
            else if (choice == "7" && isAdmin) accountMgr.runAdminAccountMenu(currentUsername);
            else if (choice == "0") return;
            else cout << "Lua chon khong hop le!\n";
        } catch (ExitToPreviousMenu&) { continue; }
    }
}

int main() {
    AccountManager accountMgr;
    ProductManager productMgr;
    CustomerManager customerMgr;
    InvoiceManager invoiceMgr;

    cout << "========================================================\n";
    cout << "   CHUONG TRINH QUAN LY BAN HANG\n";
    cout << "   * Trong chuong trinh ban co the nhap chinh xac tu\n";
    cout << "     \"exit\" de tro ve trang truoc do.\n";
    cout << "========================================================\n";

    while (true) {
        cout << "\n----- DANG NHAP -----\n";
        Account *current = accountMgr.loginInteractive();
        if (current == nullptr) {
            cout << "Tam biet!\n";
            break;
        }
        string currentUsername = current->username;
        string currentRole = current->role;
        try {
            mainMenuLoop(currentUsername, currentRole, productMgr, customerMgr, invoiceMgr, accountMgr);
        } catch (ExitToPreviousMenu&) {
            // quay ve man hinh dang nhap
        }
        cout << "Da dang xuat.\n";
    }

    return 0;
}
