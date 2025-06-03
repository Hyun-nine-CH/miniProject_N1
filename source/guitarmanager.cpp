#include "guitarmanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

const std::string GUITAR_FILE = "data/pdList_guitars.txt";

inline std::string addComma(int number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << std::fixed << number;
    return oss.str();
}

GuitarManager::GuitarManager() {
    loadFromFile();
}

void GuitarManager::loadFromFile() {
    guitars.clear();
    std::ifstream file(GUITAR_FILE);
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string code, model, brand, type;
        int price, stock;

        getline(ss, code, ',');
        getline(ss, model, ',');
        getline(ss, brand, ',');
        getline(ss, type, ',');
        ss >> price; ss.ignore();
        ss >> stock;

        guitars.emplace_back(code, model, brand, type, price, stock);
    }
}

void GuitarManager::saveToFile() const {
    std::ofstream file(GUITAR_FILE);
    for (const auto& g : guitars) {
        file << g.getCode() << ","
             << g.getModel() << ","
             << g.getBrand() << ","
             << g.getType() << ","
             << g.getPrice() << ","
             << g.getStock() << "\n";
    }
}

void GuitarManager::inputGuitar() {
    std::string code, model, brand, type;
    int price, stock;

    std::cin.ignore(); // 버퍼 초기화
    std::cout << "코드: ";
    std::getline(std::cin, code);

    std::cout << "모델명: ";
    std::getline(std::cin, model);

    std::cout << "브랜드: ";
    std::getline(std::cin, brand);

    std::cout << "타입: ";
    std::getline(std::cin, type);

    std::cout << "가격: ";
    std::cin >> price;
    std::cin.ignore(); // 버퍼 초기화

    std::cout << "재고: ";
    std::cin >> stock;
    std::cin.ignore(); // 버퍼 초기화

    guitars.emplace_back(code, model, brand, type, price, stock);
    saveToFile();
}

void GuitarManager::deleteGuitar() {
    std::string code;
    std::cout << "삭제할 코드: ";
    std::cin >> code;

    auto it = std::remove_if(guitars.begin(), guitars.end(),
        [&](const Guitar& g) { return g.getCode() == code; });

    if (it != guitars.end()) {
        guitars.erase(it, guitars.end());
        std::cout << "삭제되었습니다.\n";
        saveToFile();
    } else {
        std::cout << "해당 코드를 찾을 수 없습니다.\n";
    }
}

bool GuitarManager::displayMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 일렉기타 관리 ---\n";
        std::cout << "1. 등록\n";
        std::cout << "2. 삭제\n";
        std::cout << "3. 전체 목록 보기\n";
        std::cout << "4. 이전 메뉴로 돌아가기\n";
        std::cout << "선택: ";
        std::cin >> choice;

        switch (choice) {
            case 1: inputGuitar(); break;
            case 2: deleteGuitar(); break;
            case 3: displayGuitar(); break;
            case 4: return true; // ProductManager로 돌아가기
            default: std::cout << "잘못된 선택입니다.\n"; break;
        }
    }
    return true;
}

void GuitarManager::displayGuitar() const {
    while (true) {
        std::cout << "\n코드       | 모델명           | 브랜드       | 타입     | 가격       | 재고\n";
        std::cout << "--------------------------------------------------------------------------\n";
        for (const auto& g : guitars) {
            std::cout << std::setw(12) << g.getCode() << " | "
                      << std::setw(16) << g.getModel() << " | "
                      << std::setw(12) << g.getBrand() << " | "
                      << std::setw(8)  << g.getType() << " | "
                      << std::setw(10) << addComma(g.getPrice()) << " | "
                      << std::setw(5)  << addComma(g.getStock()) << "\n";
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
            std::cout << "\n코드       | 모델명           | 브랜드       | 타입     | 가격       | 재고\n";
            std::cout << "--------------------------------------------------------------------------\n";

            bool found = false;
            for (const auto& g : guitars) {
                if (g.getModel().find(keyword) != std::string::npos ||
                    g.getBrand().find(keyword) != std::string::npos ||
                    g.getType().find(keyword) != std::string::npos ||
                    g.getCode().find(keyword) != std::string::npos) {
                    std::cout << std::setw(12) << g.getCode() << " | "
                              << std::setw(16) << g.getModel() << " | "
                              << std::setw(12) << g.getBrand() << " | "
                              << std::setw(8)  << g.getType() << " | "
                              << std::setw(10) << addComma(g.getPrice()) << " | "
                              << std::setw(5)  << addComma(g.getStock()) << "\n";
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

const Guitar* GuitarManager::findByCode(const std::string& code) const{
    for (auto& g : guitars) {
        if (g.getCode() == code)
            return &g;
    }
    return nullptr;
}

Guitar* GuitarManager::findByCode(const std::string& code) {
    for (auto& g : guitars) {
        if (g.getCode() == code)
            return &g;
    }
    return nullptr;
}