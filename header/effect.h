#ifndef EFFECT_H
#define EFFECT_H

#include <string>

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