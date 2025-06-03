#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>

class Client {
public:
    Client(int id = 0, const std::string& name = "", const std::string& phone = "", const std::string& address = "");

    std::string getName() const;
    void setName(const std::string& name);

    std::string getPhoneNumber() const;
    void setPhoneNumber(const std::string& phone);

    std::string getAddress() const;
    void setAddress(const std::string& address);

    int id() const;
    bool operator==(const Client& other) const;

private:
    int m_id;
    std::string m_name;
    std::string m_phoneNumber;
    std::string m_address;
};

#endif // __CLIENT_H__