#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include <map>
#include <vector>
#include <string>
#include "client.h"

namespace ClientSystem {
class ClientManager {
public:
    ClientManager();
    ~ClientManager();

    void inputClient();                // 고객 입력
    void addClient(Client*);           // 고객 객체 추가
    void deleteClient(int);            // 고객 삭제
    void modifyClient(int);            // 고객 수정
    int makeId();                      // ID 생성
    void displayInfo();                // 전체 고객 정보 출력
    void saveToFile();                 // 텍스트 파일 저장
    std::vector<std::string> parseCSV(std::istream&, char); // CSV 파싱

    Client* findClientById(int) const;       // ID로 검색
    Client* findClientByName(const std::string&) const; // 이름으로 검색

    bool displayMenu();                // 메뉴 표시

private:
    std::map<int, Client*> clientList;
};
}

#endif // __CLIENTMANAGER_H__