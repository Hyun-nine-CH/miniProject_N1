#include "ampmanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

const std::string AMP_FILE = "data/pdList_amps.txt";

inline std::string addComma(int number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << std::fixed << number;
    return oss.str();
}

AmpManager::AmpManager() {
    loadFromFile();
}

void AmpManager::loadFromFile() {
    amps.clear();
    std::ifstream file(AMP_FILE);
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

        amps.emplace_back(code, model, brand, price, stock);
    }
}

void AmpManager::saveToFile() const {
    std::ofstream file(AMP_FILE);
    for (const auto& a : amps) {
        file << a.getCode() << ","
             << a.getModel() << ","
             << a.getBrand() << ","
             << a.getPrice() << ","
             << a.getStock() << "\n";
    }
}

void AmpManager::inputAmp() {
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

    amps.emplace_back(code, model, brand, price, stock);
    saveToFile();
}

void AmpManager::deleteAmp() {
    std::string code;
    std::cout << "삭제할 코드: ";
    std::cin >> code;

    auto it = std::remove_if(amps.begin(), amps.end(),
        [&](const Amp& a) { return a.getCode() == code; });

    if (it != amps.end()) {
        amps.erase(it, amps.end());
        std::cout << "삭제되었습니다.\n";
        saveToFile();
    } else {
        std::cout << "해당 코드를 찾을 수 없습니다.\n";
    }
}

bool AmpManager::displayMenu() {
    int choice;
    while (true) {
        std::cout << "\n--- 앰프 관리 ---\n";
        std::cout << "1. 등록\n";
        std::cout << "2. 삭제\n";
        std::cout << "3. 전체 목록 보기\n";
        std::cout << "4. 이전 메뉴로 돌아가기\n";
        std::cout << "선택: ";
        std::cin >> choice;

        switch (choice) {
            case 1: inputAmp(); break;
            case 2: deleteAmp(); break;
            case 3: displayAmp(); break;
            case 4: return true; // ProductManager로 돌아가기
            default: std::cout << "잘못된 선택입니다.\n"; break;
        }
    }
    return true;
}

void AmpManager::displayAmp() const {
    while (true) {
        std::cout << "\n코드       | 모델명         | 브랜드     | 가격       | 재고\n";
        std::cout << "------------------------------------------------------------\n";
        for (const auto& a : amps) {
            std::cout << std::setw(10) << a.getCode() << " | "
                      << std::setw(12) << a.getModel() << " | "
                      << std::setw(12) << a.getBrand() << " | "
                      << std::setw(10) << addComma(a.getPrice()) << " | "
                      << std::setw(5) << addComma(a.getStock()) << "\n";
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
            for (const auto& a : amps) {
                if (a.getModel().find(keyword) != std::string::npos ||
                    a.getBrand().find(keyword) != std::string::npos ||
                    a.getCode().find(keyword) != std::string::npos) {
                    std::cout << std::setw(10) << a.getCode() << " | "
                              << std::setw(12) << a.getModel() << " | "
                              << std::setw(12) << a.getBrand() << " | "
                              << std::setw(10) << addComma(a.getPrice()) << " | "
                              << std::setw(5)  << addComma(a.getStock()) << "\n";
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

const Amp* AmpManager::findByCode(const std::string& code) const{
    for (auto& a : amps) {
        if (a.getCode() == code)
            return &a;
    }
    return nullptr;
}

Amp* AmpManager::findByCode(const std::string& code) {
    for (auto& a : amps) {
        if (a.getCode() == code)
            return &a;
    }
    return nullptr;
}