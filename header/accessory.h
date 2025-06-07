#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <string>

class Accessory {
private:
    std::string code;
    std::string model;
    std::string purpose;
    int price;
    int stock;
    // Accessory 클래스는 악세서리의 정보를 저장하는 클래스이며 멤버 변수를 private로 선언하여 외부에서 직접 접근하지 못하도록 한다.
    // Accessory는 Amp, Tuner, PowerSupply, Cable와 달리 purpose 멤버 변수가 존재한다.
    // purpose는 악세서리의 용도를 나타내며, Amp, Tuner, PowerSupply, Cable와 달리 purpose 멤버 변수의 이름이 다르다.
    // 따라서 Accessory는 Amp, Tuner, PowerSupply, Cable과 달리 상속받는 요소가 없다. 따라서, 이렇게 멤버 변수를 직접 선언하였다.

public:
    Accessory() = default;
    // 다른 생성자(예: Accessory(code, model, purpose, price, stock))를 정의해놓으면 컴파일러는 자동으로 기본 생성자를 만들어주지 않아 별도로 사용
    // 객체를 비워서 만들 필요가 있을 때, 파일에서 불러올 때, STL 컨테이너(vector, map 등)에서 객체 생성을 위해 필요하므로 사용
    
    Accessory(const std::string& code,
            const std::string& model,
            const std::string& purpose,
            int price,
            int stock);
            // Accessory 생성자, 객체 생성 시, 모든 멤버 변수를 초기화하는 생성자이므로 초기화리스크트를 통해 직접 초기화되어 성능이 좋다

// 여기서부터,
    std::string getCode() const;
    std::string getModel() const;
    std::string getPurpose() const;
    int getPrice() const;
    int getStock() const;

    void setPrice(int price);
    void setStock(int stock);
// 여기까지는 tuner, amp, powersupply, cable와 달리 purpose 멤버 변수의 이름이 달라서 상속에서 제외하였다. 따라서 이렇게 따로 명시하였다.

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty); // 각각의 메서드를 헤더 파일에 선언하였다.
};

#endif // ACCESSORY_H