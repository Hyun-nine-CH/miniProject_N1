#ifndef TUNER_H
#define TUNER_H

#include "subproduct.h"

class Tuner : public SubProduct {
public:
    Tuner() = default;

    Tuner(const std::string& code,
          const std::string& model,
          const std::string& brand,
          int price, int stock);

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // TUNER_H
