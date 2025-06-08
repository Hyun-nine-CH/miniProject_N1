#include "order.h"

Order::Order(int orderId, int clientId, const std::string& productCode,
             int quantity, const std::string& date, const std::string& status)
    : orderId(orderId), clientId(clientId), productCode(productCode),
      quantity(quantity), date(date), status(status) {}
      // Order 클래스의 생성자(Constructor)
      // orderId, clientId, productCode, quantity, date, status를 초기화 리스트로 초기화함
      // : 뒤의 멤버 이니셜라이저(initializer list)를 통해 각 멤버 변수에 직접 값을 대입하여 초기화

int Order::getOrderId() const {
    return orderId;
} // 고객 ID를 반환하는 getter 함수

int Order::getClientId() const {
    return clientId;
}

std::string Order::getProductCode() const {
    return productCode;
}

int Order::getQuantity() const {
    return quantity;
}

std::string Order::getDate() const {
    return date;
}

std::string Order::getStatus() const {
    return status;
}

void Order::setStatus(const std::string& newStatus) {
    status = newStatus;
} // 주문 상태를 변경하는 setter 함수

void Order::setDate(const std::string& newDate) {
    date = newDate;
} // 주문 날짜를 변경하는 setter 함수