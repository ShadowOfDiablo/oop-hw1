#ifndef USER_H
#define USER_H

#include "MyString.h"
#include "Vector.hpp"
#include "Map.hpp"
#include "CourseManagementSystem.hpp"
#include <ctime>

enum UserRole {
    ADMIN,
    TEACHER,
    STUDENT,
    UNKNOWN_ROLE
};

class CourseManagementSystem;

class User {
protected:
    MyString c_firstName;
    MyString c_lastName;
    size_t u32id;
    MyString c_password;
    UserRole e_role = UNKNOWN_ROLE;
    Map<size_t, Vector<std::pair<MyString, time_t>>> inbox;
    Map<MyString, Vector<MyString>> courses;
public:
    User(const MyString& c_firstName, const MyString& c_lastName, size_t u32id, const MyString& c_password, UserRole e_inputRole);
    User(const MyString& c_dataLine);
    virtual ~User() {}
    MyString getFirstName() const;
    MyString getLastName() const;
    size_t getId() const;
    MyString getPassword() const;
    UserRole getRole() const;
    virtual void changePassword(MyString c_oldPassm, MyString c_newPass);
    void setPassword(const MyString& c_newPassword);
    void sendMessage(size_t u32recipientId, const MyString& c_message);
    void viewMailbox() const;
    virtual void clearMailbox();
    virtual MyString serialize() const;
};

#endif // USER_H
