// MyString.h
#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>
#include <cstring>

class MyString;
std::ostream& operator<<(std::ostream& os, const MyString& s);

class MyString {
public:
    MyString();
    MyString(const char* s);
    MyString(const MyString& other);
    ~MyString();
    MyString& operator=(const MyString& other);
    MyString& operator+=(const MyString& other);
    char operator[](int index) const;
    int length() const;
    int find(const MyString& str) const;
    const char* c_str() const;
    friend MyString operator+(const MyString& lhs, const MyString& rhs);
private:
    char* data;
    int len;
    void allocate_and_copy(const char* s);
    
};
bool operator==(const MyString& lhs, const MyString& rhs);
bool operator!=(const MyString& lhs, const MyString& rhs);
bool operator<(const MyString& lhs, const MyString& rhs);
bool operator>(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);
bool operator>=(const MyString& lhs, const MyString& rhs);

#endif // MYSTRING_H