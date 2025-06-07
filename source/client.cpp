#include "client.h"

Client::Client(int id, const std::string& name, const std::string& phoneNumber, const std::string& address)
    : m_id(id), m_name(name), m_phoneNumber(phoneNumber), m_address(address) {}
// 생성자 초기화 리스트 방식으로 모든 멤버 초기화
// 고객을 생성할 때 ID, 이름, 전화번호, 주소를 함께 세팅

std::string Client::getName() const {
    return m_name;
}

// getter 함수로, 각각 이름, 전화번호, 주소, 고객 ID를 반환하는 함수

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

// 입력된 문자열로 내부 멤버를 수정하는 setter 함수들

int Client::id() const {
    return m_id;
}

bool Client::operator==(const Client& other) const {
    return m_id == other.m_id;
} // 고객끼리 == 연산을 하면 ID가 같은지 비교 (비교 연산자 오버로딩)