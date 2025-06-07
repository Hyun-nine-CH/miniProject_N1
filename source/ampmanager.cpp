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

void AmpManager::saveToFile() const { // const는 이 함수가 AmpManager의 멤버 값을 수정하지 않음 (불변 메서드)
    std::ofstream file(AMP_FILE); // 파일에 데이터를 쓰기 위한 파일 스트림 생성, 기존 파일이 있으면 덮어쓰고 없으면 새로 생성
    for (const auto& a : amps) { // amps는 Amp 객체들의 std::vector<Amp>, 범위 기반 for문으로, 벡터의 각 요소를 읽기 전용으로(const auto&) 순회
        file << a.getCode() << ","
             << a.getModel() << ","
             << a.getBrand() << ","
             << a.getPrice() << "," // CSV 형식으로 각 앰프의 속성을 쉼표로 구분하여 파일에 저장
             << a.getStock() << "\n";
    }
}

void AmpManager::inputAmp() { // 반환값 없이(void) 사용자로부터 새 앰프 정보를 입력받음
    std::string code, model, brand;
    int price, stock;

    std::cin.ignore(); // 버퍼 초기화, std::cin을 사용한 이후 줄바꿈 문자(\n)가 남아 있으면 getline()에 영향을 주므로 제거함.
    std::cout << "코드: ";
    std::getline(std::cin, code); // getline()은 공백을 포함한 전체 줄을 문자열로 읽기

    std::cout << "모델명: ";
    std::getline(std::cin, model);

    std::cout << "브랜드: ";
    std::getline(std::cin, brand);

    std::cout << "가격: ";
    std::cin >> price; // 가격은 int 타입이므로 std::cin을 이용해 숫자 입력
    std::cin.ignore(); // 버퍼 초기화, cin >> price 이후 남아 있는 \n 제거 → 아래 cin >> stock이나 이후 getline() 사용 대비

    std::cout << "재고: ";
    std::cin >> stock;
    std::cin.ignore(); // 버퍼 초기화, 마지막에 안정성을 높이기 위해 추가 버퍼 정리

    amps.emplace_back(code, model, brand, price, stock);
    saveToFile(); // 입력된 내용을 반영하여 전체 amps 목록을 다시 파일로 저장
}

void AmpManager::deleteAmp() {
    std::string code;
    std::cout << "삭제할 코드: ";
    std::cin >> code;

    auto it = std::remove_if(amps.begin(), amps.end(),
        [&](const Amp& a) { return a.getCode() == code; });
    // STL 알고리즘 std::remove_if()를 사용하여 조건에 맞는 요소를 논리적으로 제거
    // amps.begin(), amps.end() → amps 벡터의 전체 구간을 순회 대상으로 지정
    // [&](const Amp& a) { return a.getCode() == code; } → 람다 함수로, 각 Amp 객체의 코드가 입력된 code와 일치하는지 검사
    // [&] → 현재 스코프(외부)의 code 변수를 람다 함수에서 참조할 수 있도록 함, const Amp& a로 각 Amp 객체를 참조
    // const Amp& a → 각 Amp 객체를 참조하며 순회
    // return a.getCode() == code; → 입력된 code와 일치하는 경우를 삭제 대상으로 지정
    // std::remove_if()는 실제로 벡터에서 요소를 제거하지 않고, 삭제할 요소들을 뒤로 밀어내고, 새로운 끝 위치 반복자를 반환

    if (it != amps.end()) { // remove_if 결과가 amps.end()와 다르다면 ⇒ 삭제할 항목이 최소 1개는 있었다는 뜻
        amps.erase(it, amps.end()); // it부터 amps.end()까지 범위의 요소를 실제로 제거, 범위는 remove_if가 뒤로 밀어놓은 삭제 대상
        std::cout << "삭제되었습니다.\n";
        saveToFile();
    } else {
        std::cout << "해당 코드를 찾을 수 없습니다.\n";
    }
}

bool AmpManager::displayMenu() { // 무한 루프 기반 인터페이스, AmpManager 클래스의 멤버 함수이며, 반환 타입은 bool
    int choice; // 사용자 입력을 저장할 정수 변수 선언, 메뉴 번호를 입력받는 데 사용
    while (true) { // true를 반환하여 호출한 쪽(ProductManager 등)으로 돌아갈 수 있는 구조
        std::cout << "\n--- 앰프 관리 ---\n";
        std::cout << "1. 등록\n";
        std::cout << "2. 삭제\n";
        std::cout << "3. 전체 목록 보기\n";
        std::cout << "4. 이전 메뉴로 돌아가기\n"; // 무한 루프를 통해 사용자가 4번(되돌아가기)을 선택하기 전까지 반복 실행, 내부에서 return이 나올 때까지 무한 반복
        std::cout << "선택: ";
        std::cin >> choice; // std::cin >> choice는 콘솔로부터 정수를 읽음

        switch (choice) { // 사용자 선택값에 따라 실행 분기를 나눔
            case 1: inputAmp(); break; // 1. 등록 → 앰프 등록 함수 inputAmp() 호출 후 루프 계속
            case 2: deleteAmp(); break; // 2. 삭제 → 앰프 삭제 함수 deleteAmp() 호출 후 루프 계속
            case 3: displayAmp(); break; // 3. 전체 목록 보기 → displayAmp() 호출로 전체 앰프 출력 및 검색 기능 제공
            case 4: return true; // ProductManager로 돌아가기 (4. 이전 메뉴 → 함수 종료(true 반환) → 상위 메뉴로 되돌아감)
            default: std::cout << "잘못된 선택입니다.\n"; break; // 그 외의 숫자 입력 시, 잘못된 입력 메시지를 출력하고 루프 재시작
        }
    }
    return true; // while (true) 내부의 case 4에서 이미 return하기 때문에 여긴 사실상 도달 불가
}

void AmpManager::displayAmp() const { // const 지정 → 이 함수는 amps 벡터를 읽기만 하고, 수정하지 않음
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