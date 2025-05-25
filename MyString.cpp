// MyString.cpp
#define _CRT_SECURE_NO_WARNINGS // To suppress strcpy/strcat/localtime warnings if compiling with MSVC

#include "MyString.h"
#include <iostream> // For std::ostream
#include <cstring>  // For strlen, strcpy, strcat, strcmp, strstr

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
    char* new_data = new char[new_len + 1];
    strcpy(new_data, data);
    strcat(new_data, other.data);
    delete[] data;
    data = new_data;
    len = new_len;
    return *this;
}

char MyString::operator[](int index) const {
    if (index >= 0 && index < len) {
        return data[index];
    }
    return '\0';
}

int MyString::length() const {
    return len;
}

int MyString::find(const MyString& str) const {
    if (str.length() == 0) {
        return 0;
    }
    if (len < str.length()) {
        return -1;
    }
    const char* found_ptr = strstr(data, str.c_str());
    if (found_ptr != nullptr) {
        return found_ptr - data;
    }
    return -1;
}

const char* MyString::c_str() const {
    return data;
}

void MyString::allocate_and_copy(const char* s) {
    len = strlen(s);
    data = new char[len + 1];
    strcpy(data, s);
}

std::ostream& operator<<(std::ostream& os, const MyString& s) {
    os << s.c_str();
    return os;
}

bool operator==(const MyString& lhs, const MyString& rhs) {
    return (lhs.length() == rhs.length()) && (strcmp(lhs.c_str(), rhs.c_str()) == 0);
}

bool operator!=(const MyString& lhs, const MyString& rhs) {
    return !(lhs == rhs);
}

MyString operator+(const MyString& lhs, const MyString& rhs) {
    int new_len = lhs.length() + rhs.length();
    char* new_data = new char[new_len + 1];
    strcpy(new_data, lhs.c_str());
    strcat(new_data, rhs.c_str());
    MyString result(new_data);
    delete[] new_data;
    return result;
}