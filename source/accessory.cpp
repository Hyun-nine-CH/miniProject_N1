#include "accessory.h"

Accessory::Accessory(const std::string& code,
                const std::string& model,
                const std::string& purpose,
                int price,
                int stock)
    : code(code), model(model), purpose(purpose),
      price(price), stock(stock) {}
      // Accessory 생성자, 객체 생성 시, 모든 멤버 변수를 초기화하는 생성자이므로 초기화리스크트를 통해 직접 초기화되어 성능이 좋다
      // 매개변수와 멤버변수 이름이 같아 this->(* && .)를 생략하기 위해 초기화리스트를 사용하였다.

std::string Accessory::getCode() const { return code; }
std::string Accessory::getModel() const { return model; }
std::string Accessory::getPurpose() const { return purpose; }
int Accessory::getPrice() const { return price; }
int Accessory::getStock() const { return stock; }
// 순수 읽기 전용 함수로 const를 사용하여 객체의 상태를 변경하지 않음을 명시하였다.
// getter 함수들은 객체의 상태를 읽기만 하므로 const로 선언되어 있다.

void Accessory::setPrice(int price) { this->price = price; }
void Accessory::setStock(int stock) { this->stock = stock; }
// 외부에서 가격이나 재고를 변경할 수 있도록 setter 함수를 제공한다.

bool Accessory::isStockAvailable(int qty) const {
    return stock >= qty;
} // 재고가 충분한지 확인하는 함수로, 요청된 수량이 현재 재고보다 작거나 같은지 비교한다.
// 재고가 충분한 경우 true를 반환하고, 그렇지 않으면 false를 반환한다.
// 이 함수는 const로 선언되어 있어 객체의 상태를 변경하지 않는다.

void Accessory::reduceStock(int qty) {
    if (qty <= stock)
        stock -= qty;
} // 재고를 감소시키는 함수로, 요청된 수량이 현재 재고보다 작거나 같은 경우에만 재고를 감소시킨다.

void Accessory::addStock(int qty) {
    stock += qty;
} // 재고를 증가시키는 함수로, 요청된 수량만큼 재고를 증가시킨다.