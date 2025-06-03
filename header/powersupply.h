#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H

#include "subproduct.h"

class PowerSupply : public SubProduct {
public:
    PowerSupply() = default;

    PowerSupply(const std::string& code,
                const std::string& model,
                const std::string& brand,
                int price, int stock);

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // POWERSUPPLY_H
