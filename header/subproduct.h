#ifndef SUBPRODUCT_H
#define SUBPRODUCT_H

#include <string>

class SubProduct {
protected:
    std::string code;
    std::string model;
    std::string brand;
    int price;
    int stock;
    // code, model, brand, price, stock은 직접적으로 외부에서는 접근하지 못하고, 파생 클래스만 접근 가능하게 하였다.
    // 캡슐화 원칙을 지키면서도 Amp, Tuner, Accessory 등 파생 클래스 구현하여 편의성 확보에 기여하였다.

public:
    SubProduct() = default;
    // 디폴트 생성자로 파일 입출력 시 필요할 수 있어서 추가하였다.
    // 다른 생성자(예: Accessory(code, model, purpose, price, stock))를 정의해놓으면 컴파일러는 자동으로 기본 생성자를 만들어주지 않아 별도로 사용
    // 객체를 비워서 만들 필요가 있을 때, 파일에서 불러올 때, STL 컨테이너(vector, map 등)에서 객체 생성을 위해 필요하므로 사용
    
    SubProduct(const std::string& code,
               const std::string& model,
               const std::string& brand,
               int price, int stock);

    // Getter
    std::string getCode() const;
    std::string getModel() const;
    std::string getBrand() const;
    int getPrice() const;
    int getStock() const;

    // Setter
    void setStock(int s);
    void setPrice(int p);

// 객체 속성 접근을 직접 노출하지 않고, 간접 접근 인터페이스로 제공하게 하였다.
// 특히 getStock(), setStock()은 주문/환불 처리에서 필수요소로 적용하였다.

    virtual ~SubProduct() = default; // SubProduct를 상속받은 포인터를 delete할 때, 파생 클래스의 소멸자도 정확히 호출되도록 하였다.
};

#endif
