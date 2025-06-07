#ifndef ACCESSORYMANAGER_H
#define ACCESSORYMANAGER_H

#include <vector>
#include <string>
#include "accessory.h"

class AccessoryManager {
private:
    std::vector<Accessory> accessories;

public:
    AccessoryManager();

    void loadFromFile();
    void saveToFile() const;

    void inputAccessory();    // 콘솔 입력
    void deleteAccessory();   // 코드 기준 삭제
    void displayAccessory() const;

    const Accessory* findByCode(const std::string& code) const;
    Accessory* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // ACCESSORYMANAGER_H