#include "effect.h"

Effect::Effect(const std::string& code,
               const std::string& model,
               const std::string& brand,
               const std::string& effectType,
               int price,
               int stock)
    : code(code), model(model), brand(brand), effectType(effectType),
      price(price), stock(stock) {}

std::string Effect::getCode() const { return code; }
std::string Effect::getModel() const { return model; }
std::string Effect::getBrand() const { return brand; }
std::string Effect::getEffectType() const { return effectType; }
int Effect::getPrice() const { return price; }
int Effect::getStock() const { return stock; }

void Effect::setPrice(int price) { this->price = price; }
void Effect::setStock(int stock) { this->stock = stock; }

bool Effect::isStockAvailable(int qty) const {
    return stock >= qty;
}

void Effect::reduceStock(int qty) {
    if (qty <= stock)
        stock -= qty;
}
void Effect::addStock(int qty) {
    stock += qty;
}