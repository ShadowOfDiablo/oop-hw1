#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include "MyString.h"
#include "Vector.hpp"
#include "Map.hpp"

class Course;

class Teacher : public User {
private:
    Map<MyString, Course*> m_taughtCourses;

public:
    Teacher(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password);
    Teacher(const MyString& p_dataLine);
    ~Teacher() override;

    Course* createCourse(const MyString& p_courseName, const MyString& p_coursePassword);

    bool addToCourse(const MyString& p_courseName, size_t u32studentID);

    void changePassword(MyString oldPassm, MyString newPass);

    MyString serialize() const;

    Course* getCourse(const MyString& p_courseName) const;

    void sendMessage(size_t u32recipientId, MyString message);

    void viewMailbox() const;


};

#endif // TEACHER_H