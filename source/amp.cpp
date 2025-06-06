#include "amp.h"

Amp::Amp(const std::string& code,
         const std::string& model,
         const std::string& brand,
         int price, int stock)
    : SubProduct(code, model, brand, price, stock) {}
// Amp 생성자, SubProduct의 생성자를 호출하여 멤버 변수를 초기화한다.
// SubProduct는 Amp의 부모 클래스이며, Amp는 SubProduct를 상속받는다.
// 즉, Amp는 특정 제품군의 특성만 분리하여 관리하는 파생 클래스입니다.

bool Amp::isStockAvailable(int qty) const {
    return getStock() >= qty;
}

void Amp::reduceStock(int qty) {
    if (qty <= getStock()) {
        setStock(getStock() - qty);
    } // 재고가 충분한 경우에만 재고를 감소시킵니다.
} // 직접 stock 멤버에 접근하지 않고, getter/setter를 통해 캡슐화를 유지하고 있습니다.
// Amp 이외에 Cable, Tuner, PowerSupply와 같은 파생 클래스에서도 동일한 방식으로 재고를 관리하므로 이하 설명은 생략하였다.

void Amp::addStock(int qty) {
    setStock(getStock() + qty);
}
