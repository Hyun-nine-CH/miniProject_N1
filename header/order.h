#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order {
public:
    Order(int orderId, int clientId, const std::string& productCode,
          int quantity, const std::string& date, const std::string& status = "구매완료");

    // Getter
    int getOrderId() const;
    int getClientId() const;
    std::string getProductCode() const;
    int getQuantity() const;
    std::string getDate() const;
    std::string getStatus() const;

    // Setter
    void setStatus(const std::string& newStatus);
    void setDate(const std::string& newDate);  // 날짜 수정 가능

private:
    int orderId;
    int clientId;
    std::string productCode;
    int quantity;
    std::string date;
    std::string status; // "구매완료", "환불완료"
};

#endif // ORDER_H
