#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H

#include "subproduct.h"

// Accessory.h의 기본 설명과 같다.

class PowerSupply : public SubProduct {
public:
    PowerSupply() = default;

    PowerSupply(const std::string& code,
                const std::string& model,
                const std::string& brand,
                int price, int stock);

// accessory.h와 달리, tuner, amp, powersupply, cable은 부모클래스로부터 상속 받을 요소가 공통으로 들어있기 때문에 비어있는 공간이다.

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // POWERSUPPLY_H
