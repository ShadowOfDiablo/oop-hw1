// User.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "User.h"
#include <iostream>
#include <fstream>
#include <cstring>

size_t myStringToInt(const MyString& p_myString) {
    size_t u32_res = 0U;
    int u32_idx = 0;

    if (p_myString.length() == 0) {
        return 0U;
    }

    if (p_myString.c_str()[0U] == '-') {
        return 0U;
    }

    for (; u32_idx < p_myString.length(); ++u32_idx) {
        char u8_digit_char = p_myString.c_str()[static_cast<size_t>(u32_idx)];
        if ((u8_digit_char >= '0') && (u8_digit_char <= '9')) {
            u32_res = (u32_res * 10U) + static_cast<size_t>(u8_digit_char - '0');
        }
        else {
            return 0U;
        }
    }
    return u32_res;
}

MyString intToMyString(size_t u32value) {
    if (u32value == 0U) {
        return MyString("0");
    }

    char u8_buffer[32];
    size_t u32_idx = 0U;

    size_t u32_temp_value = u32value;
    while (u32_temp_value > 0U) {
        u8_buffer[u32_idx++] = static_cast<char>((u32_temp_value % 10U) + '0');
        u32_temp_value /= 10U;
    }

    u8_buffer[u32_idx] = '\0';

    for (size_t u32_j = 0U; u32_j < (u32_idx / 2U); ++u32_j) {
        char u8_temp = u8_buffer[u32_j];
        u8_buffer[u32_j] = u8_buffer[u32_idx - 1U - u32_j];
        u8_buffer[u32_idx - 1U - u32_j] = u8_temp;
    }

    return MyString(u8_buffer);
}

Vector<MyString> splitMyString(const MyString& p_myString, char u8_delimiter) {
    Vector<MyString> c_tokens;
    char* p_char_c_str_copy = new char[static_cast<size_t>(p_myString.length()) + 1U];
    (void)strcpy(p_char_c_str_copy, p_myString.c_str());

    char u8_delimiter_str[2];
    u8_delimiter_str[0] = u8_delimiter;
    u8_delimiter_str[1] = '\0';

    char* p_char_token = strtok(p_char_c_str_copy, u8_delimiter_str);
    while (p_char_token != nullptr) {
        c_tokens.push_back(MyString(p_char_token));
        p_char_token = strtok(nullptr, u8_delimiter_str);
    }
    delete[] p_char_c_str_copy;
    p_char_c_str_copy = nullptr;
    return c_tokens;
}

User::User(const MyString& p_firstName, const MyString& p_lastName, size_t u32inputID, const MyString& p_password, UserRole e_inputRole)
    : c_firstName(p_firstName), c_lastName(p_lastName), u32id(u32inputID), c_password(p_password), e_role(e_inputRole) {
}

User::User(const MyString& p_dataLine) {
    Vector<MyString> c_parts = splitMyString(p_dataLine, ',');
    if (c_parts.getSize() >= 5U) {
        c_firstName = c_parts[0U];
        c_lastName = c_parts[1U];
        u32id = myStringToInt(c_parts[2U]);
        c_password = c_parts[3U];

        MyString c_role_str = c_parts[4U];
        if (strcmp(c_role_str.c_str(), "Admin") == 0) {
            e_role = ADMIN;
        }
        else if (strcmp(c_role_str.c_str(), "Teacher") == 0) {
            e_role = TEACHER;
        }
        else if (strcmp(c_role_str.c_str(), "Student") == 0) {
            e_role = STUDENT;
        }
        else {
            e_role = UNKNOWN_ROLE;
        }

        if (c_parts.getSize() > 5U) {
            MyString c_messages_str = c_parts[5U];
            if (c_messages_str.length() > 2) {
                char* p_char_msg_c_str = new char[static_cast<size_t>(c_messages_str.length()) + 1U];
                (void)strcpy(p_char_msg_c_str, c_messages_str.c_str());

                char* p_char_start_brace = strchr(p_char_msg_c_str, '{');
                if (p_char_start_brace != nullptr) {
                    p_char_start_brace++;
                    char* p_char_end_brace = strrchr(p_char_start_brace, '}');
                    if (p_char_end_brace != nullptr) {
                        *p_char_end_brace = '\0';
                    }

                    char* p_char_token = strtok(p_char_start_brace, "\" ,");
                    while (p_char_token != nullptr) {
                        if (strlen(p_char_token) > 0U) {
                            c_inbox.push_back(MyString(p_char_token));
                        }
                        p_char_token = strtok(nullptr, "\" ,");
                    }
                }
                delete[] p_char_msg_c_str;
                p_char_msg_c_str = nullptr;
            }
        }

        if (c_parts.getSize() > 6U) {
            MyString c_courses_str = c_parts[6U];
            if (c_courses_str.length() > 2) {
                char* p_char_course_c_str = new char[static_cast<size_t>(c_courses_str.length()) + 1U];
                (void)strcpy(p_char_course_c_str, c_courses_str.c_str());

                char* p_char_start_bracket = strchr(p_char_course_c_str, '[');
                if (p_char_start_bracket != nullptr) {
                    p_char_start_bracket++;
                    char* p_char_end_bracket = strrchr(p_char_start_bracket, ']');
                    if (p_char_end_bracket != nullptr) {
                        *p_char_end_bracket = '\0';
                    }

                    char* p_char_course_token = strtok(p_char_start_bracket, ",");
                    while (p_char_course_token != nullptr) {
                        if (strlen(p_char_course_token) > 0U) {
                            Vector<MyString> c_course_details = splitMyString(MyString(p_char_course_token), ':');
                            if (c_course_details.getSize() >= 3U) {
                                MyString c_course_name = c_course_details[0U];
                                MyString c_assignment_and_grade = c_course_details[1U];
                                c_assignment_and_grade += ":";
                                c_assignment_and_grade += c_course_details[2U];

                                Vector<MyString> c_assignments_for_course;
                                c_assignments_for_course.push_back(c_assignment_and_grade);
                                courses.insert(c_course_name, c_assignments_for_course);
                            }
                        }
                        p_char_course_token = strtok(nullptr, ",");
                    }
                }
                delete[] p_char_course_c_str;
                p_char_course_c_str = nullptr;
            }
        }
    }
}

MyString User::getFirstName() const { return c_firstName; }
MyString User::getLastName() const { return c_lastName; }
size_t User::getId() const { return u32id; }
MyString User::getPassword() const { return c_password; }
UserRole User::getRole() const { return e_role; }

void User::setPassword(const MyString& p_newPassword) {
    this->c_password = p_newPassword;
}

void User::addMessage(const MyString& p_message) {
    c_inbox.push_back(p_message);
}

void User::viewInbox() const {
    std::cout << "--- " << c_firstName.c_str() << "'s Inbox ---" << std::endl;
    if (c_inbox.getSize() == 0U) {
        std::cout << "Inbox is empty." << std::endl;
        return;
    }
    for (size_t u32_idx = 0U; u32_idx < c_inbox.getSize(); ++u32_idx) {
        std::cout << "- " << c_inbox[u32_idx].c_str() << std::endl;
    }
    std::cout << "-----------------------" << std::endl;
}

MyString User::serialize() const {
    MyString c_serialized_data = c_firstName;
    c_serialized_data += ",";
    c_serialized_data += c_lastName;
    c_serialized_data += ",";
    c_serialized_data += intToMyString(u32id);
    c_serialized_data += ",";
    c_serialized_data += c_password;
    c_serialized_data += ",";
    if (e_role == ADMIN) {
        c_serialized_data += "Admin";
    }
    else if (e_role == TEACHER) {
        c_serialized_data += "Teacher";
    }
    else if (e_role == STUDENT) {
        c_serialized_data += "Student";
    }
    else {
        c_serialized_data += "Unknown";
    }

    c_serialized_data += ",{";
    for (size_t u32_idx = 0U; u32_idx < c_inbox.getSize(); ++u32_idx) {
        c_serialized_data += "\"";
        c_serialized_data += c_inbox[u32_idx];
        c_serialized_data += "\"";
        if (u32_idx < (c_inbox.getSize() - 1U)) {
            c_serialized_data += ",";
        }
    }
    c_serialized_data += "},[";

    c_serialized_data += "]";

    c_serialized_data += ";";
    return c_serialized_data;
}