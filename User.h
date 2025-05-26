// User.h
#ifndef USER_H
#define USER_H

#include "MyString.h"
#include "Vector.hpp"
#include "Map.hpp"

enum UserRole {
    ADMIN,
    TEACHER,
    STUDENT,
    UNKNOWN_ROLE
};

size_t myStringToInt(const MyString& p_myString);
MyString intToMyString(size_t u32value);
Vector<MyString> splitMyString(const MyString& p_myString, char u8_delimiter);

class User {
protected:
    MyString c_firstName;
    MyString c_lastName;
    size_t u32id;
    MyString c_password;
    UserRole e_role;
    Vector<MyString> c_inbox;
    Map<MyString, Vector<MyString>> courses;

public:
    User(const MyString& p_firstName, const MyString& p_lastName, size_t u32inputID, const MyString& p_password, UserRole e_inputRole);
    User(const MyString& p_dataLine);

    virtual ~User() {}

    MyString getFirstName() const;
    MyString getLastName() const;
    size_t getId() const;
    MyString getPassword() const;
    UserRole getRole() const;
    virtual void changePassword(MyString oldPassm, MyString newPass) {};
    void setPassword(const MyString& p_newPassword);

    void addMessage(const MyString& p_message);
    void viewInbox() const;

    MyString serialize() const;
};

#endif // USER_H