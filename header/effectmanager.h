#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <vector>
#include <string>
#include "effect.h"

class EffectManager {
private:
    std::vector<Effect> effects;

public:
    EffectManager();

    void loadFromFile();
    void saveToFile() const;

    void inputEffect();    // 콘솔 입력
    void deleteEffect();   // 코드 기준 삭제
    void displayEffect() const;

    const Effect* findByCode(const std::string& code) const;
    Effect* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // EFFECTMANAGER_H