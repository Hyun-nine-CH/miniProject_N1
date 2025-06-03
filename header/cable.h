#ifndef CABLE_H
#define CABLE_H

#include "subproduct.h"

class Cable : public SubProduct {
public:
    Cable() = default;

    Cable(const std::string& code,
          const std::string& model,
          const std::string& brand,
          int price, int stock);

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // CABLE_H
