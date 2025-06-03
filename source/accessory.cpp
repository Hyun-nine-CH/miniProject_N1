#include "accessory.h"

Accessory::Accessory(const std::string& code,
                const std::string& model,
                const std::string& purpose,
                int price,
                int stock)
    : code(code), model(model), purpose(purpose),
      price(price), stock(stock) {}

std::string Accessory::getCode() const { return code; }
std::string Accessory::getModel() const { return model; }
std::string Accessory::getPurpose() const { return purpose; }
int Accessory::getPrice() const { return price; }
int Accessory::getStock() const { return stock; }

void Accessory::setPrice(int price) { this->price = price; }
void Accessory::setStock(int stock) { this->stock = stock; }

bool Accessory::isStockAvailable(int qty) const {
    return stock >= qty;
}

void Accessory::reduceStock(int qty) {
    if (qty <= stock)
        stock -= qty;
}
void Accessory::addStock(int qty) {
    stock += qty;
}