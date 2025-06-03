#include "guitar.h"

Guitar::Guitar(const std::string& code,
               const std::string& model,
               const std::string& brand,
               const std::string& type,
               int price,
               int stock)
    : code(code), model(model), brand(brand), type(type),
      price(price), stock(stock) {}

std::string Guitar::getCode() const { return code; }
std::string Guitar::getModel() const { return model; }
std::string Guitar::getBrand() const { return brand; }
std::string Guitar::getType() const { return type; }
int Guitar::getPrice() const { return price; }
int Guitar::getStock() const { return stock; }

void Guitar::setPrice(int price) { this->price = price; }
void Guitar::setStock(int stock) { this->stock = stock; }

bool Guitar::isStockAvailable(int qty) const {
    return stock >= qty;
}

void Guitar::reduceStock(int qty) {
    if (qty <= stock)
        stock -= qty;
}
void Guitar::addStock(int qty) {
    stock += qty;
}