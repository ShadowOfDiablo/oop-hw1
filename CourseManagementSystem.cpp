#define _CRT_SECURE_NO_WARNINGS

#include "CourseManagementSystem.hpp"
#include <iostream>
#include <fstream>
#include <ctime>

#include "MyString.h"
#include "User.h"
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"
#include "Vector.hpp"
#include "Map.hpp"
#include "Course.h"

Vector<User*> CourseManagementSystem::g_users;
Vector<Course*> CourseManagementSystem::g_courses;
User* CourseManagementSystem::g_p_logged_in_user = nullptr;

size_t g_u32_next_user_id = 100U;

const size_t K_ADMIN_ID = 0U;
const size_t K_INITIAL_USER_ID = 100U;

CourseManagementSystem::CourseManagementSystem() {
    loadData();
}

CourseManagementSystem::~CourseManagementSystem()
{
	saveData();
	for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
		delete g_users[u32_idx];
		g_users[u32_idx] = nullptr;
	}
	g_users.clear();
	for (size_t u32_idx = 0U; u32_idx < g_courses.getSize(); ++u32_idx) {
		delete g_courses[u32_idx];
		g_courses[u32_idx] = nullptr;
	}
	g_courses.clear();
}

void CourseManagementSystem::loadData() {
    loadUsers();
    loadCourses();
}

void CourseManagementSystem::saveData() {
    saveUsers();
    saveCourses();
}

void CourseManagementSystem::saveUsers() {
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

void CourseManagementSystem::loadUsers() {
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
        std::cout << "No users.txt found." << std::endl;
    }

    if (g_users.isEmpty()) {
        std::cout << "No valid users loaded. Starting with default admin." << std::endl;
        Admin* p_admin = new Admin("admin", "admin", K_ADMIN_ID, "0000");
        g_users.push_back(p_admin);
        g_u32_next_user_id = K_INITIAL_USER_ID;
    }
}


void CourseManagementSystem::saveCourses() {
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

void CourseManagementSystem::loadCourses() {
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

User* CourseManagementSystem::findUserById(size_t u32id) {
    for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
        if (g_users[u32_idx]->getId() == u32id) {
            return g_users[u32_idx];
        }
    }
    return nullptr;
}

User* CourseManagementSystem::findUserByIdAndPassword(size_t u32id, const MyString& p_password) {
    for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
        if (g_users[u32_idx]->getId() == u32id && g_users[u32_idx]->getPassword() == p_password) {
            return g_users[u32_idx];
        }
    }
    return nullptr;
}

Course* CourseManagementSystem::findCourseByName(const MyString& p_courseName) {
    for (size_t u32_idx = 0U; u32_idx < g_courses.getSize(); ++u32_idx) {
        if (g_courses[u32_idx]->getCourseName() == p_courseName) {
            return g_courses[u32_idx];
        }
    }
    return nullptr;
}

size_t CourseManagementSystem::myStringToSizeT(const MyString& str) {
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
void CourseManagementSystem::processCommand(const MyString& p_command_line) {
    Vector<MyString> c_tokens = splitMyString(p_command_line, ' ');
    if (c_tokens.isEmpty())
    {
        return;
    }

    MyString command = MyString::trim(c_tokens[0]);

    std::cout << command;
    if (command == "login") {
        if (c_tokens.getSize() != 3U) {
            std::cout << "Usage: login <id> <password>" << std::endl;
            return;
        }
        size_t id = myStringToInt(MyString::trim(c_tokens[1]));
        MyString password = MyString::trim(c_tokens[2]);

        User* user = findUserByIdAndPassword(id, password);
        if (user != nullptr) {
            g_p_logged_in_user = user;
            std::cout << "Login successful! Welcome, "
                << user->getFirstName() << "!" << std::endl;
        }
        else {
            std::cout << "Login failed. Incorrect id or password." << std::endl;
        }
        return;
    }

    if (g_p_logged_in_user == nullptr) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    switch (g_p_logged_in_user->getRole()) {
    case ADMIN:
        handleAdminCommands(c_tokens);
        break;
    case TEACHER:
        handleTeacherCommands(c_tokens);
        break;
    case STUDENT:
        handleStudentCommands(c_tokens);
        break;
    default:
        std::cout << "Invalid role." << std::endl;
        break;
    }
}



void CourseManagementSystem::handleAdminCommands(const Vector<MyString>& c_tokens) {
    MyString c_command = c_tokens[0U];
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
            if (g_users[u32_idx]->getRole() == ADMIN) std::cout << "Admin";
            else if (g_users[u32_idx]->getRole() == TEACHER) std::cout << "Teacher";
            else if (g_users[u32_idx]->getRole() == STUDENT) std::cout << "Student";
            std::cout << std::endl;
        }
        std::cout << "-----------------" << std::endl;
    }
    else {
        std::cout << "Unknown admin command." << std::endl;
    }
}
void CourseManagementSystem::handleTeacherCommands(const Vector<MyString>& c_tokens) {
    MyString c_command = c_tokens[0U];
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
    else if (c_command == "assign_homework") {
        if (c_tokens.getSize() == 3U) {
            MyString c_course_name = c_tokens[1U];
            MyString c_homework_str = c_tokens[2U];

            Course* p_course = findCourseByName(c_course_name);
            if (p_course != nullptr) {
                Map<MyString, MyString> c_homework;
                Vector<MyString> c_assignments = splitMyString(c_homework_str, ',');

                for (size_t u32_idx = 0U; u32_idx < c_assignments.getSize(); ++u32_idx) {
                    Vector<MyString> c_task_grade = splitMyString(c_assignments[u32_idx], ':');
                    if (c_task_grade.getSize() == 2U) {
                        size_t u32_grade = myStringToSizeT(c_task_grade[1U]);
                        MyString grade_str = (u32_grade == 0U) ? MyString("No grade") : intToMyString(u32_grade);
                        c_homework.insert(c_task_grade[0U], grade_str);
                    }
                }

                std::cout << "Homework assigned for course " << c_course_name << std::endl;
            }
            else {
                std::cout << "Course " << c_course_name << " not found." << std::endl;
            }
        }
        else {
            std::cout << "Usage: assign_homework <courseName> <Task1:grade,Task2:grade,...>" << std::endl;
        }
    }
    else {
        std::cout << "Unknown teacher command." << std::endl;
    }
}

void CourseManagementSystem::handleStudentCommands(const Vector<MyString>& c_tokens) {
    MyString c_command = c_tokens[0U];
    Student* p_student = static_cast<Student*>(g_p_logged_in_user);

    if (c_command == "enroll_course") {
        if (c_tokens.getSize() == 2U) {
            MyString c_course_name = c_tokens[1U];
            Course* p_course = findCourseByName(c_course_name);
            if (p_course != nullptr) {
                p_student->enrollInCourse(c_course_name);
                p_course->addStudent(p_student->getId());
                std::cout << "Enrolled in " << c_course_name << std::endl;
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

void CourseManagementSystem::run() {
    std::cout << "Welcome to Course Management System!" << std::endl;
    while (true) {
        char u8_buffer[256];
        std::cout << "> ";
        if (!std::cin.getline(u8_buffer, sizeof(u8_buffer))) {
            std::cout << "\nExiting the system. Goodbye!" << std::endl;
            break;
        }
        MyString commandLine(u8_buffer);
        if (commandLine == "exit" || commandLine == "quit") {
            std::cout << "Exiting the system. Goodbye!" << std::endl;
            break;
        }

        processCommand(commandLine);
        saveData();
    }
}


