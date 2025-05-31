#ifndef COURSE_H
#define COURSE_H

#include "MyString.h"
#include "Vector.hpp"
#include "Map.hpp"

class Course {
private:
    MyString c_courseName;
    MyString c_password;
    Vector<size_t> u32_enrolledStudentIDs;
    Map<MyString, Map<size_t, MyString>> c_assignments;
public:
    Course(const MyString& c_courseName, const MyString& c_password);
    Course(const MyString& c_dataLine);
    MyString getCourseName() const;
    MyString getPassword() const;
    Vector<size_t> getEnrolledStudentIDs() const;
    void addStudent(size_t u32_studentID);
    void addAssignment(const MyString& c_assignmentName);
    void submitAssignment(size_t u32_studentID, const MyString& c_assignmentName, const MyString& c_submissionMessage);
    void gradeAssignment(const MyString& c_assignmentName, size_t u32_studentID, const MyString& c_grade);
    Map<MyString, Map<size_t, MyString>> getAssignments() const;
    MyString serialize() const;
    void viewAssignmentSubmissions(const MyString& c_assignmentName) const;
};

#endif // COURSE_H
