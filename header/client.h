#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>

class Client { // 고객 한 명의 정보를 담는 Client 클래스 정의
public:
    Client(int id = 0, const std::string& name = "", const std::string& phone = "", const std::string& address = "");
    // 기본값이 설정된 생성자

    // 이름 반환 (getName) / 변경 (setName)
    std::string getName() const; //  객체 상태 수정 금지
    void setName(const std::string& name);

    std::string getPhoneNumber() const;
    void setPhoneNumber(const std::string& phone);

    std::string getAddress() const;
    void setAddress(const std::string& address);

    int id() const; // 고객의 고유 ID 반환용 메서드를 만듦
    bool operator==(const Client& other) const; // 두 Client 객체가 같은지 비교하는 == 연산자 오버로딩

private:
    int m_id; // 고객 고유 번호
    std::string m_name; // 고객 이름
    std::string m_phoneNumber; // 고객 전화번호
    std::string m_address; // 고객 주소
};

#endif // __CLIENT_H__