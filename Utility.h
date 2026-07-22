#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

inline string getCurrentDateStr() {
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    ostringstream oss;
    oss << setfill('0') << setw(2) << now->tm_mday << "-"
        << setfill('0') << setw(2) << (now->tm_mon + 1) << "-"
        << setfill('0') << setw(2) << (now->tm_year % 100);
    return oss.str();
}

// Ném ra khi người dùng nhập "exit" -> báo hiệu quay lại menu trước đó
class ExitToPreviousMenu {};

inline string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Đọc 1 dòng nhập, tự động throw ExitToPreviousMenu nếu người dùng gõ "exit"
inline string readInput(const string &prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        // Stdin da dong / EOF -> coi nhu thoat
        throw ExitToPreviousMenu();
    }
    line = trim(line);
    if (line == "exit") throw ExitToPreviousMenu();
    return line;
}

// Đọc số nguyên, có kiểm tra hợp lệ, hỗ trợ exit
inline int readInt(const string &prompt) {
    while (true) {
        string s = readInput(prompt);
        if (s.empty()) { cout << "Vui long nhap gia tri!\n"; continue; }
        bool ok = true;
        size_t i = 0;
        if (s[0] == '-') i = 1;
        if (i >= s.size()) ok = false;
        for (; i < s.size(); i++) if (!isdigit((unsigned char)s[i])) { ok = false; break; }
        if (!ok) { cout << "Gia tri khong hop le, vui long nhap lai (so nguyen)!\n"; continue; }
        try { return stoi(s); } catch (...) { cout << "Gia tri khong hop le!\n"; continue; }
    }
}

// Đọc số thực (giá tiền), hỗ trợ exit
inline double readDouble(const string &prompt) {
    while (true) {
        string s = readInput(prompt);
        try {
            size_t pos;
            double val = stod(s, &pos);
            if (pos != s.size()) throw invalid_argument("bad");
            return val;
        } catch (...) {
            cout << "Gia tri khong hop le, vui long nhap lai (so)!\n";
        }
    }
}

inline vector<string> splitTokens(const string &line) {
    vector<string> tokens;
    stringstream ss(line);
    string tok;
    while (ss >> tok) tokens.push_back(tok);
    return tokens;
}

inline string toLower(const string &s) {
    string r = s;
    transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return tolower(c); });
    return r;
}

// Kiểm tra chuỗi ngày dd-mm-yy hợp lệ đơn giản (chỉ kiểm tra định dạng số và dấu gạch ngang)
inline bool isValidDateFormat(const string &s) {
    if (s.size() != 8) return false;
    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) {
            if (s[i] != '-') return false;
        } else {
            if (!isdigit((unsigned char)s[i])) return false;
        }
    }
    return true;
}

#endif
