#ifndef TUNERMANAGER_H
#define TUNERMANAGER_H

#include <vector>
#include <string>
#include "tuner.h"

// Accessorymanager.h의 기본 설명과 같다.

class TunerManager {
private:
    std::vector<Tuner> tuners;

public:
    TunerManager();

    void loadFromFile();            
    void saveToFile() const;

    void inputTuner();    // 콘솔 입력
    void deleteTuner();   // 코드 기준 삭제
    void displayTuner() const;

    const Tuner* findByCode(const std::string& code) const;
    Tuner* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // TUNERMANAGER_H