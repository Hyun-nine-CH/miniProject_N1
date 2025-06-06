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

public:
    Accessory() = default;

    Accessory(const std::string& code,
            const std::string& model,
            const std::string& purpose,
            int price,
            int stock);
            // Accessory 생성자, 객체 생성 시, 모든 멤버 변수를 초기화하는 생성자이므로 초기화리스크트를 통해 직접 초기화되어 성능이 좋다

    std::string getCode() const;
    std::string getModel() const;
    std::string getPurpose() const;
    int getPrice() const;
    int getStock() const;

    void setPrice(int price);
    void setStock(int stock);

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty); // 각각의 메서드를 헤더 파일에 선언하였다.
};

#endif // ACCESSORY_H