#include <vector>
#include <algorithm> // C++ STL(Standard Template Library)에서 정렬, 검색, 수치 연산 등 다양한 범용 알고리즘 함수들을 제공하는 헤더 파일
#include <fstream>
#include <sstream>
#include <iomanip> //  C++에서 입출력 포맷 조절을 위한 헤더 파일
#include <iostream>

#include "client.h"
#include "clientmanager.h"

namespace ClientSystem {
// ClientSystem 네임스페이스 안에 ClientManager 클래스 정의
// 다수의 파일과 클래스가 얽혀 있을 때 유지보수에 유리함

const std::string CLIENT_FILE = "data/clientlist.txt";

inline std::string formatPhoneNumber(const std::string& phone) {
    if (phone.length() == 11) {
        return phone.substr(0, 3) + "-" + phone.substr(3, 4) + "-" + phone.substr(7, 4);
    } // substr(start, length) → start 위치부터 length 길이만큼 잘라서 반환
    return phone;
} // 11자리 번호를 010-XXXX-XXXX 형식으로 변환
// inline은 짧고 자주 호출되는 함수의 호출 비용을 줄이기 위해, 컴파일러에게 해당 함수의 본문을 함수 호출 부분에 직접 삽입하도록 지시하는 키워드
// namespace 안에서 정의한 작은 함수에서 자주 사용

ClientManager::ClientManager() { // 프로그램 시작 시 clientlist.txt 파일을 열고 고객 목록을 불러옴
    std::ifstream file(CLIENT_FILE);
    if (!file.fail()) {
        while (!file.eof()) {
            std::vector<std::string> row = parseCSV(file, ',');
            if (row.size() == 4) {
                int id = std::stoi(row[0]);
                Client* c = new Client(id, row[1], row[2], row[3]);
                clientList.insert({ id, c });
            } // parseCSV를 이용해 파싱 후 Client 객체 생성 및 clientList map에 저장
        }
    }
}

ClientManager::~ClientManager() {
    saveToFile();  // 종료 시 자동 저장
}

void ClientManager::saveToFile() { // clientList의 내용을 clientlist.txt에 저장
    std::ofstream file(CLIENT_FILE);
    if (file.is_open()) {
        for (const auto& v : clientList) {
            Client* c = v.second;
            file << c->id() << ", " << c->getName() << ", "
                 << c->getPhoneNumber() << ", "
                 << c->getAddress() << '\n';
        } // map<int, Client*> 형식으로 되어 있으므로 반복문으로 순회 후 파일에 기록
    }
}

void ClientManager::inputClient() { // 사용자로부터 이름, 번호, 주소 입력을 받아 새로운 고객 객체 생성
    std::string name, number, address;
    std::cout << "name : "; std::cin >> name;
    std::cout << "number : "; std::cin >> number;
    std::cout << "address : "; std::cin.ignore(); std::getline(std::cin, address);

    int id = makeId(); // 고유 ID는 makeId()로 자동 생성, 가입 순으로 진행
    Client* c = new Client(id, name, number, address);
    clientList.insert({ id, c });

    saveToFile(); // clientList에 추가한 뒤 즉시 저장
}

void ClientManager::deleteClient(int key) {
    if (clientList.find(key) == clientList.end()) {
        std::cerr << "존재하지 않는 고객입니다. 다시 입력하세요.\n";
        return;
    } // 고객 ID를 받아 삭제. 존재하지 않을 경우 에러 메시지 출력
    clientList.erase(key); // map에서 해당 ID를 찾아 삭제
    saveToFile();
}

void ClientManager::modifyClient(int key) { // 특정 고객 ID에 해당하는 객체를 찾아 정보 수정
    Client* c = findClientById(key);
    if (!c) {
        std::cerr << "존재하지 않는 고객입니다. 다시 입력하세요.\n";
        return;
    }

    std::cout << "  ID  |   Name   | Phone Number |   Address\n";
    std::cout << std::setw(5) << std::setfill('0') << std::right << c->id() << " | " << std::left;
    std::cout << std::setw(12) << std::setfill(' ') << c->getName() << " | ";
    std::cout << std::setw(12) << c->getPhoneNumber() << " | ";
    std::cout << c->getAddress() << "\n";

    std::string name, number, address;
    std::cout << "name : "; std::cin >> name;
    std::cout << "number : "; std::cin >> number;
    std::cout << "address : "; std::cin.ignore(); std::getline(std::cin, address);
    // 사용자에게 기존 정보 출력 후 새 값 입력

    c->setName(name);
    c->setPhoneNumber(number);
    c->setAddress(address); // 최신화

    saveToFile(); // 이후 즉시 저장
}

Client* ClientManager::findClientById(int id) const{ // map을 탐색해 ID로 검색
    auto it = clientList.find(id);
    if (it != clientList.end()) return it->second;
    return nullptr; // 없으면 nullptr 반환
}

Client* ClientManager::findClientByName(const std::string& name) const{ // 선형 탐색으로 이름 일치하는 객체 탐색
    for (const auto& pair : clientList) {
        if (pair.second->getName() == name)
            return pair.second;
    }
    return nullptr;
}

bool ClientManager::displayMenu() {
    while (true) {
        int choice;
        std::cout << "\n--- 고객 관리 ---\n";
        std::cout << "1. 고객 목록 보기\n";
        std::cout << "2. 고객 등록\n";
        std::cout << "3. 고객 삭제\n";
        std::cout << "4. 고객 정보 수정\n";
        std::cout << "5. 이전 메뉴로 돌아가기\n";
        std::cout << "선택: ";
        std::cin >> choice;
        std::cin.ignore(1000, '\n'); // 충분하게 버퍼를 비워줌, 충분히 큰 M을 1000으로 임의 설정

        switch (choice) {
            case 1:
                displayInfo(); // 고객 정보 보기
                break;
            case 2:
                inputClient(); // 고객 등록
                break;
            case 3: {
                displayInfo();
                int key;
                std::cout << "   Choose Key : ";
                std::cin >> key; // 삭제할 고객의 ID 입력
                std::cin.ignore(1000, '\n'); // 사용자 입력 후 남아 있는 개행 문자(\n)를 제거해서 다음 입력이 꼬이지 않도록 함
                deleteClient(key);
                break;
            }
            case 4: {
                displayInfo();
                int key;
                std::cout << "   Choose Key : ";
                std::cin >> key;
                std::cin.ignore(1000, '\n');
                modifyClient(key); // 고객 정보 수정
                break;
            }
            case 5:
                return false;
            default:
                std::cout << "잘못된 선택입니다.\n";
                break;
        }
    }
    return true;
}

void ClientManager::displayInfo() { // 전체 고객 목록 출력
    while (true) {
        std::cout << "\n  ID  |   Name    | Phone Number |   Address\n";
        std::cout << "---------------------------------------------\n";
        for (const auto& v : clientList) { // clientList는 std::map<int, Client*> 타입
            // 키(int형 ID)와 값(Client* 포인터)의 쌍(pair)으로 구성된 자료구조
            // v는 각 std::pair<int, Client*>를 의미
            // auto&는 참조를 의미하고, const는 수정하지 않겠다는 뜻
            // v.first는 고객 ID (int), v.second는 고객 객체를 가리키는 포인터 (Client*)

            Client* c = v.second;
            // 해당 고객 객체의 포인터를 c라는 변수로 꺼내어 고객 정보에 접근
            std::cout << std::setw(5) << std::setfill('0') << std::right << c->id() << " | " << std::left;
            std::cout << std::setw(12) << std::setfill(' ') << c->getName() << " | ";
            std::cout << std::setw(12) << formatPhoneNumber(c->getPhoneNumber()) << " | ";
            std::cout << c->getAddress() << "\n";
        } // 테이블 형식 (setw, setfill)으로 정렬
        // std::setw(n)	→ n칸 너비 확보
        // std::setfill(char) → 남는 칸 채울 문자
        // std::right, std::left → 오른쪽 정렬, 왼쪽 정렬

        std::cout << "\n========= 키워드 검색 =========\n";
        std::cout << "1. 검색하기\n";
        std::cout << "2. 되돌아가기\n";
        std::cout << "선택: ";

        int opt;
        std::cin >> opt;
        std::cin.ignore(1000, '\n');

        if (opt == 1) {
            std::string keyword;
            std::cout << "검색할 키워드 입력: ";
            std::getline(std::cin, keyword);

            std::cout << "\n[검색 결과]\n";
            std::cout << "  ID  |   Name    | Phone Number |   Address\n";
            std::cout << "---------------------------------------------\n";

            bool found = false; // 기본 상태를 false로 설정
            for (const auto& v : clientList) {
                Client* c = v.second;
                if (std::to_string(c->id()).find(keyword) != std::string::npos ||
                    c->getName().find(keyword) != std::string::npos ||
                    c->getPhoneNumber().find(keyword) != std::string::npos ||
                    c->getAddress().find(keyword) != std::string::npos) {

                    // std::string::npos는 "찾을 수 없는 위치"를 의미하는 특수한 상수
                    // std::size_t 타입의 최대값을 의미하며, find() 함수 등에서 검색 실패를 나타낼 때 사용
                    // std::string::find() 함수는 찾지 못했을 때 npos를 반환
                    // std::size_t의 최대값은 대략 2^(64) - 1이며, 이는 64비트 시스템에서 약 18,446,744,073,709,551,615에 해당
                    // index로 나올 수 없는 값이므로, 이를 find() 함수에서 npos로 반환

                    std::cout << std::setw(5) << std::setfill('0') << std::right << c->id() << " | " << std::left;
                    std::cout << std::setw(12) << std::setfill(' ') << c->getName() << " | ";
                    std::cout << std::setw(12) << formatPhoneNumber(c->getPhoneNumber()) << " | ";
                    std::cout << c->getAddress() << "\n";
                    found = true;
                }
            } // 찾았으면, 찾은 고객의 정보를 출력, 기본 상태에서 true로 찾았음을 표시

            if (!found) { // true가 아니면, 즉 찾은 고객이 없으면 아래의 문구 출력
                std::cout << "일치하는 항목이 없습니다.\n";
            }

            std::cout << "\n확인을 했다면 Enter 키를 입력하세요";
            std::cin.get(); // 여기서도 입력 대기용 버퍼 비우기 설정
            // 사용자가 키보드를 입력하지 않는 한, 멈춰 있는 상황이 오고, 한 문자를 치면, 바로 어디론가 이동하는, 현재 프로그램만 종료하는 느낌으로 기폭제 역할을 함
        } else if (opt == 2) {
            return; // 상위 메뉴로 돌아감
        } else {
            std::cout << "잘못된 선택입니다.\n";
        }
    }
}

void ClientManager::addClient(Client* c) { // Client* c → 새로운 고객 객체의 포인터
    clientList.insert({ c->id(), c });
    // clientList → std::map<int, Client*> 형식의 맵으로,
    // c->id() → 이 고객의 고유 ID를 가져옴, 그 후, insert({ c->id(), c }) → ID를 키로, 포인터를 값으로 넣음
    saveToFile();
}

int ClientManager::makeId() { // 고객 ID는 자동 증가 방식 (고객 입력 정보에 별도의 ID 입력 없는 것을 알 수 있음)
    if (clientList.empty()) {
        return 1;
    } else {
        auto it = clientList.rbegin();
        // std::map은 기본적으로 key 정렬이 오름차순 (가장 작은 key가 앞에 오고, 가장 큰 key가 뒤에 오도록 정렬됨)
        // rbegin()은 가장 큰 key (최근 고객 ID)에 접근하는 역방향 반복자 reverse iterator로써 가장 큰 ID를 지정함 (이말은 즉, 맨 뒤의 값)
        // reverse iterator, 나름 고급 용어라 생각함

        return it->first + 1; // it->first → 가장 큰 ID + 1 → 새 ID 생성
    }
}

// CSV 파일의 각 줄을 안전하게 파싱해서 문자열 벡터로 반환하는 함수
std::vector<std::string> ClientManager::parseCSV(std::istream& file, char delimiter) {
    // delimiter → 구분자 (예: ',' 또는 '\t')
    std::string line;
    std::vector<std::string> row;
    // row → 결과를 담을 벡터 (한 줄이 하나의 벡터로 분할됨)

    if (std::getline(file, line)) { // 한 줄 전체를 읽어옴 (\n 기준)
        std::stringstream ss(line); // ss → 읽어온 줄을 셀 단위로 나누기 위해 사용
        std::string cell; // cell → 분할된 각 항목을 임시 저장

        while (std::getline(ss, cell, delimiter)) {
            // std::getline(ss, cell, delimiter) → 구분자 기준으로 자름
            size_t start = cell.find_first_not_of(" \n\r\t");
            size_t end = cell.find_last_not_of(" \n\r\t");
            // find_first_not_of, find_last_not_of → 앞뒤 공백 제거
            if (start != std::string::npos && end != std::string::npos)
                row.push_back(cell.substr(start, end - start + 1));
                // substr(...) → 실제로 공백을 제외한 텍스트만 추출
            else
                row.push_back(""); // row.push_back(...) → 결과 벡터에 ...추가; 공백만 있는 경우, 공백 추가
        }
    }

    return row;
}
}