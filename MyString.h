// MyString.h
#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream> // For std::ostream
#include <cstring>  // For strlen, strcpy, strcat, strcmp

// Pre-declare operator<< for MyString
class MyString;
std::ostream& operator<<(std::ostream& os, const MyString& s); // NO 'friend' keyword here

class MyString {
public:
    // Constructors
    MyString();
    MyString(const char* s);
    MyString(const MyString& other); // Copy constructor

    // Destructor
    ~MyString();

    // Assignment operator
    MyString& operator=(const MyString& other);

    // Concatenation operator
    MyString& operator+=(const MyString& other);

    // Character access operator
    char operator[](int index) const;

    // Length of the string
    int length() const;

    // Find substring
    int find(const MyString& str) const;

    // C-style string access (PUBLIC GETTER)
    const char* c_str() const;

private:
    char* data;
    int len;

    // Helper for allocation and copy (member function, can access data)
    void allocate_and_copy(const char* s);
};

// Global comparison operators
bool operator==(const MyString& lhs, const MyString& rhs);
bool operator!=(const MyString& lhs, const MyString& rhs);

// Global concatenation operator
MyString operator+(const MyString& lhs, const MyString& rhs);

#endif // MYSTRING_H