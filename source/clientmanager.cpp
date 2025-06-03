#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "client.h"
#include "clientmanager.h"

namespace ClientSystem {
const std::string CLIENT_FILE = "data/clientlist.txt";

inline std::string formatPhoneNumber(const std::string& phone) {
    if (phone.length() == 11) {
        return phone.substr(0, 3) + "-" + phone.substr(3, 4) + "-" + phone.substr(7, 4);
    }
    return phone;
}

ClientManager::ClientManager() {
    std::ifstream file(CLIENT_FILE);
    if (!file.fail()) {
        while (!file.eof()) {
            std::vector<std::string> row = parseCSV(file, ',');
            if (row.size() == 4) {
                int id = std::stoi(row[0]);
                Client* c = new Client(id, row[1], row[2], row[3]);
                clientList.insert({ id, c });
            }
        }
    }
}

ClientManager::~ClientManager() {
    saveToFile();  // 종료 시 자동 저장
}

void ClientManager::saveToFile() {
    std::ofstream file(CLIENT_FILE);
    if (file.is_open()) {
        for (const auto& v : clientList) {
            Client* c = v.second;
            file << c->id() << ", " << c->getName() << ", "
                 << c->getPhoneNumber() << ", "
                 << c->getAddress() << '\n';
        }
    }
}

void ClientManager::inputClient() {
    std::string name, number, address;
    std::cout << "name : "; std::cin >> name;
    std::cout << "number : "; std::cin >> number;
    std::cout << "address : "; std::cin.ignore(); std::getline(std::cin, address);

    int id = makeId();
    Client* c = new Client(id, name, number, address);
    clientList.insert({ id, c });

    saveToFile(); // 바로 저장
}

void ClientManager::deleteClient(int key) {
    if (clientList.find(key) == clientList.end()) {
        std::cerr << "존재하지 않는 고객입니다. 다시 입력하세요.\n";
        return;
    }
    clientList.erase(key);
    saveToFile();
}

void ClientManager::modifyClient(int key) {
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

    c->setName(name);
    c->setPhoneNumber(number);
    c->setAddress(address);

    saveToFile();
}

Client* ClientManager::findClientById(int id) const{
    auto it = clientList.find(id);
    if (it != clientList.end()) return it->second;
    return nullptr;
}

Client* ClientManager::findClientByName(const std::string& name) const{
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
        std::cin.ignore(1000, '\n');

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