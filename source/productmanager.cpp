#include "productmanager.h"
#include <iostream>

namespace ProductSystem { // ProductSystem 네임스페이스 안에 정의된 ProductManager 클래스의 구현부
    // 내부적으로 각 제품군 매니저 객체를 직접 멤버로 가지고 있음, 이걸 통해 모든 제품 저장, 로딩, 메뉴 제어를 담당
ProductManager::ProductManager() {
    loadFromFile();
} // 객체가 생성되면 즉시 loadFromFile() 호출 → 프로그램 시작 시 파일에서 기존 데이터를 자동 로딩

ProductManager::~ProductManager() {
    saveToFile();
} // 객체가 소멸되기 직전에 모든 제품 정보를 저장 → 프로그램 종료 시 자동 저장 보장

void ProductManager::saveToFile() { // 각 제품군별 매니저에게 saveToFile() 요청
    guitarManager.saveToFile();
    effectManager.saveToFile();
    tunerManager.saveToFile();
    ampManager.saveToFile();
    powerSupplyManager.saveToFile();
    cableManager.saveToFile();
    accessoryManager.saveToFile();
} // 각 매니저는 자신에게 해당하는 txt 파일로 제품 데이터를 저장함

void ProductManager::loadFromFile() { // 프로그램 실행 시 각 제품군별 txt 파일을 로딩
    guitarManager.loadFromFile();
    effectManager.loadFromFile();
    tunerManager.loadFromFile();
    ampManager.loadFromFile();
    powerSupplyManager.loadFromFile();
    cableManager.loadFromFile();
    accessoryManager.loadFromFile();
} // 모든 제품 데이터가 ProductManager 안의 하위 매니저들에 메모리로 적재됨

bool ProductManager::displayMenu() { // 제품관리 메인 메뉴 출력
    int choice;
    std::cout << "\n========= 제품 관리 =========\n";
    std::cout << "1. 일렉기타 관리\n";
    std::cout << "2. 이펙터 관리\n";
    std::cout << "3. 튜너 관리\n";
    std::cout << "4. 앰프 관리\n";
    std::cout << "5. 파워서플라이 관리\n";
    std::cout << "6. 케이블 관리\n";
    std::cout << "7. 액세서리 관리\n";
    std::cout << "8. 메인 메뉴로 돌아가기\n"; // 8은 상위 메뉴로 돌아가기
    std::cout << "선택: ";
    std::cin >> choice; // 사용자 입력을 받아 각 제품군별 메뉴로 분기

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
            std::cout << "\n잘못된 선택입니다.\n";
            return true;
    } // 각 제품군별 displayMenu() 호출 → 내부에서 추가/삭제/검색 등의 메뉴 제공
} // 잘못된 번호 입력 시 안내 메시지 후 재진입 (return true)
}