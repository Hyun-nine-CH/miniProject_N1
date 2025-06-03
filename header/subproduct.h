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

public:
    SubProduct() = default;
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

    virtual ~SubProduct() = default;
};

#endif
