#include "client.h"

Client::Client(int id, const std::string& name, const std::string& phoneNumber, const std::string& address)
    : m_id(id), m_name(name), m_phoneNumber(phoneNumber), m_address(address) {}

std::string Client::getName() const {
    return m_name;
}

void Client::setName(const std::string& name) {
    m_name = name;
}

std::string Client::getPhoneNumber() const {
    return m_phoneNumber;
}

void Client::setPhoneNumber(const std::string& phoneNumber) {
    m_phoneNumber = phoneNumber;
}

std::string Client::getAddress() const {
    return m_address;
}

void Client::setAddress(const std::string& address) {
    m_address = address;
}

int Client::id() const {
    return m_id;
}

bool Client::operator==(const Client& other) const {
    return m_id == other.m_id;
}