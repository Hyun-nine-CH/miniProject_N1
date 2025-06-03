#include "tuner.h"

Tuner::Tuner(const std::string& code,
             const std::string& model,
             const std::string& brand,
             int price, int stock)
    : SubProduct(code, model, brand, price, stock) {}

bool Tuner::isStockAvailable(int qty) const {
    return getStock() >= qty;
}

void Tuner::reduceStock(int qty) {
    if (qty <= getStock()) {
        setStock(getStock() - qty);
    }
}

void Tuner::addStock(int qty) {
    setStock(getStock() + qty);
}
