#ifndef CABLEMANAGER_H
#define CABLEMANAGER_H

#include <vector>
#include <string>
#include "cable.h"

// Accessorymanager.h의 기본 설명과 같다.

class CableManager {
private:
    std::vector<Cable> cables;

public:
    CableManager();  

    void loadFromFile();
    void saveToFile() const;

    void inputCable();    // 콘솔 입력
    void deleteCable();   // 코드 기준 삭제
    void displayCable() const;

    const Cable* findByCode(const std::string& code) const;
    Cable* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // CABLEMANAGER_H