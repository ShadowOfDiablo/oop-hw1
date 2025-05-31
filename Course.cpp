#define _CRT_SECURE_NO_WARNINGS
#include "Course.h"
#include "CourseManagementSystem.hpp"
#include <iostream>
#include <cstring>
#include <ctime>

Course::Course(const MyString& c_courseName, const MyString& c_password)
    : c_courseName(c_courseName), c_password(c_password) {
}

Course::Course(const MyString& c_dataLine) {
    Vector<MyString> c_parts = CourseManagementSystem::splitMyString(c_dataLine, ',');
    if (c_parts.getSize() >= 3U) {
        c_courseName = c_parts[0U];
        c_password = c_parts[1U];
        MyString c_idsStr = c_parts[2U];
        if (c_idsStr.length() > 2) {
            char* p_ids_cstr = new char[static_cast<size_t>(c_idsStr.length()) + 1U];
            strcpy(p_ids_cstr, c_idsStr.c_str());
            char* p_startBrace = strchr(p_ids_cstr, '{');
            if (p_startBrace != nullptr) {
                p_startBrace++;
                char* p_endBrace = strchr(p_startBrace, '}');
                if (p_endBrace != nullptr) {
                    *p_endBrace = '\0';
                }
                char* p_token = strtok(p_startBrace, ",");
                while (p_token != nullptr) {
                    if (strlen(p_token) > 0U) {
                        u32_enrolledStudentIDs.push_back(CourseManagementSystem::myStringToInt(MyString(p_token)));
                    }
                    p_token = strtok(nullptr, ",");
                }
            }
            delete[] p_ids_cstr;
        }
    }
}

MyString Course::getCourseName() const {
    return c_courseName;
}

MyString Course::getPassword() const {
    return c_password;
}

void Course::addStudent(size_t u32_studentID) {
    bool b_found = false;
    for (size_t u32_i = 0U; u32_i < u32_enrolledStudentIDs.getSize(); ++u32_i) {
        if (u32_enrolledStudentIDs[u32_i] == u32_studentID) {
            b_found = true;
            break;
        }
    }
    if (!b_found) {
        u32_enrolledStudentIDs.push_back(u32_studentID);
        for (size_t u32_i = 0U; u32_i < c_assignments.getSize(); ++u32_i) {
            c_assignments.data[u32_i].value.insert(u32_studentID, MyString("Pending"));
        }
    }
}

Vector<size_t> Course::getEnrolledStudentIDs() const {
    return u32_enrolledStudentIDs;
}

void Course::addAssignment(const MyString& c_assignmentName) {
    if (c_assignments.contains(c_assignmentName)) {
        std::cout << "Assignment " << c_assignmentName.c_str() << " already exists in " << c_courseName.c_str() << std::endl;
        return;
    }
    Map<size_t, MyString> studentGrades;
    for (size_t u32_i = 0U; u32_i < u32_enrolledStudentIDs.getSize(); ++u32_i) {
        studentGrades.insert(u32_enrolledStudentIDs[u32_i], MyString("Pending"));
    }
    c_assignments.insert(c_assignmentName, studentGrades);
    std::cout << "Assignment " << c_assignmentName.c_str() << " added to course " << c_courseName.c_str() << std::endl;
}


void Course::submitAssignment(size_t u32_studentID, const MyString& c_assignmentName, const MyString& c_submissionMessage)
{
    if (!c_assignments.contains(c_assignmentName)) {
        std::cout << "Assignment " << c_assignmentName.c_str() << " does not exist in " << c_courseName.c_str() << std::endl;
        return;
    }
    Map<size_t, MyString>& studentGrades = c_assignments[c_assignmentName];
    if (!studentGrades.contains(u32_studentID)) {
        std::cout << "Student with ID " << CourseManagementSystem::intToMyString(u32_studentID).c_str()
            << " is not registered for this assignment." << std::endl;
        return;
    }
    studentGrades[u32_studentID] = MyString("Submitted");
    std::cout << "Student " << CourseManagementSystem::intToMyString(u32_studentID).c_str()
        << " submitted assignment " << c_assignmentName.c_str()
        << " for course " << c_courseName.c_str() << std::endl;
}

void Course::gradeAssignment(const MyString& c_assignmentName, size_t u32_studentID, const MyString& c_grade) {
    if (!c_assignments.contains(c_assignmentName)) {
        std::cout << "Assignment " << c_assignmentName.c_str() << " does not exist in " << c_courseName.c_str() << std::endl;
        return;
    }
    Map<size_t, MyString>& studentGrades = c_assignments[c_assignmentName];
    if (!studentGrades.contains(u32_studentID)) {
        std::cout << "Student with ID " << CourseManagementSystem::intToMyString(u32_studentID).c_str()
            << " is not registered for this assignment." << std::endl;
        return;
    }
    studentGrades[u32_studentID] = c_grade;
    std::cout << "Assignment " << c_assignmentName.c_str() << " for student "
        << CourseManagementSystem::intToMyString(u32_studentID).c_str()
        << " graded as " << c_grade.c_str() << std::endl;
}

Map<MyString, Map<size_t, MyString>> Course::getAssignments() const {
    return c_assignments;
}

MyString Course::serialize() const {
    MyString ser = c_courseName + MyString(",") + c_password + MyString(",");
    ser += "{";
    for (size_t u32_i = 0U; u32_i < u32_enrolledStudentIDs.getSize(); ++u32_i) {
        ser += CourseManagementSystem::intToMyString(u32_enrolledStudentIDs[u32_i]);
        if (u32_i < u32_enrolledStudentIDs.getSize() - 1U) {
            ser += ",";
        }
    }
    ser += "}";
    ser += "[";
    for (size_t u32_i = 0U; u32_i < c_assignments.getSize(); ++u32_i) {
        ser += c_assignments.data[u32_i].key;
        ser += ":{";
        Map<size_t, MyString> grades = c_assignments.data[u32_i].value;
        for (size_t u32_j = 0U; u32_j < grades.getSize(); ++u32_j) {
            ser += CourseManagementSystem::intToMyString(grades.data[u32_j].key);
            ser += ":";
            ser += grades.data[u32_j].value;
            if (u32_j < grades.getSize() - 1U) {
                ser += ",";
            }
        }
        ser += "}";
        if (u32_i < c_assignments.getSize() - 1U) {
            ser += ",";
        }
    }
    ser += "]";
    return ser;
}
void Course::viewAssignmentSubmissions(const MyString& c_assignmentName) const {
    if (!c_assignments.contains(c_assignmentName)) {
        std::cout << "Assignment " << c_assignmentName.c_str() << " does not exist in course " << c_courseName.c_str() << std::endl;
        return;
    }
    const Map<size_t, MyString>& submissions = c_assignments[c_assignmentName];
    std::cout << "Submissions for " << c_assignmentName.c_str() << " in " << c_courseName.c_str() << ":" << std::endl;
    for (size_t u32_i = 0U; u32_i < submissions.getSize(); ++u32_i) {
        std::cout << "Student " << CourseManagementSystem::intToMyString(submissions.data[u32_i].key).c_str()
            << " submitted: " << submissions.data[u32_i].value.c_str() << std::endl;
    }
}

