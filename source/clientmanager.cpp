#include <vector>
#include <algorithm> // C++ STL(Standard Template Library)에서 정렬, 검색, 수치 연산 등 다양한 범용 알고리즘 함수들을 제공하는 헤더 파일
#include <fstream>
#include <sstream>
#include <iomanip> //  C++에서 입출력 포맷 조절을 위한 헤더 파일
#include <iostream>

#include "client.h"
#include "clientmanager.h"

namespace ClientSystem {
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
                displayInfo();
                break;
            case 2:
                inputClient();
                break;
            case 3: {
                displayInfo();
                int key;
                std::cout << "   Choose Key : ";
                std::cin >> key;
                std::cin.ignore(1000, '\n');
                deleteClient(key);
                break;
            }
            case 4: {
                displayInfo();
                int key;
                std::cout << "   Choose Key : ";
                std::cin >> key;
                std::cin.ignore(1000, '\n');
                modifyClient(key);
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

void ClientManager::displayInfo() {
    while (true) {
        std::cout << "\n  ID  |   Name    | Phone Number |   Address\n";
        std::cout << "---------------------------------------------\n";
        for (const auto& v : clientList) {
            Client* c = v.second;
            std::cout << std::setw(5) << std::setfill('0') << std::right << c->id() << " | " << std::left;
            std::cout << std::setw(12) << std::setfill(' ') << c->getName() << " | ";
            std::cout << std::setw(12) << formatPhoneNumber(c->getPhoneNumber()) << " | ";
            std::cout << c->getAddress() << "\n";
        }

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

            bool found = false;
            for (const auto& v : clientList) {
                Client* c = v.second;
                if (std::to_string(c->id()).find(keyword) != std::string::npos ||
                    c->getName().find(keyword) != std::string::npos ||
                    c->getPhoneNumber().find(keyword) != std::string::npos ||
                    c->getAddress().find(keyword) != std::string::npos) {

                    std::cout << std::setw(5) << std::setfill('0') << std::right << c->id() << " | " << std::left;
                    std::cout << std::setw(12) << std::setfill(' ') << c->getName() << " | ";
                    std::cout << std::setw(12) << formatPhoneNumber(c->getPhoneNumber()) << " | ";
                    std::cout << c->getAddress() << "\n";
                    found = true;
                }
            }

            if (!found) {
                std::cout << "일치하는 항목이 없습니다.\n";
            }

            std::cout << "\n확인을 했다면 Enter 키를 입력하세요";
            std::cin.get();
        } else if (opt == 2) {
            return;
        } else {
            std::cout << "잘못된 선택입니다.\n";
        }
    }
}

void ClientManager::addClient(Client* c) {
    clientList.insert({ c->id(), c });
    saveToFile();
}

int ClientManager::makeId() {
    if (clientList.empty()) {
        return 1;
    } else {
        auto it = clientList.rbegin(); // reverse iterator로 가장 큰 ID
        return it->first + 1;
    }
}

std::vector<std::string> ClientManager::parseCSV(std::istream& file, char delimiter) {
    std::string line;
    std::vector<std::string> row;

    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, delimiter)) {
            size_t start = cell.find_first_not_of(" \n\r\t");
            size_t end = cell.find_last_not_of(" \n\r\t");
            if (start != std::string::npos && end != std::string::npos)
                row.push_back(cell.substr(start, end - start + 1));
            else
                row.push_back(""); // 공백만 있는 경우
        }
    }

    return row;
}
}