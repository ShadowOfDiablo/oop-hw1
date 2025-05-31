#include "Admin.h"
#include <cstring>
#include "Teacher.h"
#include "Student.h"

Admin::Admin(const MyString& p_firstName, const MyString& p_lastName, size_t u32id, const MyString& p_password)
    : User(p_firstName, p_lastName, u32id, p_password, ADMIN) {
}

Admin::Admin(const MyString& p_dataLine)
    : User(p_dataLine) {
}

Teacher* Admin::addTeacher(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password) {
    return new Teacher(c_firstName, c_lastName, u32_id, c_password);
}

Student* Admin::addStudent(const MyString& c_firstName, const MyString& c_lastName, size_t u32_id, const MyString& c_password)
{
    return new Student(c_firstName, c_lastName, u32_id, c_password);
}

void Admin::changePassword(MyString oldPassm, MyString newPass)
{
    if (oldPassm == getPassword())
    {
        setPassword(newPass);
    }
}

MyString Admin::serialize() const {
    return User::serialize();
}

void Admin::sendMessage(size_t u32recipientId, MyString message) {
    User::sendMessage(u32recipientId, message);
}


void Admin::viewMailbox() const {
    User::viewMailbox();
};

void Admin::message_all(MyString msg)
{
    Vector<User*>& users = CourseManagementSystem::getUsers();
    MyString fullMessage = MyString("Admin: ") + msg;

    for (size_t u32_i = 0U; u32_i < users.getSize(); ++u32_i) {
        if (users[u32_i] != nullptr && users[u32_i]->getId() != this->getId()) {
            sendMessage(users[u32_i]->getId(), fullMessage);
        }
    }
    std::cout << "Broadcast successful: " << fullMessage.c_str() << std::endl;
}

void Admin::clearMailbox()
{
    inbox.clear();
}
