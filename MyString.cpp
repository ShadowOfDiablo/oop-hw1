// MyString.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "MyString.h"
#include <iostream>
#include <cstring>

MyString::MyString() : data(nullptr), len(0) {
    allocate_and_copy("");
}

MyString::MyString(const char* s) : data(nullptr), len(0) {
    if (s) {
        allocate_and_copy(s);
    }
    else {
        allocate_and_copy("");
    }
}

MyString::MyString(const MyString& other) : data(nullptr), len(0) {
    allocate_and_copy(other.data);
}

MyString::~MyString() {
    delete[] data;
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        delete[] data;
        allocate_and_copy(other.data);
    }
    return *this;
}

MyString& MyString::operator+=(const MyString& other) {
    int new_len = len + other.len;
    char* new_data = new char[static_cast<size_t>(new_len) + 1U];
    (void)strcpy(new_data, data);
    (void)strcat(new_data, other.data);
    delete[] data;
    data = new_data;
    len = new_len;
    return *this;
}

char MyString::operator[](int index) const {
    char u8_ret_val = '\0';
    if ((index >= 0) && (index < len)) {
        u8_ret_val = data[static_cast<size_t>(index)];
    }
    return u8_ret_val;
}

int MyString::length() const {
    return len;
}

int MyString::find(const MyString& str) const {
    int u32_ret_val = -1;
    if (str.length() == 0) {
        u32_ret_val = 0;
    }
    else if (len >= str.length()) {
        const char* found_ptr = strstr(data, str.c_str());
        if (found_ptr != nullptr) {
            u32_ret_val = static_cast<int>(found_ptr - data);
        }
    }
    return u32_ret_val;
}

const char* MyString::c_str() const {
    return data;
}

MyString operator+(const MyString& lhs, const MyString& rhs) {
    MyString temp(lhs);
    temp += rhs;
    return temp;
}

void MyString::allocate_and_copy(const char* s) {
    len = static_cast<int>(strlen(s));
    data = new char[static_cast<size_t>(len) + 1U];
    (void)strcpy(data, s);
}

std::ostream& operator<<(std::ostream& os, const MyString& s) {
    os << s.c_str();
    return os;
}

bool operator==(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

bool operator!=(const MyString& lhs, const MyString& rhs) {
    return !(lhs == rhs);
}

bool operator<(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator>(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}

bool operator<=(const MyString& lhs, const MyString& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const MyString& lhs, const MyString& rhs) {
    return !(lhs < rhs);
}
