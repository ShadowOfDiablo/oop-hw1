#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "MyString.h"

class Student : public User {
public:
    Student(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password);
    Student(const MyString& c_dataLine);
    void enrollInCourse(const MyString& c_courseName);
    void submitAssignment(const MyString& p_courseName, const MyString& p_assignmentName, const MyString& c_submissionMessage);
    void view_assignment_submissions(const MyString& c_assignmentName);
    void viewGrades() const;
    void changePassword(MyString c_oldPassm, MyString c_newPass);
    MyString serialize() const;
    void sendMessage(size_t u32_recipientId, const MyString& c_message);
    void viewMailbox() const;
    void clearMailbox();
};

#endif // STUDENT_H
