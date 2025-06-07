#ifndef AMPMANAGER_H
#define AMPMANAGER_H

#include <vector>
#include <string>
#include "amp.h"

class AmpManager {
private:
    std::vector<Amp> amps;

public:
    AmpManager();

    void loadFromFile();
    void saveToFile() const;

    void inputAmp();    // 콘솔 입력
    void deleteAmp();   // 코드 기준 삭제
    void displayAmp() const;

    const Amp* findByCode(const std::string& code) const;
    Amp* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // AMPMANAGER_H