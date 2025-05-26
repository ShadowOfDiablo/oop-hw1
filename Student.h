#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "MyString.h"

class Student : public User {
public:
    Student(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password);
    Student(const MyString& p_dataLine);

    void enrollInCourse(const MyString& p_courseName);
    void submitAssignment(const MyString& p_courseName, const MyString& p_assignmentName);
    void viewGrades() const;

    void changePassword(MyString oldPassm, MyString newPass);

    MyString serialize() const ;
    void sendMessage(size_t u32recipientId, const MyString& c_message);
    void viewMailbox() const;
};

#endif // STUDENT_H