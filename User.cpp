#define _CRT_SECURE_NO_WARNINGS
#include "User.h"
#include "CourseManagementSystem.hpp"
#include <iostream>
#include <ctime>

User::User(const MyString& p_firstName, const MyString& p_lastName,
    size_t u32inputID, const MyString& p_password, UserRole e_inputRole)
    : c_firstName(p_firstName), c_lastName(p_lastName),
    u32id(u32inputID), c_password(p_password), e_role(e_inputRole) {
}

User::User(const MyString& p_dataLine) {
    // Use the static splitMyString function.
    Vector<MyString> c_parts = CourseManagementSystem::splitMyString(p_dataLine, ',');
    if (c_parts.getSize() >= 5U) {
        c_firstName = c_parts[0U];
        c_lastName = c_parts[1U];
        u32id = CourseManagementSystem::myStringToInt(c_parts[2U]);
        c_password = c_parts[3U];

        MyString c_role_str = c_parts[4U];
        if (c_role_str == "Admin") {
            e_role = ADMIN;
        }
        else if (c_role_str == "Teacher") {
            e_role = TEACHER;
        }
        else if (c_role_str == "Student") {
            e_role = STUDENT;
        }
        else {
            e_role = UNKNOWN_ROLE;
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

void User::setPassword(const MyString& p_newPassword) {
    c_password = p_newPassword;
}

void User::sendMessage(size_t u32recipientId, const MyString& c_message) {
    User* p_recipient = CourseManagementSystem::findUserById(u32recipientId);

    if (p_recipient != nullptr) {
        time_t u32now = time(nullptr);
        // Assuming inbox is a map-like container keyed on the sender's id.
        p_recipient->inbox[u32id].push_back({ c_message, u32now });
        std::cout << "Message sent to " << p_recipient->getFirstName() << "!" << std::endl;
    }
    else {
        std::cout << "User with ID "
            << CourseManagementSystem::intToMyString(u32recipientId)
            << " not found." << std::endl;
    }
}

void User::viewMailbox() const {
    std::cout << "\n--- Mailbox for " << getFirstName() << " ---\n";

    if (inbox.isEmpty()) {
        std::cout << "No messages.\n";
        return;
    }

    for (size_t u32idx = 0U; u32idx < inbox.getSize(); ++u32idx) {
        std::cout << "\n** From "
            << CourseManagementSystem::intToMyString(inbox.data[u32idx].key)
            << " **\n";

        for (size_t u32j = 0U; u32j < inbox.data[u32idx].value.getSize(); ++u32j) {
            time_t u32timestamp = inbox.data[u32idx].value[u32j].second;
            char u8buffer[26];
            struct tm* p_tm_info = localtime(&u32timestamp);
            strftime(u8buffer, 26, "%Y-%m-%d %H:%M:%S", p_tm_info);

            std::cout << "  [" << u8buffer << "] "
                << inbox.data[u32idx].value[u32j].first << "\n";
        }
    }

    std::cout << "------------------------------------\n";
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
    return c_data;
}
