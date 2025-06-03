#include "amp.h"

Amp::Amp(const std::string& code,
         const std::string& model,
         const std::string& brand,
         int price, int stock)
    : SubProduct(code, model, brand, price, stock) {}

bool Amp::isStockAvailable(int qty) const {
    return getStock() >= qty;
}

void Amp::reduceStock(int qty) {
    if (qty <= getStock()) {
        setStock(getStock() - qty);
    }
}

void Amp::addStock(int qty) {
    setStock(getStock() + qty);
}
