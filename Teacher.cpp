#include "Teacher.h"
#include "Course.h"
#include "CourseManagementSystem.hpp"
#include <iostream>

Teacher::Teacher(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password)
    : User(c_firstName, c_lastName, u32_id, c_password, TEACHER) {
}

Teacher::Teacher(const MyString& c_dataLine)
    : User(c_dataLine) {
}

Teacher::~Teacher() {
    for (size_t u32_i = 0U; u32_i < m_taughtCourses.getSize(); ++u32_i) {
        delete m_taughtCourses.data[u32_i].value;
        m_taughtCourses.data[u32_i].value = nullptr;
    }
}

Course* Teacher::createCourse(const MyString& c_courseName, const MyString& c_coursePassword) {
    if (m_taughtCourses.contains(c_courseName)) {
        std::cout << "Course with name " << c_courseName.c_str() << " already exists." << std::endl;
        return nullptr;
    }
    Course* p_new_course = new Course(c_courseName, c_coursePassword);
    m_taughtCourses.insert(c_courseName, p_new_course);
    std::cout << "Course '" << c_courseName.c_str() << "' created successfully." << std::endl;
    return p_new_course;
}

bool Teacher::addToCourse(const MyString& c_courseName, size_t u32_studentID) {
    if (m_taughtCourses.contains(c_courseName)) {
        Course* p_course = m_taughtCourses[c_courseName];
        p_course->addStudent(u32_studentID);
        sendMessage(u32_studentID, MyString("You have been enrolled in course: ") + c_courseName);
        std::cout << "Student " << CourseManagementSystem::intToMyString(u32_studentID).c_str()
            << " enrolled in course " << c_courseName.c_str() << "." << std::endl;
        return true;
    }
    std::cout << "Course " << c_courseName.c_str() << " not found or not taught by this teacher." << std::endl;
    return false;
}

void Teacher::message_students(const MyString& c_message) {
    if (m_taughtCourses.getSize() == 0U) {
        std::cout << "No course available." << std::endl;
        return;
    }
    // For demonstration, we use the first course in the map.
    Course* p_course = m_taughtCourses.data[0].value;
    Vector<size_t> u32_enrolled = p_course->getEnrolledStudentIDs();
    for (size_t u32_i = 0U; u32_i < u32_enrolled.getSize(); ++u32_i) {
        sendMessage(u32_enrolled[u32_i], c_message);
    }
}

void Teacher::enroll_to_course(const MyString& c_course, size_t u32_studentID) {
    if (m_taughtCourses.contains(c_course)) {
        Course* p_course = m_taughtCourses[c_course];
        p_course->addStudent(u32_studentID);
        sendMessage(u32_studentID, MyString("You have been enrolled in course ") + c_course);
        std::cout << "Student " << CourseManagementSystem::intToMyString(u32_studentID).c_str()
            << " enrolled in " << c_course.c_str() << std::endl;
    }
    else {
        std::cout << "Course " << c_course.c_str() << " not found." << std::endl;
    }
}

void Teacher::grade_assignment(const MyString& c_courseName, const MyString& c_assignmentName, size_t u32_studentID, const MyString& c_grade, const MyString& c_feedback) {
    if (m_taughtCourses.contains(c_courseName)) {
        Course* p_course = m_taughtCourses[c_courseName];
        p_course->gradeAssignment(c_assignmentName, u32_studentID, c_grade);
        sendMessage(u32_studentID, c_courseName + MyString(" | ") + c_assignmentName + MyString(" | ") + c_grade + MyString(" | ") + c_feedback);
    }
    else {
        std::cout << "Course " << c_courseName.c_str() << " not found." << std::endl;
    }
}

void Teacher::clearMailbox() {
    inbox.clear();
}

void Teacher::changePassword(MyString c_oldPassm, MyString c_newPass) {
    if (c_oldPassm == getPassword())
        setPassword(c_newPass);
}

MyString Teacher::serialize() const {
    return User::serialize();
}

Course* Teacher::getCourse(const MyString& c_courseName) const {
    if (m_taughtCourses.contains(c_courseName))
        return m_taughtCourses[c_courseName];
    return nullptr;
}

void Teacher::sendMessage(size_t u32_recipientId, MyString c_message) {
    User::sendMessage(u32_recipientId, c_message);
}

void Teacher::viewMailbox() const {
    User::viewMailbox();
}
