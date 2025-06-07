#include "tuner.h"

// amp.cpp의 기본 설명과 같다.

Tuner::Tuner(const std::string& code,
             const std::string& model,
             const std::string& brand,
             int price, int stock)
    : SubProduct(code, model, brand, price, stock) {}
// Tuner 생성자, SubProduct의 생성자를 호출하여 멤버 변수를 초기화한다.
// Tuner는 SubProduct를 상속받아 특정 제품군의 특성만 분리하여 관리하는 파생 클래스です。
// SubProduct는 Tuner의 부모 클래스이며, Tuner는 SubProduct를 상속받는다.

bool Tuner::isStockAvailable(int qty) const {
    return getStock() >= qty;
}

void Tuner::reduceStock(int qty) {
    if (qty <= getStock()) {
        setStock(getStock() - qty);
    }
}

void Tuner::addStock(int qty) {
    setStock(getStock() + qty);
}
