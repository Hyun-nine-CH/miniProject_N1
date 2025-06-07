#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order { // 하나의 주문 데이터를 표현하는 클래스
public:
    Order(int orderId, int clientId, const std::string& productCode,
          int quantity, const std::string& date, const std::string& status = "구매완료");
        // 기본 주문 상태는 "구매완료"로 설정됨
        // 주문 생성자, 주문 ID, 고객 ID, 제품 코드, 수량, 날짜, 상태를 초기화
        // 상태는 "구매완료" 또는 "환불완료"로 설정 가능
        
    // Getter
    int getOrderId() const;
    int getClientId() const;
    std::string getProductCode() const;
    int getQuantity() const;
    std::string getDate() const;
    std::string getStatus() const;
    // 각각의 private 멤버를 읽어오는 용도

    // Setter
    void setStatus(const std::string& newStatus);
    void setDate(const std::string& newDate);
    // 주문 상태 또는 날짜를 변경할 수 있도록 허용

private:
    int orderId;
    int clientId;
    std::string productCode;
    int quantity;
    std::string date;
    std::string status; // "구매완료", "환불완료"
};

#endif // ORDER_H
