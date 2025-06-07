#ifndef POWERSUPPLYMANAGER_H
#define POWERSUPPLYMANAGER_H

#include <vector>
#include <string>
#include "powersupply.h"

// Accessorymanager.h의 기본 설명과 같다.

class PowerSupplyManager {
private:
    std::vector<PowerSupply> powersupplies;

public:
    PowerSupplyManager();

    void loadFromFile();
    void saveToFile() const;

    void inputPowerSupply();    // 콘솔 입력
    void deletePowerSupply();   // 코드 기준 삭제
    void displayPowerSupply() const;

    const PowerSupply* findByCode(const std::string& code) const;
    PowerSupply* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // POWERSUPPLYMANAGER_H