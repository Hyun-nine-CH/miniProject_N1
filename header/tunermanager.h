#ifndef TUNERMANAGER_H
#define TUNERMANAGER_H

#include <vector>
#include <string>
#include "tuner.h"

class TunerManager {
private:
    std::vector<Tuner> tuners;

public:
    TunerManager();

    void loadFromFile();            
    void saveToFile() const;
    // 파일 이름을 인자로 받는 오버로딩된 함수

    void inputTuner();    // 콘솔 입력
    void deleteTuner();   // 코드 기준 삭제
    void displayTuner() const;

    const Tuner* findByCode(const std::string& code) const;
    Tuner* findByCode(const std::string& code);

    bool displayMenu();    // 내부 메뉴 루프
};

#endif // TUNERMANAGER_H