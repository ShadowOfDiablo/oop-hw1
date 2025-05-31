#include "Student.h"
#include "CourseManagementSystem.hpp"
#include "Course.h"
#include <iostream>
#include <cstring>

Student::Student(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password)
    : User(c_firstName, c_lastName, u32_id, c_password, STUDENT) {
}

Student::Student(const MyString& c_dataLine)
    : User(c_dataLine) {
}

void Student::enrollInCourse(const MyString& c_courseName) {
    courses.insert(c_courseName, Vector<MyString>());
    std::cout << "Student " << getFirstName().c_str() << " enrolled in course " << c_courseName.c_str() << std::endl;
}

void Student::submitAssignment(const MyString& p_courseName, const MyString& p_assignmentName, const MyString& c_submissionMessage) {
    Course* p_course = CourseManagementSystem::findCourseByName(p_courseName);
    if (p_course != nullptr)
        p_course->submitAssignment(getId(), p_assignmentName, c_submissionMessage);
    else
        std::cout << "Course " << p_courseName.c_str() << " not found." << std::endl;
}

void Student::view_assignment_submissions(const MyString& c_assignmentName) {
    if (courses.isEmpty()) {
        std::cout << "No courses enrolled." << std::endl;
        return;
    }
    bool b_found = false;
    for (size_t u32_i = 0U; u32_i < courses.getSize(); ++u32_i) {
        MyString c_courseName = courses.data[u32_i].key;
        Course* p_course = CourseManagementSystem::findCourseByName(c_courseName);
        if (p_course != nullptr) {
            Map<MyString, Map<size_t, MyString>> c_assignments = p_course->getAssignments();
            if (c_assignments.contains(c_assignmentName)) {
                Map<size_t, MyString>& studentGrades = c_assignments[c_assignmentName];
                if (studentGrades.contains(getId())) {
                    MyString c_submission = studentGrades[getId()];
                    std::cout << "Course: " << c_courseName.c_str() << " Assignment: " << c_assignmentName.c_str() << " Submission: " << c_submission.c_str() << std::endl;
                    b_found = true;
                }
            }
        }
    }
    if (!b_found)
        std::cout << "No submission found for assignment " << c_assignmentName.c_str() << std::endl;
}

void Student::viewGrades() const
{
    std::cout << "--- " << getFirstName().c_str() << "'s Grades ---" << std::endl;
    if (courses.getSize() == 0U) {
        std::cout << "No courses enrolled." << std::endl;
        return;
    }
    bool b_hasGrades = false;
    for (size_t u32_i = 0U; u32_i < courses.getSize(); ++u32_i) {
        MyString c_courseName = courses.data[u32_i].key;
        Course* p_course = CourseManagementSystem::findCourseByName(c_courseName);
        if (p_course != nullptr) {
            Map<MyString, Map<size_t, MyString>> c_assignments = p_course->getAssignments();
            for (size_t u32_j = 0U; u32_j < c_assignments.getSize(); ++u32_j) {
                MyString c_assignmentName = c_assignments.data[u32_j].key;
                Map<size_t, MyString>& grades = c_assignments.data[u32_j].value;
                if (grades.contains(getId())) {
                    MyString c_grade = grades[getId()];
                    std::cout << c_courseName.c_str() << " | " << c_assignmentName.c_str() << " | " << c_grade.c_str() << std::endl;
                    b_hasGrades = true;
                }
            }
        }
    }
    if (!b_hasGrades) {
        std::cout << "No grades recorded yet." << std::endl;
    }
}

void Student::changePassword(MyString c_oldPassm, MyString c_newPass) {
    if (c_oldPassm == getPassword())
        setPassword(c_newPass);
}

MyString Student::serialize() const {
    MyString c_serialized_data = User::serialize();
    return c_serialized_data;
}

void Student::sendMessage(size_t u32_recipientId, const MyString& c_message) {
    User::sendMessage(u32_recipientId, c_message);
}

void Student::viewMailbox() const {
    User::viewMailbox();
}

void Student::clearMailbox() {
    inbox.clear();
}

