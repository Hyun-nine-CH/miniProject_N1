#include "ampmanager.h"
#include <iostream>
#include <fstream> // 읽기 및 쓰기를 위한 파일 스트림
#include <sstream> // 문자열 스트림을 사용하여 파일에서 데이터를 읽고 쓸 때 유용
#include <iomanip> // 출력 형식을 제어하는 조정자(manipulator) 제공하고 너비, 정렬, 고정소수점, 채우기 문자 설정 등에 사용

const std::string AMP_FILE = "data/pdList_amps.txt";

inline std::string addComma(int number) {
    std::ostringstream oss; // ostringstream은 문자열에 데이터를 포맷팅하여 저장할 때 사용
    oss.imbue(std::locale(""));
    // imbue()는 스트림(std::ostringstream, std::cin, std::cout 등)에 새로운 지역(locale)을 설정하는 함수
    // std::locale("")는 현재 시스템의 로케일을 사용하여 숫자 형식(예: 천 단위 구분 기호)을 설정
    oss << std::fixed << number;
    // 여기서 fixed는 double 또는 float과 같은 부동소수점 숫자가 과학적 표기법이 아닌 소수점 이하 자릿수가 고정된 일반적인 표기법으로 출력하게 하는 함수
    return oss.str();
}

AmpManager::AmpManager() {
    loadFromFile();
} // 생성자에서 파일을 불러와 초기화를 진행

void AmpManager::loadFromFile() {
    amps.clear(); // 파일로부터 앰프 데이터를 불러와 amps 벡터 초기화
    std::ifstream file(AMP_FILE);
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line); // 텍스트 파일에 대한 한 줄을 읽은 후, 문자열 스트림을 사용하여 데이터를 파싱
        std::string code, model, brand;
        int price, stock;

        getline(ss, code, ','); // 그 구분을 쉼표(,)로 문자열로 읽음
        getline(ss, model, ',');
        getline(ss, brand, ',');
        ss >> price; ss.ignore(); // stringstream ss에서 정수 price 값을 추출, ss.ignore();는 다음 입력을 위해 버퍼를 초기화
        ss >> stock;
        
        amps.emplace_back(code, model, brand, price, stock);
        // emplace_back()은 C++11부터 도입된 std::vector의 멤버 함수로, 객체를 벡터의 맨 뒤에 "직접 생성"하여 추가하는 함수
        /*
        push_back(obj) : 이미 생성된 객체를 추가 (복사/이동 비용 발생 가능)
        emplace_back(args...) : 생성자 인자만 넘겨서 객체를 벡터 내부에서 직접 생성
        따라서 클래스 객체 생성 시, 복사/이동 비용을 줄이고 싶다면 emplace_back을 사용하는 것이 더 좋아서 사용
        */
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