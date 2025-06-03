#include "order.h"

Order::Order(int orderId, int clientId, const std::string& productCode,
             int quantity, const std::string& date, const std::string& status)
    : orderId(orderId), clientId(clientId), productCode(productCode),
      quantity(quantity), date(date), status(status) {}

int Order::getOrderId() const {
    return orderId;
}

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
}

void Order::setDate(const std::string& newDate) {
    date = newDate;
}