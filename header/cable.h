#ifndef CABLE_H
#define CABLE_H

#include "subproduct.h"

// Accessory.h의 기본 설명과 같다.

class Cable : public SubProduct {
    // Amp, Tuner, PowerSupply, Cable와 같은 제품군은 공통적으로 code, model, brand, price, stock 멤버 변수를 가지며, 내용은 같다.
public:
    Cable() = default;

    Cable(const std::string& code,
          const std::string& model,
          const std::string& brand,
          int price, int stock);

// accessory.h와 달리, tuner, amp, powersupply, cable은 부모클래스로부터 상속 받을 요소가 공통으로 들어있기 때문에 비어있는 공간이다.

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // CABLE_H
