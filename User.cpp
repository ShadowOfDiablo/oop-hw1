#define _CRT_SECURE_NO_WARNINGS
#include "User.h"
#include "CourseManagementSystem.hpp"
#include <iostream>
#include <ctime>
#include <cstring>

User::User(const MyString& c_firstName, const MyString& c_lastName,
    size_t u32id, const MyString& c_password, UserRole e_inputRole)
    : c_firstName(c_firstName), c_lastName(c_lastName),
    u32id(u32id), c_password(c_password), e_role(e_inputRole) {
}

User::User(const MyString& c_dataLine) {
    Vector<MyString> c_parts = CourseManagementSystem::splitMyString(c_dataLine, ',');
    if (c_parts.getSize() >= 5U) {
        c_firstName = c_parts[0U];
        c_lastName = c_parts[1U];
        u32id = CourseManagementSystem::myStringToInt(c_parts[2U]);
        c_password = c_parts[3U];
        MyString c_role_str = c_parts[4U];
        if (c_role_str == "Admin")
            e_role = ADMIN;
        else if (c_role_str == "Teacher")
            e_role = TEACHER;
        else if (c_role_str == "Student")
            e_role = STUDENT;
        else
            e_role = UNKNOWN_ROLE;
        if (c_parts.getSize() >= 6U) {
            MyString c_mailbox = c_parts[5U];
            if (c_mailbox.length() >= 2);
        }
    }
}

MyString User::getFirstName() const {
    return c_firstName;
}

MyString User::getLastName() const {
    return c_lastName;
}

size_t User::getId() const {
    return u32id;
}

MyString User::getPassword() const {
    return c_password;
}

UserRole User::getRole() const {
    return e_role;
}

void User::setPassword(const MyString& c_newPassword) {
    c_password = c_newPassword;
}

void User::sendMessage(size_t u32recipientId, const MyString& c_message) {
    User* p_recipient = CourseManagementSystem::findUserById(u32recipientId);
    if (p_recipient != nullptr) {
        time_t u32now = time(nullptr);
        p_recipient->inbox[u32id].push_back({ c_message, u32now });
        std::cout << "Message sent to " << p_recipient->getFirstName().c_str() << "!" << std::endl;
    }
    else {
        std::cout << "User with ID " << CourseManagementSystem::intToMyString(u32recipientId).c_str() << " not found." << std::endl;
    }
}

void User::viewMailbox() const {
    std::cout << "\n--- Mailbox for " << getFirstName().c_str() << " ---\n";
    if (inbox.isEmpty()) {
        std::cout << "No messages.\n";
        return;
    }
    for (size_t u32_i = 0U; u32_i < inbox.getSize(); ++u32_i) {
        std::cout << "\n** From " << CourseManagementSystem::intToMyString(inbox.data[u32_i].key).c_str() << " **\n";
        for (size_t u32_j = 0U; u32_j < inbox.data[u32_i].value.getSize(); ++u32_j) {
            time_t u32timestamp = inbox.data[u32_i].value[u32_j].second;
            char c_buffer[26];
            struct tm* p_tm_info = localtime(&u32timestamp);
            strftime(c_buffer, 26, "%Y-%m-%d %H:%M:%S", p_tm_info);
            std::cout << "  [" << c_buffer << "] " << inbox.data[u32_i].value[u32_j].first.c_str() << "\n";
        }
    }
    std::cout << "------------------------------------\n";
}

void User::clearMailbox() {
    inbox.clear();
}

void User::changePassword(MyString c_oldPassm, MyString c_newPass) {
    if (c_oldPassm == getPassword())
        setPassword(c_newPass);
}

MyString User::serialize() const {
    MyString c_data = c_firstName + MyString(",") +
        c_lastName + MyString(",") +
        CourseManagementSystem::intToMyString(u32id) + MyString(",") +
        c_password + MyString(",");
    switch (e_role) {
    case ADMIN:
        c_data += "Admin";
        break;
    case TEACHER:
        c_data += "Teacher";
        break;
    case STUDENT:
        c_data += "Student";
        break;
    default:
        c_data += "Unknown";
        break;
    }
    MyString c_mailbox = "{";
    for (size_t u32_i = 0U; u32_i < inbox.getSize(); ++u32_i) {
        const Vector<std::pair<MyString, time_t>>& c_messages = inbox.data[u32_i].value;
        for (size_t u32_j = 0U; u32_j < c_messages.getSize(); ++u32_j) {
            c_mailbox += c_messages[u32_j].first;
            if (!(u32_i == inbox.getSize() - 1 && u32_j == c_messages.getSize() - 1))
                c_mailbox += ",";
        }
    }
    c_mailbox += "}";
    c_data += MyString(",") + c_mailbox;
    return c_data;
}
