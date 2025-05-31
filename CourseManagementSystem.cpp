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
        std::cout << "No users.txt found.\n" << std::endl;
    }

    if (g_users.isEmpty()) {
        std::cout << "No valid users loaded. Starting with default admin.\n" << std::endl;
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
            throw std::invalid_argument("Invalid ID: MyString contains non-numeric characters.\n");
        }
    }
    return result;
}
void CourseManagementSystem::processCommand(const MyString& p_command_line) {
    Vector<MyString> tokens = splitMyString(p_command_line, ' ');
    if (tokens.isEmpty()) {
        return;
    }

    MyString command = MyString::trim(tokens[0]);

    if (command == "logout") {
        g_p_logged_in_user = nullptr;
        std::cout << "Logged out successfully." << std::endl;
        return;
    }

    if (command == "login") {
        if (tokens.getSize() != 3U) {
            std::cout << "Usage: login <id> <password>" << std::endl;
            return;
        }
        if (g_p_logged_in_user != nullptr)
        {
            std::cout << "You are already logged in as :"<< std::endl;
        }
        size_t id = myStringToInt(MyString::trim(tokens[1]));
        MyString password = MyString::trim(tokens[2]);

        User* user = findUserByIdAndPassword(id, password);
        if (user != nullptr) {
            g_p_logged_in_user = user;
            std::cout << "Login successful!" << std::endl;
        }
        else {
            std::cout << "Login failed. Incorrect id or password." << std::endl;
        }
        return;
    }
    if (command == "message_all") {

        if (g_p_logged_in_user == nullptr || g_p_logged_in_user->getRole() != ADMIN) {
            std::cout << "Error: Only an admin can use this command." << std::endl;
            return;
        }

        if (tokens.getSize() < 2U) {
            std::cout << "Usage: message_all <message>" << std::endl;
            return;
        }
    
        MyString message;
        for (size_t i = 1; i < tokens.getSize(); ++i) {
            message = message + tokens[i] + MyString(" ");
        }

        Admin* adminUser = dynamic_cast<Admin*>(g_p_logged_in_user);
        if (adminUser != nullptr) {
            adminUser->message_all(message);
        }
        else {
            std::cout << "Error: The logged in user is not an Admin." << std::endl;
        }
        return;
    }

    if (g_p_logged_in_user == nullptr) {
        std::cout << "Please login first." << std::endl;
        return;
    }

    switch (g_p_logged_in_user->getRole()) {
    case ADMIN:
        handleAdminCommands(tokens);
        break;
    case TEACHER:
        handleTeacherCommands(tokens);
        break;
    case STUDENT:
        handleStudentCommands(tokens);
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
            std::cout << "Added teacher " << p_new_teacher->getFirstName() << " " << p_new_teacher->getLastName() << " with ID " << intToMyString(p_new_teacher->getId()) << std::endl;
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
            std::cout << "Added student " << p_new_student->getFirstName() << " " << p_new_student->getLastName() << " with ID " << intToMyString(p_new_student->getId()) << std::endl;
        }
        else {
            std::cout << "Usage: add_student <firstName> <lastName> <password>" << std::endl;
        }
    }
    else if (c_command == "change_password") {
        if (c_tokens.getSize() == 3U) {
            p_admin->changePassword(c_tokens[1U], c_tokens[2U]);
        }
        else {
            std::cout << "Usage: change_password <oldPassword> <newPassword>" << std::endl;
        }
    }
    else if (c_command == "message") {
        if (c_tokens.getSize() >= 3U) {
            size_t u32_recipient_id = myStringToSizeT(c_tokens[1U]);
            MyString c_message = "";
            for (size_t u32_idx = 2U; u32_idx < c_tokens.getSize(); ++u32_idx) {
                c_message += c_tokens[u32_idx];
                if (u32_idx < c_tokens.getSize() - 1U) {
                    c_message += MyString(" "); 
                }
            }
            p_admin->sendMessage(u32_recipient_id, c_message);
        }
        else {
            std::cout << "Usage: send_message <recipientId> <message>" << std::endl;
        }
    }
    else if (c_command == "view_users") {
        std::cout << "--- All Users ---" << std::endl;
        for (size_t u32_idx = 0U; u32_idx < g_users.getSize(); ++u32_idx) {
            std::cout << "ID: " << intToMyString(g_users[u32_idx]->getId())
                << ", Name: " << g_users[u32_idx]->getFirstName() << " "
                << g_users[u32_idx]->getLastName() << ", Role: ";
            if (g_users[u32_idx]->getRole() == ADMIN)
                std::cout << "Admin";
            else if (g_users[u32_idx]->getRole() == TEACHER)
                std::cout << "Teacher";
            else if (g_users[u32_idx]->getRole() == STUDENT)
                std::cout << "Student";
            std::cout << std::endl;
        }
        std::cout << "-----------------" << std::endl;
    }
    else if (c_command == "clear_mailbox") {
        p_admin->clearMailbox();
        std::cout << "Mailbox cleared." << std::endl;
    }
    else if (c_command == "mailbox") {
        p_admin->viewMailbox();
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
                std::cout << "Course '" << c_course_name.c_str()
                    << "' created by teacher " << p_teacher->getFirstName().c_str() << std::endl;
            }
        }
        else {
            std::cout << "Usage: create_course <courseName> <coursePassword>" << std::endl;
        }
    }
    else if (c_command == "change_password") {
        if (c_tokens.getSize() == 3U) {
            p_teacher->changePassword(c_tokens[1U], c_tokens[2U]);
        }
        else {
            std::cout << "Usage: change_password <oldPassword> <newPassword>" << std::endl;
        }
    }
    else if (c_command == "assign_homework") {
        if (c_tokens.getSize() == 3U) {
            MyString c_course_name = c_tokens[1U];
            MyString c_assignment_name = c_tokens[2U];
            Course* p_course = findCourseByName(c_course_name);
            if (p_course != nullptr)
            {
                p_course->addAssignment(c_assignment_name);
                std::cout << "Homework " << c_assignment_name.c_str() << " assigned for course " << c_course_name.c_str() << std::endl;
            }
            else {
                std::cout << "Course " << c_course_name.c_str() << " not found." << std::endl;
            }
        }
        else {
            std::cout << "Usage: assign_homework <courseName> <assignmentName>" << std::endl;
        }
    }
    else if (c_command == "message") {
        if (c_tokens.getSize() >= 3U) {
            size_t u32_recipient_id = myStringToSizeT(c_tokens[1U]);
            MyString c_message = "";
            for (size_t u32_idx = 2U; u32_idx < c_tokens.getSize(); ++u32_idx) {
                c_message += c_tokens[u32_idx];
                if (u32_idx < c_tokens.getSize() - 1U) {
                    c_message += MyString(" ");
                }
            }
            p_teacher->sendMessage(u32_recipient_id, c_message);
        }
        else {
            std::cout << "Usage: send_message <recipientId> <message>" << std::endl;
        }
    }
    else if (c_command == "add_to_course") {
        if (c_tokens.getSize() == 3U) {
            MyString c_course_name = c_tokens[1U];
            size_t u32_studentID = myStringToSizeT(c_tokens[2U]);
            bool success = p_teacher->addToCourse(c_course_name, u32_studentID);
            if (success) {
                p_teacher->sendMessage(u32_studentID, MyString("You were added to course by ") + p_teacher->getFirstName());
            }
        }
        else {
            std::cout << "Usage: add_to_course <courseName> <studentId>" << std::endl;
        }
    }
    else if (c_command == "message_students") {
        if (c_tokens.getSize() >= 2U) {
            MyString c_message = "";
            for (size_t u32_idx = 1U; u32_idx < c_tokens.getSize(); ++u32_idx) {
                c_message += c_tokens[u32_idx];
                if (u32_idx < c_tokens.getSize() - 1U) {
                    c_message += MyString(" ");
                }
            }
            p_teacher->message_students(c_message);
        }
        else {
            std::cout << "Usage: message_students <message>" << std::endl;
        }
    }
    else if (c_command == "grade_assignment") {
        if (c_tokens.getSize() >= 6U) {
            MyString c_course_name = c_tokens[1U];
            MyString c_assignment_name = c_tokens[2U];
            size_t u32_studentID = myStringToSizeT(c_tokens[3U]);
            MyString c_grade = c_tokens[4U];
            MyString c_feedback = "";
            for (size_t u32_idx = 5U; u32_idx < c_tokens.getSize(); ++u32_idx) {
                c_feedback += c_tokens[u32_idx];
                if (u32_idx < c_tokens.getSize() - 1U) {
                    c_feedback += MyString(" ");
                }
            }
            p_teacher->grade_assignment(c_course_name, c_assignment_name, u32_studentID, c_grade, c_feedback);
        }
        else {
            std::cout << "Usage: grade_assignment <courseName> <assignmentName> <studentId> <grade> <feedback>" << std::endl;
        }
    }
    else if (c_command == "view_assignment_submissions") {
        if (c_tokens.getSize() == 3U) {
            MyString c_course_name = c_tokens[1U];
            MyString c_assignment_name = c_tokens[2U];
            Course* p_course = findCourseByName(c_course_name);
            if (p_course != nullptr) {
                p_course->viewAssignmentSubmissions(c_assignment_name);
            }
            else {
                std::cout << "Course " << c_course_name.c_str() << " not found." << std::endl;
            }
        }
        else {
            std::cout << "Usage: view_assignment_submissions <courseName> <assignmentName>" << std::endl;
        }
    }
    else if (c_command == "clear_mailbox") {
        p_teacher->clearMailbox();
        std::cout << "Mailbox cleared." << std::endl;
    }
    else if (c_command == "mailbox") {
        p_teacher->viewMailbox();
    }
    else {
        std::cout << "Unknown teacher command." << std::endl;
    }
}


void CourseManagementSystem::handleStudentCommands(const Vector<MyString>& c_tokens) {
    MyString c_command = c_tokens[0U];
    Student* p_student = static_cast<Student*>(g_p_logged_in_user);

    if (c_command == "enroll") {
        if (c_tokens.getSize() == 3U) {
            MyString c_course_name = c_tokens[1U];
            MyString c_course_pass = c_tokens[2U];
            Course* p_course = findCourseByName(c_course_name);
            if (p_course != nullptr && c_course_pass == p_course->getPassword()) {
                p_student->enrollInCourse(c_course_name);
                p_course->addStudent(p_student->getId());
                std::cout << "Enrolled in " << c_course_name.c_str() << std::endl;
            }
            else {
                std::cout << "Course " << c_course_name.c_str() << " not found." << std::endl;
            }
        }
        else {
            std::cout << "Usage: enroll_course <courseName> <coursePassword>" << std::endl;
        }
    }
    else if (c_command == "change_password") {
        if (c_tokens.getSize() == 3U) {
            p_student->changePassword(c_tokens[1U], c_tokens[2U]);
        }
        else {
            std::cout << "Usage: change_password <oldPassword> <newPassword>" << std::endl;
        }
    }
    else if (c_command == "submit_assignment") {
        if (c_tokens.getSize() >= 4U) {
            MyString c_course_name = c_tokens[1U];
            MyString c_assignment_name = c_tokens[2U];
            MyString c_submissionMessage = "";
            for (size_t u32_idx = 3U; u32_idx < c_tokens.getSize(); ++u32_idx) {
                c_submissionMessage += c_tokens[u32_idx];
                if (u32_idx < c_tokens.getSize() - 1U) {
                    c_submissionMessage += MyString(" ");
                }
            }
            p_student->submitAssignment(c_course_name, c_assignment_name, c_submissionMessage);
        }
        else {
            std::cout << "Usage: submit_assignment <courseName> <assignmentName> <submissionMessage>" << std::endl;
        }
    }
    else if (c_command == "message") {
        if (c_tokens.getSize() >= 3U) {
            size_t u32_recipient_id = myStringToSizeT(c_tokens[1U]);
            MyString c_message = "";
            for (size_t u32_idx = 2U; u32_idx < c_tokens.getSize(); ++u32_idx) {
                c_message += c_tokens[u32_idx];
                if (u32_idx < c_tokens.getSize() - 1U) {
                    c_message += MyString(" ");
                }
            }
            p_student->sendMessage(u32_recipient_id, c_message);
        }
        else {
            std::cout << "Usage: send_message <recipientId> <message>" << std::endl;
        }
    }
    else if (c_command == "mailbox") {
        p_student->viewMailbox();
    }
    else if (c_command == "grades") {
        p_student->viewGrades();
    }
    else if (c_command == "clear_mailbox") {
        p_student->clearMailbox();
        std::cout << "Mailbox cleared." << std::endl;
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


