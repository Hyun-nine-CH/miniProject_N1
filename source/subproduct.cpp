#include "subproduct.h"

SubProduct::SubProduct(const std::string& code,
                       const std::string& model,
                       const std::string& brand,
                       int price, int stock)
    : code(code), model(model), brand(brand), price(price), stock(stock) {}

std::string SubProduct::getCode() const {
    return code;
}

std::string SubProduct::getModel() const {
    return model;
}

std::string SubProduct::getBrand() const {
    return brand;
}

int SubProduct::getPrice() const {
    return price;
}

int SubProduct::getStock() const {
    return stock;
}

void SubProduct::setStock(int s) {
    stock = s;
}

void SubProduct::setPrice(int p) {
    price = p;
}
