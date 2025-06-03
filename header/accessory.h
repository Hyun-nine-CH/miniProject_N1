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

public:
    Accessory() = default;

    Accessory(const std::string& code,
            const std::string& model,
            const std::string& purpose,
            int price,
            int stock);

    std::string getCode() const;
    std::string getModel() const;
    std::string getPurpose() const;
    int getPrice() const;
    int getStock() const;

    void setPrice(int price);
    void setStock(int stock);

    bool isStockAvailable(int qty) const;
    void reduceStock(int qty);
    void addStock(int qty);
};

#endif // ACCESSORY_H