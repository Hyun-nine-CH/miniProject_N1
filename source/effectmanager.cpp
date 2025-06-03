#include "effectmanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// ✅ 파일 경로 상수 추가
const std::string EFFECT_FILE = "data/pdList_effects.txt";

inline std::string addComma(int number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << std::fixed << number;
    return oss.str();
}

EffectManager::EffectManager() {
    loadFromFile(); // ✅ 파일명 명시 제거
}

void EffectManager::loadFromFile() {
    effects.clear();
    std::ifstream file(EFFECT_FILE);
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string code, model, brand, effectType;
        int price, stock;

        getline(ss, code, ',');
        getline(ss, model, ',');
        getline(ss, brand, ',');
        getline(ss, effectType, ',');
        ss >> price; ss.ignore();
        ss >> stock;

        effects.emplace_back(code, model, brand, effectType, price, stock);
    }
}

void EffectManager::saveToFile() const {
    std::ofstream file(EFFECT_FILE);
    for (const auto& e : effects) {
        file << e.getCode() << ","
             << e.getModel() << ","
             << e.getBrand() << ","
             << e.getEffectType() << ","
             << e.getPrice() << ","
             << e.getStock() << "\n";
    }
}

void EffectManager::inputEffect() {
    std::string code, model, brand, effectType;
    int price, stock;

    std::cin.ignore(); // 버퍼 초기화
    std::cout << "코드: ";
    std::getline(std::cin, code);

    std::cout << "모델명: ";
    std::getline(std::cin, model);

    std::cout << "브랜드: ";
    std::getline(std::cin, brand);

    std::cout << "타입: ";
    std::getline(std::cin, effectType);

    std::cout << "가격: ";
    std::cin >> price;
    std::cin.ignore(); // 버퍼 초기화

    std::cout << "재고: ";
    std::cin >> stock;
    std::cin.ignore(); // 버퍼 초기화

    effects.emplace_back(code, model, brand, effectType, price, stock);
    saveToFile(); // ✅ 저장
}

void EffectManager::deleteEffect() {
    std::string code;
    std::cout << "삭제할 코드: ";
    std::cin >> code;

    auto it = std::remove_if(effects.begin(), effects.end(),
        [&](const Effect& e) { return e.getCode() == code; });

    if (it != effects.end()) {
        effects.erase(it, effects.end());
        std::cout << "삭제되었습니다.\n";
        saveToFile(); // ✅ 저장
    } else {
        std::cout << "해당 코드를 찾을 수 없습니다.\n";
    }
}

bool EffectManager::displayMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 이펙터 관리 ---\n";
        std::cout << "1. 등록\n";
        std::cout << "2. 삭제\n";
        std::cout << "3. 전체 목록 보기\n";
        std::cout << "4. 이전 메뉴로 돌아가기\n";
        std::cout << "선택: ";
        std::cin >> choice;

        switch (choice) {
            case 1: inputEffect(); break;
            case 2: deleteEffect(); break;
            case 3: displayEffect(); break;
            case 4: return true; // ProductManager로 돌아가기
            default: std::cout << "잘못된 선택입니다.\n"; break;
        }
    }
    return true;
}

void EffectManager::displayEffect() const {
    while (true) {
        std::cout << "\n코드       | 모델명         | 브랜드       | 타입     | 가격       | 재고\n";
        std::cout << "--------------------------------------------------------------------------\n";
        for (const auto& e : effects) {
            std::cout << std::setw(10) << e.getCode() << " | "
                      << std::setw(12) << e.getModel() << " | "
                      << std::setw(12) << e.getBrand() << " | "
                      << std::setw(8) << e.getEffectType() << " | "
                      << std::setw(10) << addComma(e.getPrice()) << " | "
                      << std::setw(5) << addComma(e.getStock()) << "\n";
        }

        std::cout << "\n========= 키워드 검색 =========\n";
        std::cout << "1. 검색하기\n";
        std::cout << "2. 되돌아가기\n";
        std::cout << "선택: ";

        int opt;
        std::cin >> opt;

        if (opt == 1) {
            std::cin.ignore();
            std::string keyword;
            std::cout << "검색할 키워드 입력: ";
            std::getline(std::cin, keyword);

            std::cout << "\n[검색 결과]\n";
            std::cout << "\n코드       | 모델명         | 브랜드       | 타입     | 가격       | 재고\n";
            std::cout << "--------------------------------------------------------------------------\n";

            bool found = false;
            for (const auto& e : effects) {
                if (e.getModel().find(keyword) != std::string::npos ||
                    e.getBrand().find(keyword) != std::string::npos ||
                    e.getEffectType().find(keyword) != std::string::npos ||
                    e.getCode().find(keyword) != std::string::npos) {
                    std::cout << std::setw(10) << e.getCode() << " | "
                              << std::setw(12) << e.getModel() << " | "
                              << std::setw(12) << e.getBrand() << " | "
                              << std::setw(8)  << e.getEffectType() << " | "
                              << std::setw(10) << addComma(e.getPrice()) << " | "
                              << std::setw(5)  << addComma(e.getStock()) << "\n";
                    found = true;
                }
            }

            if (!found) {
                std::cout << "일치하는 항목이 없습니다.\n";
            }

            std::cout << "\n확인을 했다면 Enter 키를 입력하세요";
            std::cin.get();
            return; // 검색 결과만 보여주고 종료

        } else if (opt == 2) {
            return; // displayMenu로 돌아감
        } else {
            std::cout << "잘못된 선택입니다.\n";
        }
    }
}

const Effect* EffectManager::findByCode(const std::string& code) const {
    for (auto& e : effects) {
        if (e.getCode() == code)
            return &e;
    }
    return nullptr;
}

Effect* EffectManager::findByCode(const std::string& code) {
    for (auto& e : effects) {
        if (e.getCode() == code)
            return &e;
    }
    return nullptr;
}