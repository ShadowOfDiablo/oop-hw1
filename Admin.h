#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Teacher.h"
#include "Student.h"
#include "MyString.h"

class Admin : public User {
public:
    Admin(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password);
    Admin(const MyString& p_dataLine);

    Teacher* addTeacher(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password);

    Student* addStudent(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password);

    void changePassword(MyString oldPassm, MyString newPass);

    MyString serialize() const;

    void sendMessage(size_t u32recipientId, MyString message);

    void viewMailbox() const;

};

#endif // ADMIN_H