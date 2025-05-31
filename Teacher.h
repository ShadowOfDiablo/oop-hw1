#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include "MyString.h"
#include "Vector.hpp"
#include "Map.hpp"

class Course; // Forward declaration

class Teacher : public User {
private:
    Map<MyString, Course*> m_taughtCourses;
public:
    Teacher(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password);
    Teacher(const MyString& c_dataLine);
    ~Teacher() override;

    Course* createCourse(const MyString& c_courseName, const MyString& c_coursePassword);
    bool addToCourse(const MyString& c_courseName, size_t u32_studentID);

    void changePassword(MyString c_oldPassm, MyString c_newPass);
    MyString serialize() const;
    Course* getCourse(const MyString& c_courseName) const;
    void sendMessage(size_t u32_recipientId, MyString c_message);
    void viewMailbox() const;
    void clearMailbox();

    void message_students(const MyString& c_message);
    void enroll_to_course(const MyString& c_course, size_t u32_studentID);
    void grade_assignment(const MyString& c_courseName, const MyString& c_assignmentName, size_t u32_studentID, const MyString& c_grade, const MyString& c_feedback);
};

#endif // TEACHER_H
