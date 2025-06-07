#include "accessorymanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// ampmanager.cpp의 기본 설명과 같다.

const std::string ACCESSORY_FILE = "data/pdList_accessories.txt";

inline std::string addComma(int number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << std::fixed << number;
    return oss.str();
}

AccessoryManager::AccessoryManager() {
    loadFromFile(); // 기본 경로 사용
}

// 파일명 명시적 로드
void AccessoryManager::loadFromFile() {
    accessories.clear();
    std::ifstream file(ACCESSORY_FILE);
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string code, model, purpose;
        int price, stock;

        getline(ss, code, ',');
        getline(ss, model, ',');
        getline(ss, purpose, ',');
        ss >> price; ss.ignore();
        ss >> stock;

        accessories.emplace_back(code, model, purpose, price, stock);
    }
}

// 파일명 명시적 저장
void AccessoryManager::saveToFile() const {
    std::ofstream file(ACCESSORY_FILE);
    for (const auto& as : accessories) {
        file << as.getCode() << ","
             << as.getModel() << ","
             << as.getPurpose() << ","
             << as.getPrice() << ","
             << as.getStock() << "\n";
    }
}

void AccessoryManager::inputAccessory() {
    std::string code, model, purpose;
    int price, stock;

    std::cin.ignore(); // 버퍼 초기화
    std::cout << "코드: ";
    std::getline(std::cin, code);

    std::cout << "모델명: ";
    std::getline(std::cin, model);

    std::cout << "용도: ";
    std::getline(std::cin, purpose);

    std::cout << "가격: ";
    std::cin >> price;
    std::cin.ignore(); // 버퍼 초기화

    std::cout << "재고: ";
    std::cin >> stock;
    std::cin.ignore(); // 버퍼 초기화

    accessories.emplace_back(code, model, purpose, price, stock);
    saveToFile();  // 기본 경로 저장
}

void AccessoryManager::deleteAccessory() {
    std::string code;
    std::cout << "삭제할 코드: ";
    std::cin >> code;

    auto it = std::remove_if(accessories.begin(), accessories.end(),
        [&](const Accessory& as) { return as.getCode() == code; });

    if (it != accessories.end()) {
        accessories.erase(it, accessories.end());
        std::cout << "삭제되었습니다.\n";
        saveToFile();
    } else {
        std::cout << "해당 코드를 찾을 수 없습니다.\n";
    }
}

bool AccessoryManager::displayMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 액세서리 관리 ---\n";
        std::cout << "1. 등록\n";
        std::cout << "2. 삭제\n";
        std::cout << "3. 전체 목록 보기\n";
        std::cout << "4. 이전 메뉴로 돌아가기\n";
        std::cout << "선택: ";
        std::cin >> choice;

        switch (choice) {
            case 1: inputAccessory(); break;
            case 2: deleteAccessory(); break;
            case 3: displayAccessory(); break;
            case 4: return true; // ProductManager로 돌아가기
            default: std::cout << "잘못된 선택입니다.\n"; break;
        }
    }
    return true;
}

void AccessoryManager::displayAccessory() const {
    while (true) {
        std::cout << "\n코드       | 모델명         | 용도     | 가격       | 재고\n";
        std::cout << "------------------------------------------------------------\n";
        for (const auto& as : accessories) {
            std::cout << std::setw(10) << as.getCode() << " | "
                      << std::setw(12) << as.getModel() << " | "
                      << std::setw(12) << as.getPurpose() << " | "
                      << std::setw(10) << addComma(as.getPrice()) << " | "
                      << std::setw(5) << addComma(as.getStock()) << "\n";
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
            std::cout << "\n코드       | 모델명         | 용도     | 가격       | 재고\n";
            std::cout << "------------------------------------------------------------\n";

            bool found = false;
            for (const auto& as : accessories) {
                if (as.getModel().find(keyword) != std::string::npos ||
                    as.getPurpose().find(keyword) != std::string::npos ||
                    as.getCode().find(keyword) != std::string::npos) {
                    std::cout << std::setw(10) << as.getCode() << " | "
                              << std::setw(12) << as.getModel() << " | "
                              << std::setw(12) << as.getPurpose() << " | "
                              << std::setw(10) << addComma(as.getPrice()) << " | "
                              << std::setw(5)  << addComma(as.getStock()) << "\n";
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

const Accessory* AccessoryManager::findByCode(const std::string& code) const{
    for (auto& as : accessories) {
        if (as.getCode() == code)
            return &as;
    }
    return nullptr;
}

Accessory* AccessoryManager::findByCode(const std::string& code) {
    for (auto& as : accessories) {
        if (as.getCode() == code)
            return &as;
    }
    return nullptr;
}