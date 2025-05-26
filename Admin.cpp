// Admin.cpp
#include "Admin.h"
#include <cstring>

Admin::Admin(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password)
    : User(p_firstName, p_lastName, u32id, p_password, ADMIN) {
}

Admin::Admin(const MyString& p_dataLine)
    : User(p_dataLine) {
}

Teacher* Admin::addTeacher(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password) {
    return new Teacher(p_firstName, p_lastName, u32id, p_password);
}

Student* Admin::addStudent(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password) {
    return new Student(p_firstName, p_lastName, u32id, p_password);
}

void Admin::changePassword(MyString oldPassm, MyString newPass)
{
    if (oldPassm == getPassword())
    {
        setPassword(newPass);
    }
}

MyString Admin::serialize() const {
    return User::serialize();
}