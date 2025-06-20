#include "powersupplymanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// ampmanager.cpp의 기본 설명과 같다.

const std::string POWERSUPPLY_FILE = "data/pdList_powersupplies.txt";

inline std::string addComma(int number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << std::fixed << number;
    return oss.str();
}

PowerSupplyManager::PowerSupplyManager() {
    loadFromFile();
}

void PowerSupplyManager::loadFromFile() {
    powersupplies.clear();
    std::ifstream file(POWERSUPPLY_FILE);
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string code, model, brand;
        int price, stock;

        getline(ss, code, ',');
        getline(ss, model, ',');
        getline(ss, brand, ',');
        ss >> price; ss.ignore();
        ss >> stock;

        powersupplies.emplace_back(code, model, brand, price, stock);
    }
}

void PowerSupplyManager::saveToFile() const {
    std::ofstream file(POWERSUPPLY_FILE);
    for (const auto& ps : powersupplies) {
        file << ps.getCode() << ","
             << ps.getModel() << ","
             << ps.getBrand() << ","
             << ps.getPrice() << ","
             << ps.getStock() << "\n";
    }
}

void PowerSupplyManager::inputPowerSupply() {
    std::string code, model, brand;
    int price, stock;

    std::cin.ignore(); // 버퍼 초기화
    std::cout << "코드: ";
    std::getline(std::cin, code);

    std::cout << "모델명: ";
    std::getline(std::cin, model);

    std::cout << "브랜드: ";
    std::getline(std::cin, brand);

    std::cout << "가격: ";
    std::cin >> price;
    std::cin.ignore(); // 버퍼 초기화

    std::cout << "재고: ";
    std::cin >> stock;
    std::cin.ignore(); // 버퍼 초기화

    powersupplies.emplace_back(code, model, brand, price, stock);
    saveToFile();
}

void PowerSupplyManager::deletePowerSupply() {
    std::string code;
    std::cout << "삭제할 코드: ";
    std::cin >> code;

    auto it = std::remove_if(powersupplies.begin(), powersupplies.end(),
        [&](const PowerSupply& ps) { return ps.getCode() == code; });

    if (it != powersupplies.end()) {
        powersupplies.erase(it, powersupplies.end());
        std::cout << "삭제되었습니다.\n";
        saveToFile();
    } else {
        std::cout << "해당 코드를 찾을 수 없습니다.\n";
    }
}

bool PowerSupplyManager::displayMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 파워서플라이 관리 ---\n";
        std::cout << "1. 등록\n";
        std::cout << "2. 삭제\n";
        std::cout << "3. 전체 목록 보기\n";
        std::cout << "4. 이전 메뉴로 돌아가기\n";
        std::cout << "선택: ";
        std::cin >> choice;

        switch (choice) {
            case 1: inputPowerSupply(); break;
            case 2: deletePowerSupply(); break;
            case 3: displayPowerSupply(); break;
            case 4: return true; // ProductManager로 돌아가기
            default: std::cout << "잘못된 선택입니다.\n"; break;
        }
    }
    return true;
}

void PowerSupplyManager::displayPowerSupply() const {
    while (true) {
        std::cout << "\n코드       | 모델명         | 브랜드     | 가격       | 재고\n";
        std::cout << "------------------------------------------------------------\n";
        for (const auto& ps : powersupplies) {
            std::cout << std::setw(10) << ps.getCode() << " | "
                      << std::setw(12) << ps.getModel() << " | "
                      << std::setw(12) << ps.getBrand() << " | "
                      << std::setw(10) << addComma(ps.getPrice()) << " | "
                      << std::setw(5) << addComma(ps.getStock()) << "\n";
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
            std::cout << "\n코드       | 모델명         | 브랜드     | 가격       | 재고\n";
            std::cout << "------------------------------------------------------------\n";

            bool found = false;
            for (const auto& ps : powersupplies) {
                if (ps.getModel().find(keyword) != std::string::npos ||
                    ps.getBrand().find(keyword) != std::string::npos ||
                    ps.getCode().find(keyword) != std::string::npos) {
                    std::cout << std::setw(10) << ps.getCode() << " | "
                              << std::setw(12) << ps.getModel() << " | "
                              << std::setw(12) << ps.getBrand() << " | "
                              << std::setw(10) << addComma(ps.getPrice()) << " | "
                              << std::setw(5)  << addComma(ps.getStock()) << "\n";
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

const PowerSupply* PowerSupplyManager::findByCode(const std::string& code) const{
    for (auto& ps : powersupplies) {
        if (ps.getCode() == code)
            return &ps;
    }
    return nullptr;
}

PowerSupply* PowerSupplyManager::findByCode(const std::string& code) {
    for (auto& ps : powersupplies) {
        if (ps.getCode() == code)
            return &ps;
    }
    return nullptr;
}