// main.cpp
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
//#include <limits>
#include "MyString.h"
#include "User.h"
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"
#include "Vector.hpp"
#include "Map.hpp"
#include "Course.h"

Vector<User*> g_users;
Vector<Course*> g_courses;
size_t g_u32_next_user_id = 100U;

const size_t K_ADMIN_ID = 0U;
const size_t K_INITIAL_USER_ID = 100U;

User* g_p_logged_in_user = nullptr;

void saveUsers() {
    std::ofstream c_user_file("users.txt");
    if (c_user_file.is_open()) {
        for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
            c_user_file << g_users[u32_idx]->serialize().c_str() << std::endl;
        }
        c_user_file.close();
    }
    else {
        std::cout << "Unable to open users.txt for writing." << std::endl;
    }
}

void loadUsers() {
    std::ifstream c_user_file("users.txt");
    if (c_user_file.is_open()) {
        MyString c_line;
        char u8_buffer[256];
        while (c_user_file.getline(u8_buffer, sizeof(u8_buffer))) {
            c_line = MyString(u8_buffer);
            Vector<MyString> c_parts = splitMyString(c_line, ',');
            if (c_parts.getSize() >= 5U) {
                MyString c_role_str = c_parts[4U];
                User* p_user = nullptr;
                if (strcmp(c_role_str.c_str(), "Admin") == 0) {
                    p_user = new Admin(c_line);
                }
                else if (strcmp(c_role_str.c_str(), "Teacher") == 0) {
                    p_user = new Teacher(c_line);
                }
                else if (strcmp(c_role_str.c_str(), "Student") == 0) {
                    p_user = new Student(c_line);
                }
                if (p_user != nullptr) {
                    g_users.push_back(p_user);
                    if (p_user->getId() >= g_u32_next_user_id) {
                        g_u32_next_user_id = p_user->getId() + 1U;
                    }
                }
            }
        }
        c_user_file.close();
    }
    else {
        std::cout << "No users.txt found, starting with default admin." << std::endl;
        Admin* p_admin = new Admin("admin", "admin", K_ADMIN_ID, "0000");
        g_users.push_back(p_admin);
        g_u32_next_user_id = K_INITIAL_USER_ID;
    }
}

void saveCourses() {
    std::ofstream c_course_file("courses.txt");
    if (c_course_file.is_open()) {
        for (size_t u32_idx = 0U; u32_idx < g_courses.getSize(); ++u32_idx) {
            c_course_file << g_courses[u32_idx]->serialize().c_str() << std::endl;
        }
        c_course_file.close();
    }
    else {
        std::cout << "Unable to open courses.txt for writing." << std::endl;
    }
}

void loadCourses() {
    std::ifstream c_course_file("courses.txt");
    if (c_course_file.is_open()) {
        MyString c_line;
        char u8_buffer[256];
        while (c_course_file.getline(u8_buffer, sizeof(u8_buffer))) {
            c_line = MyString(u8_buffer);
            Course* p_course = new Course(c_line);
            g_courses.push_back(p_course);
        }
        c_course_file.close();
    }
    else {
        std::cout << "No courses.txt found." << std::endl;
    }
}


User* findUserById(size_t u32id) {
    User* p_found_user = nullptr;
    for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
        if (g_users[u32_idx]->getId() == u32id) {
            p_found_user = g_users[u32_idx];
            break;
        }
    }
    return p_found_user;
}

User* findUserByIdAndPassword(size_t u32id, const MyString& p_password) {
    User* p_found_user = nullptr;
    for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
        if (g_users[u32_idx]->getId() == u32id && g_users[u32_idx]->getPassword() == p_password) {
            p_found_user = g_users[u32_idx];
            break;
        }
    }
    return p_found_user;
}

Course* findCourseByName(const MyString& p_courseName) {
    Course* p_found_course = nullptr;
    for (size_t u32_idx = 0U; u32_idx < g_courses.getSize(); ++u32_idx) {
        if (g_courses[u32_idx]->getCourseName() == p_courseName) {
            p_found_course = g_courses[u32_idx];
            break;
        }
    }
    return p_found_course;
}
size_t myStringToSizeT(const MyString& str) {
    size_t result = 0;
    for (int i = 0; i < str.length(); ++i) {
        if (isdigit(str[i])) {
            result = result * 10U + static_cast<size_t>(str[i] - '0');
        }
        else {
            throw std::invalid_argument("Invalid ID: MyString contains non-numeric characters.");
        }
    }
    return result;
}

void processCommand(const MyString& p_command_line) {
    Vector<MyString> c_tokens = splitMyString(p_command_line, ' ');
    if (c_tokens.isEmpty()) {
        return;
    }

    MyString c_command = c_tokens[0U];
    if (c_command == "login") {
        if (c_tokens.getSize() == 3U) {
            size_t c_id = myStringToSizeT(c_tokens[1U]);
            MyString c_password = c_tokens[2U];
            g_p_logged_in_user = findUserByIdAndPassword(c_id, c_password);
            if (g_p_logged_in_user != nullptr) {
                std::cout << "Logged in as " << g_p_logged_in_user->getFirstName() << " (" << intToMyString(g_p_logged_in_user->getId()) << ")" << std::endl;
            }
            else {
                std::cout << "Invalid username or password." << std::endl;
            }
        }
        else {
            std::cout << "Usage: login <username> <password>" << std::endl;
        }
    }
    else if (c_command == "logout") {
        if (g_p_logged_in_user != nullptr) {
            std::cout << "Logged out." << std::endl;
            g_p_logged_in_user = nullptr;
        }
        else {
            std::cout << "No user is currently logged in." << std::endl;
        }
    }
    else if (c_command == "change_password" && g_p_logged_in_user != nullptr)
    {
        if (g_p_logged_in_user->getRole() == TEACHER || g_p_logged_in_user->getRole() == STUDENT || g_p_logged_in_user->getRole() == ADMIN)
        {
            g_p_logged_in_user->changePassword(c_tokens[1U], c_tokens[2U]);
        }
    }
    else if (g_p_logged_in_user == nullptr) {
        std::cout << "Please login first." << std::endl;
    }
    else {
        if (g_p_logged_in_user->getRole() == ADMIN) {
            Admin* p_admin = static_cast<Admin*>(g_p_logged_in_user);
            if (c_command == "add_teacher") {
                if (c_tokens.getSize() == 4U) {
                    MyString c_first_name = c_tokens[1U];
                    MyString c_last_name = c_tokens[2U];
                    MyString c_password = c_tokens[3U];
                    Teacher* p_new_teacher = p_admin->addTeacher(c_first_name, c_last_name, g_u32_next_user_id++, c_password);
                    g_users.push_back(p_new_teacher);
                    std::cout << "Teacher " << p_new_teacher->getFirstName() << " added with ID " << intToMyString(p_new_teacher->getId()) << std::endl;
                }
                else {
                    std::cout << "Usage: add_teacher <firstName> <lastName> <password>" << std::endl;
                }
            }
            else if (c_command == "add_student") {
                if (c_tokens.getSize() == 4U) {
                    MyString c_first_name = c_tokens[1U];
                    MyString c_last_name = c_tokens[2U];
                    MyString c_password = c_tokens[3U];
                    Student* p_new_student = p_admin->addStudent(c_first_name, c_last_name, g_u32_next_user_id++, c_password);
                    g_users.push_back(p_new_student);
                    std::cout << "Student " << p_new_student->getFirstName() << " added with ID " << intToMyString(p_new_student->getId()) << std::endl;
                }
                else {
                    std::cout << "Usage: add_student <firstName> <lastName> <password>" << std::endl;
                }
            }
            else if (c_command == "view_users") {
                std::cout << "--- All Users ---" << std::endl;
                for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
                    std::cout << "ID: " << intToMyString(g_users[u32_idx]->getId())
                        << ", Name: " << g_users[u32_idx]->getFirstName() << " " << g_users[u32_idx]->getLastName()
                        << ", Role: ";
                    if (g_users[u32_idx]->getRole() == ADMIN) {
                        std::cout << "Admin";
                    }
                    else if (g_users[u32_idx]->getRole() == TEACHER) {
                        std::cout << "Teacher";
                    }
                    else if (g_users[u32_idx]->getRole() == STUDENT) {
                        std::cout << "Student";
                    }
                    std::cout << std::endl;
                }
                std::cout << "-----------------" << std::endl;
            }
            else {
                std::cout << "Unknown admin command." << std::endl;
            }
        }
        else if (g_p_logged_in_user->getRole() == TEACHER) {
            Teacher* p_teacher = static_cast<Teacher*>(g_p_logged_in_user);
            if (c_command == "create_course") {
                if (c_tokens.getSize() == 3U) {
                    MyString c_course_name = c_tokens[1U];
                    MyString c_course_password = c_tokens[2U];
                    Course* p_new_course = p_teacher->createCourse(c_course_name, c_course_password);
                    if (p_new_course != nullptr) {
                        g_courses.push_back(p_new_course);
                        std::cout << "Course '" << c_course_name << "' created by teacher " << p_teacher->getFirstName() << std::endl;
                    }
                }
                else {
                    std::cout << "Usage: create_course <courseName> <coursePassword>" << std::endl;
                }
            }
            else if (c_command == "add_to_course") {
                if (c_tokens.getSize() == 3U) {
                    MyString c_course_name = c_tokens[1U];
                    size_t u32_student_id = myStringToInt(c_tokens[2U]);
                    User* p_student_user = findUserById(u32_student_id);
                    if (p_student_user != nullptr && p_student_user->getRole() == STUDENT) {
                        if (p_teacher->addToCourse(c_course_name, u32_student_id)) {
                            static_cast<Student*>(p_student_user)->enrollInCourse(c_course_name);
                        }
                    }
                    else {
                        std::cout << "Student with ID " << u32_student_id << " not found or is not a student." << std::endl;
                    }
                }
                else {
                    std::cout << "Usage: add_to_course <courseName> <studentId>" << std::endl;
                }
            }
            else if (c_command == "view_courses") {
                std::cout << "--- Courses Taught by " << p_teacher->getFirstName() << " ---" << std::endl;
                bool b_found_any_course = false;
                for (size_t u32_idx = 0U; u32_idx < g_courses.getSize(); ++u32_idx) {
                    Course* p_current_course = g_courses[u32_idx];
                    Course* p_course_from_teacher = p_teacher->getCourse(p_current_course->getCourseName());
                    if (p_course_from_teacher != nullptr) {
                        std::cout << "Course: " << p_current_course->getCourseName() << ", Password: " << p_current_course->getPassword() << std::endl;
                        std::cout << "  Enrolled Students IDs: {";
                        Vector<size_t> c_student_ids = p_current_course->getEnrolledStudentIDs();
                        for (size_t u32_j = 0U; u32_j < c_student_ids.getSize(); ++u32_j) {
                            std::cout << intToMyString(c_student_ids[u32_j]);
                            if (u32_j < (c_student_ids.getSize() - 1U)) {
                                std::cout << ", ";
                            }
                        }
                        std::cout << "}" << std::endl;
                        b_found_any_course = true;
                    }
                }
                if (!b_found_any_course) {
                    std::cout << "No courses taught by this teacher." << std::endl;
                }
                std::cout << "-----------------------------" << std::endl;
            }
            else {
                std::cout << "Unknown teacher command." << std::endl;
            }
        }
        else if (g_p_logged_in_user->getRole() == STUDENT) {
            Student* p_student = static_cast<Student*>(g_p_logged_in_user);
            if (c_command == "enroll_course") {
                if (c_tokens.getSize() == 2U) {
                    MyString c_course_name = c_tokens[1U];
                    Course* p_course = findCourseByName(c_course_name);
                    if (p_course != nullptr) {
                        p_student->enrollInCourse(c_course_name);
                        p_course->addStudent(p_student->getId());
                    }
                    else {
                        std::cout << "Course " << c_course_name << " not found." << std::endl;
                    }
                }
                else {
                    std::cout << "Usage: enroll_course <courseName>" << std::endl;
                }
            }
            else if (c_command == "submit_assignment") {
                if (c_tokens.getSize() == 3U) {
                    MyString c_course_name = c_tokens[1U];
                    MyString c_assignment_name = c_tokens[2U];
                    p_student->submitAssignment(c_course_name, c_assignment_name);
                }
                else {
                    std::cout << "Usage: submit_assignment <courseName> <assignmentName>" << std::endl;
                }
            }
            else if (c_command == "view_grades") {
                p_student->viewGrades();
            }
            else {
                std::cout << "Unknown student command." << std::endl;
            }
        }
    }
}

int main() {
    loadUsers();
    loadCourses();

    MyString c_command_line;
    char u8_buffer[256];

    while (true) {
        std::cout << "> ";
        std::cin.getline(u8_buffer, sizeof(u8_buffer));
        c_command_line = MyString(u8_buffer);

        if (c_command_line == "exit") {
            break;
        }

        processCommand(c_command_line);
    }

    saveUsers();
    saveCourses();

    for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
        delete g_users[u32_idx];
        g_users[u32_idx] = nullptr;
    }
    for (size_t u32_idx = 0U; u32_idx < g_courses.getSize(); ++u32_idx) {
        delete g_courses[u32_idx];
        g_courses[u32_idx] = nullptr;
    }

    return 0;
}