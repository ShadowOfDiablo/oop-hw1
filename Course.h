// Course.h
#ifndef COURSE_H
#define COURSE_H

#include "MyString.h"
#include "Vector.hpp"

class Course {
private:
    MyString m_courseName;
    MyString m_password;
    Vector<size_t> m_enrolledStudentIDs;

public:
    Course(const MyString& p_courseName, const MyString& p_password);
    Course(const MyString& p_dataLine);

    MyString getCourseName() const;
    MyString getPassword() const;
    void addStudent(size_t u32studentID);
    Vector<size_t> getEnrolledStudentIDs() const;

    MyString serialize() const;
};

#endif // COURSE_H