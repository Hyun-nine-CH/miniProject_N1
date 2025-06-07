#ifndef GUITARMANAGER_H
#define GUITARMANAGER_H

#include <vector>
#include <string>
#include "guitar.h"

// Accessorymanager.h의 기본 설명과 같다.

class GuitarManager {
private:
    std::vector<Guitar> guitars;

public:
    GuitarManager();

    void loadFromFile();
    void saveToFile() const;

    void inputGuitar();    // 콘솔 입력
    void deleteGuitar();   // 코드 기준 삭제
    void displayGuitar() const;

    const Guitar* findByCode(const std::string& code) const;
    Guitar* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // GUITARMANAGER_H
