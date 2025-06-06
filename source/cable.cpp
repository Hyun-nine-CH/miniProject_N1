#include "cable.h"

Cable::Cable(const std::string& code,
             const std::string& model,
             const std::string& brand,
             int price, int stock)
    : SubProduct(code, model, brand, price, stock) {}
// Cable 생성자, SubProduct의 생성자를 호출하여 멤버 변수를 초기화한다.
// Cable는 SubProduct를 상속받아 특정 제품군의 특성만 분리하여 관리하는 파생 클래스입니다.
// SubProduct는 Cable의 부모 클래스이며, Cable는 SubProduct를 상속받는다.

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
