#ifndef COURSE_MANAGEMENT_SYSTEM_H
#define COURSE_MANAGEMENT_SYSTEM_H

#include <iostream>
#include "MyString.h"
#include "User.h"
#include "Course.h"
#include "Vector.hpp"

class User;
class Course;

inline bool myIsDigit(char c) {
    return (c >= '0' && c <= '9');
}

class CourseManagementSystem
{
private:
    static Vector<User*> g_users;
    static Vector<Course*> g_courses;
    static User* g_p_logged_in_user;
public:
    CourseManagementSystem();
    virtual ~CourseManagementSystem();
    void loadData();
    void saveData();
    void saveUsers();
    void loadUsers();
    void saveCourses();
    void loadCourses();

    static User* findUserById(size_t u32id);

    User* findUserByIdAndPassword(size_t u32id, const MyString& p_password);
    Course* findCourseByName(const MyString& p_courseName);

    static size_t myStringToSizeT(const MyString& str);
    void processCommand(const MyString& commandLine);
    void handleAdminCommands(const Vector<MyString>& c_tokens);
    void handleTeacherCommands(const Vector<MyString>& c_tokens);
    void handleStudentCommands(const Vector<MyString>& c_tokens);
    void run();

    static size_t myStringToInt(const MyString& p_myString) {
        size_t result = 0;
        for (size_t i = 0U; i < p_myString.length(); ++i) {
            if (myIsDigit(p_myString[i])) {
                result = result * 10U + static_cast<size_t>(p_myString[i] - '0');
            }
            else {
                throw std::invalid_argument("Invalid number format.");
            }
        }
        return result;
    }

    static MyString intToMyString(size_t u32value) {
        char u8_buffer[20];
        size_t u32_idx = 0U;
        do {
            u8_buffer[u32_idx++] = static_cast<char>('0' + (u32value % 10U));
            u32value /= 10U;
        } while (u32value > 0U);
        u8_buffer[u32_idx] = '\0';
        // Reverse the string.
        for (size_t i = 0U; i < u32_idx / 2U; ++i) {
            char temp = u8_buffer[i];
            u8_buffer[i] = u8_buffer[u32_idx - 1U - i];
            u8_buffer[u32_idx - 1U - i] = temp;
        }
        return MyString(u8_buffer);
    }
    static Vector<MyString> splitMyString(const MyString& p_myString, char u8_delimiter) {
        Vector<MyString> c_parts;
        size_t u32_start = 0U, u32_end = 0U;
        while ((u32_end = p_myString.find(u8_delimiter, u32_start)) != MyString::npos) {
            c_parts.push_back(p_myString.substr(u32_start, u32_end - u32_start));
            u32_start = u32_end + 1U;
        }
        c_parts.push_back(p_myString.substr(u32_start));
        return c_parts;
    }
};

#endif // COURSE_MANAGEMENT_SYSTEM_H
