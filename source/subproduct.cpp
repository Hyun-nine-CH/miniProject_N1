#include "subproduct.h"
// 이 클래스는 공통 제품 속성을 캡슐화한 추상화 기반 클래스(Base Class) 역할을 하며, Amp, Tuner, Accessory 등에서 상속받는 구조

SubProduct::SubProduct(const std::string& code,
                       const std::string& model,
                       const std::string& brand,
                       int price, int stock)
    : code(code), model(model), brand(brand), price(price), stock(stock) {}
// SubProduct 생성자, 멤버 변수를 초기화
// SubProduct는 Amp, Tuner, Accessory 등에서 상속받아 특정 제품군의 특성만 분리하여 관리하는 파생 클래스
// SubProduct는 Amp, Tuner, Accessory의 부모 클래스이며, Amp, Tuner, Accessory는 SubProduct를 상속
// std::string 타입을 const 참조로 받음으로써 불필요한 복사를 방지

std::string SubProduct::getCode() const {
    return code;
}

std::string SubProduct::getModel() const {
    return model;
}

std::string SubProduct::getBrand() const {
    return brand;
}

int SubProduct::getPrice() const {
    return price;
}

int SubProduct::getStock() const {
    return stock;
}
// 외부에서 객체의 정보를 읽을 수 있도록 읽기 전용 인터페이스 제공
// const 함수로 선언되어 있어, 멤버를 수정하지 않음

void SubProduct::setStock(int s) {
    stock = s;
}

void SubProduct::setPrice(int p) {
    price = p;
}
// 외부에서 객체의 정보를 수정할 수 있도록 쓰기 전용 인터페이스 제공
// 직접 멤버 변수에 접근하지 않고 메서드를 통해 조작하는 방식은 캡슐화 원칙을 지킴