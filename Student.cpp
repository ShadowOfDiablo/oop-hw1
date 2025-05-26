#include "Student.h"
#include <cstring>
#include <iostream>

Student::Student(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password)
    : User(p_firstName, p_lastName, u32id, p_password, STUDENT) {
}

Student::Student(const MyString& p_dataLine)
    : User(p_dataLine) {
}

void Student::enrollInCourse(const MyString& p_courseName) {
    courses.insert(p_courseName, Vector<MyString>());
    std::cout << "Student " << getFirstName() << " enrolled in course " << p_courseName << std::endl;
}

void Student::submitAssignment(const MyString& p_courseName, const MyString& p_assignmentName) {
    if (courses.contains(p_courseName)) {
        courses[p_courseName].push_back(p_assignmentName + MyString(":Pending"));
        std::cout << "Assignment " << p_assignmentName << " submitted for course " << p_courseName << std::endl;
    }
    else {
        std::cout << "Not enrolled in course " << p_courseName << std::endl;
    }
}

void Student::viewGrades() const {
    std::cout << "--- " << getFirstName().c_str() << "'s Grades ---" << std::endl;
    if (courses.getSize() == 0U) {
        std::cout << "No courses enrolled." << std::endl;
        return;
    }
    for (size_t u32_idx = 0U; u32_idx < courses.getSize(); ++u32_idx) {
        std::cout << "Course: " << courses.data[u32_idx].key.c_str() << std::endl;
        if (courses.data[u32_idx].value.isEmpty()) {
            std::cout << "  No assignments/grades yet." << std::endl;
        }
        else {
            for (size_t u32_j = 0U; u32_j < courses.data[u32_idx].value.getSize(); ++u32_j) {
                std::cout << "    - " << courses.data[u32_idx].value[u32_j].c_str() << std::endl;
            }
        }
    }
    std::cout << "-----------------------" << std::endl;
}

void Student::changePassword(MyString oldPassm, MyString newPass)
{
    if (oldPassm == getPassword())
    {
        setPassword(newPass);
    }
}

MyString Student::serialize() const {
    MyString serialized_data = User::serialize();
    return serialized_data;
}

void Student::sendMessage(size_t u32recipientId, const MyString& c_message) {
    User::sendMessage(u32recipientId,c_message);
}

void Student::viewMailbox() const {
    User::viewMailbox();
}
