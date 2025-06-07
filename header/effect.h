#ifndef EFFECT_H
#define EFFECT_H

#include <string>

// Accessory.h의 기본 설명과 같다.

class Effect {
private:
    std::string code;
    std::string model;
    std::string brand;
    std::string effectType;
    int price;
    int stock;

public:
    Effect() = default;

    Effect(const std::string& code,
            const std::string& model,
            const std::string& brand,
            const std::string& effectType,
            int price,
            int stock);

// 다만, effect와 guitar의 경우에는 genreStarterSet의 추가 기능을 위해 type(타입) 요소가 추가 되었다.
// 이역시, Tuner, Amp, PowerSupply, Cable와 같이 4개의 파일에는 공통 요소로 겹치는 부분이 많은 것과 달리 요소의 개수에서부터 차이가 있어 별도로 진행하였다.
// 추가로 Accessory의 경우에는 요소의 개수는 같으나, purpose(용도)라는 이름이 다른 요소가 들어가 있어 별도 선언이 필요한 경우였다.

    std::string getCode() const;
    std::string getModel() const;
    std::string getBrand() const;
    std::string getEffectType() const;
    int getPrice() const;
    int getStock() const;

    void setPrice(int price);
    void setStock(int stock);

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // EFFECT_H