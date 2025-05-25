#include "Teacher.h"
#include "Course.h"
#include <cstring>
#include <iostream>

Teacher::Teacher(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password)
    : User(p_firstName, p_lastName, u32id, p_password, TEACHER) {
}

Teacher::Teacher(const MyString& p_dataLine)
    : User(p_dataLine) {
}

Teacher::~Teacher() {
    for (size_t u32_idx = 0U; u32_idx < m_taughtCourses.getSize(); ++u32_idx) {
        delete m_taughtCourses.data[u32_idx].value;
        m_taughtCourses.data[u32_idx].value = nullptr;
    }
}

Course* Teacher::createCourse(const MyString& p_courseName, const MyString& p_coursePassword) {
    if (m_taughtCourses.contains(p_courseName)) {
        std::cout << "Course with name " << p_courseName << " already exists." << std::endl;
        return nullptr;
    }
    Course* p_new_course = new Course(p_courseName, p_coursePassword);
    m_taughtCourses.insert(p_courseName, p_new_course);
    std::cout << "Course '" << p_courseName << "' created successfully." << std::endl;
    return p_new_course;
}

bool Teacher::addToCourse(const MyString& p_courseName, size_t u32studentID) {
    if (m_taughtCourses.contains(p_courseName)) {
        Course* p_course = m_taughtCourses[p_courseName];
        p_course->addStudent(u32studentID);
        std::cout << "Student " << u32studentID << " added to course " << p_courseName << "." << std::endl;
        return true;
    }
    std::cout << "Course " << p_courseName << " not found or not taught by this teacher." << std::endl;
    return false;
}

MyString Teacher::serialize() const {
    MyString serialized_data = User::serialize();
    return serialized_data;
}

Course* Teacher::getCourse(const MyString& p_courseName) const {
    if (m_taughtCourses.contains(p_courseName)) {
        return m_taughtCourses[p_courseName];
    }
    return nullptr;
}