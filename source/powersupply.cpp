#include "powersupply.h"

PowerSupply::PowerSupply(const std::string& code,
                         const std::string& model,
                         const std::string& brand,
                         int price, int stock)
    : SubProduct(code, model, brand, price, stock) {}

bool PowerSupply::isStockAvailable(int qty) const {
    return getStock() >= qty;
}

void PowerSupply::reduceStock(int qty) {
    if (qty <= getStock()) {
        setStock(getStock() - qty);
    }
}

void PowerSupply::addStock(int qty) {
    setStock(getStock() + qty);
}
