#include "productmanager.h"
#include <iostream>

namespace ProductSystem {
ProductManager::ProductManager() {
    loadFromFile();
}

ProductManager::~ProductManager() {
    saveToFile();
}

void ProductManager::saveToFile() {
    guitarManager.saveToFile();
    effectManager.saveToFile();
    tunerManager.saveToFile();
    ampManager.saveToFile();
    powerSupplyManager.saveToFile();
    cableManager.saveToFile();
    accessoryManager.saveToFile();
}

void ProductManager::loadFromFile() {
    guitarManager.loadFromFile();
    effectManager.loadFromFile();
    tunerManager.loadFromFile();
    ampManager.loadFromFile();
    powerSupplyManager.loadFromFile();
    cableManager.loadFromFile();
    accessoryManager.loadFromFile();
}

bool ProductManager::displayMenu() {
    int choice;
    std::cout << "\n========= 제품 관리 =========\n";
    std::cout << "1. 일렉기타 관리\n";
    std::cout << "2. 이펙터 관리\n";
    std::cout << "3. 튜너 관리\n";
    std::cout << "4. 앰프 관리\n";
    std::cout << "5. 파워서플라이 관리\n";
    std::cout << "6. 케이블 관리\n";
    std::cout << "7. 액세서리 관리\n";
    std::cout << "8. 메인 메뉴로 돌아가기\n";
    std::cout << "선택: ";
    std::cin >> choice;

    switch (choice) {
        case 1: return guitarManager.displayMenu();
        case 2: return effectManager.displayMenu();
        case 3: return tunerManager.displayMenu();
        case 4: return ampManager.displayMenu();
        case 5: return powerSupplyManager.displayMenu();
        case 6: return cableManager.displayMenu();
        case 7: return accessoryManager.displayMenu();
        case 8: return false;
        default:
            std::cout << "잘못된 선택입니다.\n";
            return true;
    }
}
}