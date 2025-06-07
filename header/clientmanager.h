#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include <map>
#include <vector>
#include <string>
// STL 자료구조 3개
#include "client.h"

namespace ClientSystem {
    // 이 클래스를 ClientSystem이라는 이름공간(namespace) 안에 정의함으로써, 다른 클래스와 이름 충돌을 방지
    // 보통 ClientSystem::ClientManager cm;와 같은 방식으로 사용
class ClientManager { // 내부적으로 Client 객체들을 std::map으로 관리하며, 입력/수정/삭제 기능을 제공
public:
    ClientManager(); // 생성자: 객체 초기화 및 파일 로딩 등 준비 작업에 사용됨
    ~ClientManager(); // 소멸자: 할당된 Client* 객체들을 delete하여 메모리 누수 방지

    void inputClient();                // cin으로 새 고객 정보 받아 Client 객체 생성
    void addClient(Client*);           // 외부에서 만든 Client 포인터를 내부 리스트에 추가
    void deleteClient(int);            // 고객 삭제
    void modifyClient(int);            // 고객 수정
    int makeId();                      // 고객에게 부여할 고유 ID를 자동 생성
    void displayInfo();                // 전체 고객 정보 출력
    void saveToFile();                 // 텍스트 파일 저장
    std::vector<std::string> parseCSV(std::istream&, char);
    // CSV 파싱
    // 한 줄의 CSV 데이터를 콤마 단위로 나눠서 문자열 벡터로 반환

    Client* findClientById(int) const;       // ID로 검색
    Client* findClientByName(const std::string&) const; // 이름으로 검색
    // ID 또는 이름으로 Client를 찾아서 포인터 반환

    bool displayMenu();                // 메뉴 표시

private:
    std::map<int, Client*> clientList;
    // key = 고객 ID, value = Client*
    // 빠른 검색과 수정이 가능한 map 구조 사용
};
}

#endif // __CLIENTMANAGER_H__