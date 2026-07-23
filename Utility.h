#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
using namespace std;

// ============================================================
//  UTILITY.H
//  Chua cac ham dung chung cho ca chuong trinh:
//   - Doc du lieu nguoi dung nhap (co ho tro go "exit" de quay lai)
//   - Xu ly chuoi
//   - Xu ly ngay thang
// ============================================================

// Cat khoang trang o dau va cuoi chuoi
string trim(string s) {
    int start = 0;
    while (start < (int)s.size() && s[start] == ' ') start++;
    int end = (int)s.size() - 1;
    while (end >= 0 && s[end] == ' ') end--;
    return s.substr(start, end - start + 1);
}

// Tach 1 chuoi thanh nhieu tu (cach nhau boi khoang trang)
// Vi du: "P001 Laptop Asus 25000000 15 Laptop" -> 6 phan tu
vector<string> splitBySpace(string line) {
    vector<string> result;
    stringstream ss(line);
    string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

// Noi cac phan tu tu vi tri "from" den "to" thanh 1 chuoi (dung khi ten co nhieu tu)
string joinWords(vector<string>& words, int from, int to) {
    string result = "";
    for (int i = from; i <= to && i < (int)words.size(); i++) {
        if (result != "") result += " ";
        result += words[i];
    }
    return result;
}

// Doc 1 dong nhap binh thuong (khong kiem tra "exit")
string inputLine(string prompt) {
    cout << prompt;
    string line;
    getline(cin, line);
    return trim(line);
}

// Doc 1 dong nhap, NEU nguoi dung go dung chu "exit" thi bao cho ham goi biet
// bang cach gan isExit = true. Ham goi se dung dieu nay de HUY thao tac
// va QUAY VE menu truoc do.
string inputOrExit(string prompt, bool &isExit) {
    cout << prompt;
    string line;
    getline(cin, line);
    line = trim(line);
    if (line == "exit") {
        isExit = true;
        return "";
    }
    isExit = false;
    return line;
}

// Doc 1 so nguyen, cho phep go "exit". Neu nhap sai dinh dang se bat nhap lai.
int inputIntOrExit(string prompt, bool &isExit) {
    while (true) {
        string line = inputOrExit(prompt, isExit);
        if (isExit) return 0;
        if (line == "") {
            cout << "Vui long nhap mot so nguyen!\n";
            continue;
        }
        bool ok = true;
        for (int i = 0; i < (int)line.size(); i++) {
            if (i == 0 && (line[i] == '-' || line[i] == '+')) continue;
            if (!isdigit((unsigned char)line[i])) { ok = false; break; }
        }
        if (!ok) {
            cout << "Vui long nhap mot so nguyen!\n";
            continue;
        }
        return stoi(line);
    }
}

// Doc 1 so thuc (co the co dau cham thap phan), cho phep go "exit"
double inputDoubleOrExit(string prompt, bool &isExit) {
    while (true) {
        string line = inputOrExit(prompt, isExit);
        if (isExit) return 0;
        if (line == "") {
            cout << "Vui long nhap mot con so!\n";
            continue;
        }
        try {
            size_t pos;
            double value = stod(line, &pos);
            if (pos != line.size()) throw 1;
            return value;
        } catch (...) {
            cout << "Vui long nhap mot con so!\n";
        }
    }
}

// Kiem tra 1 chuoi co dung dinh dang ngay DD-MM-YYYY khong
bool isValidDate(string date) {
    if (date.size() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit((unsigned char)date[i])) return false;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    if (month < 1 || month > 12) return false;
    int soNgayCuaThang[] = {31,29,31,30,31,30,31,31,30,31,30,31};
    if (day < 1 || day > soNgayCuaThang[month - 1]) return false;
    if (year < 2000 || year > 2100) return false;
    return true;
}

// Lay ngay hien tai, dinh dang DD-MM-YYYY
string getToday() {
    time_t now = time(0);
    tm *t = localtime(&now);
    char buf[32];
    sprintf(buf, "%02d-%02d-%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    return string(buf);
}

// Doi 1 chuoi thanh chu thuong (de so sanh khong phan biet hoa/thuong)
string toLowerStr(string s) {
    for (int i = 0; i < (int)s.size(); i++) s[i] = tolower(s[i]);
    return s;
}

// Kiem tra chuoi "text" co chua chuoi con "pattern" khong (khong phan biet hoa/thuong)
bool containsText(string text, string pattern) {
    return toLowerStr(text).find(toLowerStr(pattern)) != string::npos;
}

// In ra so tien co dau cham ngan cach hang nghin, kem chu "VND"
string formatMoney(double amount) {
    long long soTien = (long long)(amount + 0.5);
    string s = to_string(soTien);
    string result = "";
    int dem = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        result = s[i] + result;
        dem++;
        if (dem % 3 == 0 && i != 0) result = "." + result;
    }
    return result + " VND";
}

#endif
