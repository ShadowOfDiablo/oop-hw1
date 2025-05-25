// Course.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "Course.h"
#include "User.h"
#include <iostream>
#include <cstring>

Course::Course(const MyString& p_courseName, const MyString& p_password)
    : m_courseName(p_courseName), m_password(p_password) {
}

Course::Course(const MyString& p_dataLine) {
    Vector<MyString> c_parts = splitMyString(p_dataLine, ',');
    if (c_parts.getSize() >= 3U) {
        m_courseName = c_parts[0U];
        m_password = c_parts[1U];

        MyString c_student_ids_str = c_parts[2U];
        if (c_student_ids_str.length() > 2) {
            char* p_char_ids_c_str = new char[static_cast<size_t>(c_student_ids_str.length()) + 1U];
            (void)strcpy(p_char_ids_c_str, c_student_ids_str.c_str());

            char* p_char_start_brace = strchr(p_char_ids_c_str, '{');
            if (p_char_start_brace != nullptr) {
                p_char_start_brace++;
                char* p_char_end_brace = strrchr(p_char_start_brace, '}');
                if (p_char_end_brace != nullptr) {
                    *p_char_end_brace = '\0';
                }

                char* p_char_token = strtok(p_char_start_brace, ",");
                while (p_char_token != nullptr) {
                    if (strlen(p_char_token) > 0U) {
                        m_enrolledStudentIDs.push_back(myStringToInt(MyString(p_char_token)));
                    }
                    p_char_token = strtok(nullptr, ",");
                }
            }
            delete[] p_char_ids_c_str;
            p_char_ids_c_str = nullptr;
        }
    }
}

MyString Course::getCourseName() const { return m_courseName; }
MyString Course::getPassword() const { return m_password; }

void Course::addStudent(size_t u32studentID) {
    bool b_found = false;
    for (size_t u32_idx = 0U; u32_idx < m_enrolledStudentIDs.getSize(); ++u32_idx) {
        if (m_enrolledStudentIDs[u32_idx] == u32studentID) {
            b_found = true;
            break;
        }
    }
    if (!b_found) {
        m_enrolledStudentIDs.push_back(u32studentID);
    }
}

Vector<size_t> Course::getEnrolledStudentIDs() const {
    return m_enrolledStudentIDs;
}

MyString Course::serialize() const {
    MyString c_serialized_data = m_courseName;
    c_serialized_data += ",";
    c_serialized_data += m_password;
    c_serialized_data += ",{";
    for (size_t u32_idx = 0U; u32_idx < m_enrolledStudentIDs.getSize(); ++u32_idx) {
        c_serialized_data += intToMyString(m_enrolledStudentIDs[u32_idx]);
        if (u32_idx < (m_enrolledStudentIDs.getSize() - 1U)) {
            c_serialized_data += ",";
        }
    }
    c_serialized_data += "};";
    return c_serialized_data;
}