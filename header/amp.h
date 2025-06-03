#ifndef AMP_H
#define AMP_H

#include "subproduct.h"

class Amp : public SubProduct {
public:
    Amp() = default;

    Amp(const std::string& code,
        const std::string& model,
        const std::string& brand,
        int price, int stock);

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // AMP_H