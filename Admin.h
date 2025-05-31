#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "MyString.h"

class Teacher;
class Student;

class Admin : public User {
public:
    Admin(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password);
    Admin(const MyString& c_dataLine);
    Teacher* addTeacher(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password);
    Student* addStudent(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password);
    void changePassword(MyString c_oldPassm, MyString c_newPass);
    MyString serialize() const;
    void sendMessage(size_t u32_recipientId, MyString c_message);
    void viewMailbox() const;
    void message_all(MyString msg);
    void clearMailbox();
};

#endif // ADMIN_H
