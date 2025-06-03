#include "cable.h"

Cable::Cable(const std::string& code,
             const std::string& model,
             const std::string& brand,
             int price, int stock)
    : SubProduct(code, model, brand, price, stock) {}

bool Cable::isStockAvailable(int qty) const {
    return getStock() >= qty;
}

void Cable::reduceStock(int qty) {
    if (qty <= getStock()) {
        setStock(getStock() - qty);
    }
}

void Cable::addStock(int qty) {
    setStock(getStock() + qty);
}
