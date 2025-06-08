#include "genrestarterset.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "productmanager.h"
#include "ordermanager.h"
#include "clientmanager.h"

// 사용자가 원하는 장르 선택 → 해당 장르에 맞는 일렉기타와 이펙터들을 파일에서 필터링하여 추천
// 추천 목록을 보고 고객 ID/제품 코드/수량을 입력하면 주문까지 자동으로 처리
// 내부적으로 ProductManager, OrderManager, ClientManager 연동

using namespace std;
using namespace GenreStarter;
using namespace ProductSystem;
using namespace OrderSystem;
using namespace ClientSystem;

/*
익명 네임스페이스(namespace { })

익명 namespace에 감싼 해당 파일 내부(translation unit)에서만 유효하고 이것은 다른 cpp파일에서 접근이 불가능하다.
이것은 이름 충돌을 방지하기 위한 "로컬 유틸리티 함수" 정의 방식으로써 전역함수인 addComma()을 genrestarterset.cpp에서만
사용하도록 scope 제한하였다.

위에서 통으로 선언한 namespace의 모든 시스템 안의 클래스나 함수명을 생략하고 쓰겠다는 의도로 익명 namespace는
별개의, 로컬한 영역이다.
*/

namespace  {
inline std::string addComma(int number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << std::fixed << number;
    return oss.str();
} // inline + 해당 addComma() 함수에 대한 설명은 ordermanager.cpp 또는 ampmanager.cpp에 있기에 생략
}
// namespace {}로 addComma()를 지역 제한한 건 충돌 방지 + 스코프 최소화라는 의도로 설계함
// 특히, inline이 있어도 ODR (One Definition Rule)에 위반될 수도 있고
// 각 시스템마다 addComma를 정의한 상황에서는, 링커 충돌을 방지하기 위해 익명 namespace를 사용함

const std::string GUITAR_FILE = "data/pdList_guitars.txt";
const std::string EFFECT_FILE = "data/pdList_effects.txt";

GenreStarterSet::GenreStarterSet() {
    genreNames = {
        {1, "포크 록"}, {2, "블루스 록"}, {3, "프로그레시브 록"}, {4, "사이키델릭 록"},
        {5, "하드 록"}, {6, "팝 록"}, {7, "소프트 록"}, {8, "아트 록"},
        {9, "루츠 록"}, {10, "메탈"}, {11, "일렉트로닉 록"}, {12, "펑크 록"},
        {13, "얼터너티브 록"}, {14, "모던 록"}
    }; // 14개의 장르와 각각의 추천 구성(일렉기타 1종 + 이펙터 여러 개)을 초기화

    genreConfigs = {
        {1, {"Superstrat", {"Reverb", "Chorus", "EQ"}}},
        {2, {"Semi-Hollow", {"Overdrive", "Reverb", "Compressor"}}},
        {3, {"Superstrat", {"Delay", "Phaser", "Flanger", "Reverb"}}},
        {4, {"Jazz", {"Fuzz", "Echo", "Vibrato"}}}, // "Offset" → "Jazz"
        {5, {"Les Paul", {"Overdrive", "Distortion", "EQ"}}},
        {6, {"Stratocaster", {"Chorus", "Delay", "Reverb"}}},
        {7, {"Telecaster", {"Reverb", "Compressor"}}},
        {8, {"Jazz", {"Reverb", "EQ", "Chorus"}}},   // "Offset" → "Jazz"
        {9, {"Stratocaster", {"Reverb", "EQ"}}},     // "Parlor" → "Stratocaster" (대체)
        {10, {"Flying V", {"Distortion", "Noise Gate", "EQ", "Delay"}}},
        {11, {"Superstrat", {"Synth", "Chorus", "Reverb"}}},
        {12, {"Jaguar", {"Fuzz", "Distortion", "EQ"}}}, // 없는 경우: 필요 시 "Jazz" 대체 가능
        {13, {"Stratocaster", {"Overdrive", "Distortion", "Flanger", "Chorus"}}},
        {14, {"Jazz", {"Reverb", "Overdrive", "EQ"}}} // "Offset" → "Jazz"
    }; // 예를 들면, 포크 록에는 Superstrat 타입의 기타와 Reverb, Chorus, EQ 이펙터 추천
} // 음악 중에서도 일렉기타 분야의 계열에 대한 중급 지식에 대한 설명이라, 밴드를 구성하는 입문자들을 위한 스타터팩이라고 이해하면 좋다
// 현존하는 음악 악기 쇼핑몰은 굉장히 마이너한 분야이기 때문에, 안 그래도 어렵고 모르는 사람들이 많은데, 쇼핑몰 구조가 세세하게 짜여져 있지 않은 상황을 개선

void GenreStarterSet::displayGenreMenu(ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    // GenreStarterSet 클래스의 멤버 함수, 메뉴에서 사용자 입력을 받고, 선택된 장르에 맞는 주문 처리까지 유도
    // ClientManager*: 고객 정보를 조회하고 유효성 검사할 때 사용
    // ProductManager*: 제품 목록(기타, 이펙터)을 읽고 필터링할 때 사용
    // OrderManager*: 실제 주문 등록을 처리할 때 사용

    while (true) {
        cout << "\n--- Genre Starter Pack ---\n";
        for (const auto& pair : genreNames) { // genreNames는 std::map<int, std::string> 형태
            // key: 번호 (1 ~ 14)
            // value: 장르 이름 (예: "하드 록", "펑크 록" 등)
            cout << pair.first << ". " << pair.second << endl;
        }
        cout << "15. 이전 메뉴로 돌아가기" << endl;
        cout << "선택: ";
        int genreId;
        cin >> genreId; // 사용자로부터 정수를 입력받아 genreId에 저장

        if (genreId == 15) return; // 이 함수를 빠져나가면서 상위 메뉴로 돌아감
        if (genreNames.count(genreId)) { // genreId가 genreNames에 존재하는 키인지 확인
            // 유효하면 해당 장르에 맞는 주문 생성 로직으로 이동
            // 호출되는 함수: processGenreSelection(...)
            processGenreSelection(genreId, clientManager, productManager, orderManager);
        } else {
            cout << "잘못된 선택입니다." << endl;
        }
    }
}

void GenreStarterSet::processGenreSelection(int genreId, ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    // genreConfigs는 std::map<int, GenreConfig>, genreId에 해당하는 장르 구성(GenreConfig)을 가져와서,
    string gtype = genreConfigs[genreId].guitarType;
    // gtype: 추천하는 기타 타입 문자열 (예: "Les Paul")
    vector<string> etypes = genreConfigs[genreId].effectTypes;
    // etypes: 추천 이펙터 타입 리스트 (예: { "Overdrive", "EQ" })

    cout << "\n해당 장르는 일렉기타 1개와 이펙터의 타입 종류 총 " << etypes.size() << "개입니다. 구매를 진행하시겠습니까?\n";
    // 총 이펙터 타입 개수를 안내, 안내를 안하면 이펙터의 개수가 몇 개인지도 모르고 구매 절차를 밟게 됨
    cout << "1. 예\n2. 아니오\n선택: ";
    int yn; cin >> yn;
    if (yn != 1) return;
    // 사용자가 1 (예)을 선택하지 않으면(=2 선택함) 이 함수는 종료되고 이전 메뉴로 돌아감

    if (orderManager) orderManager->startNewOrderBundle();
    // OrderManager가 유효하면 묶음 주문 시작
    // 이후 생성되는 주문은 모두 같은 주문번호로 묶임

    cout << "\n----------------------<해당 장르에 맞는 일렉기타 타입 제품 목록>----------------------\n";
    cout << "코드       | 모델명           | 브랜드       | 타입           | 가격       | 재고\n";
    cout << "--------------------------------------------------------------------------------------\n";
    loadGuitarMatches(gtype);
    // loadGuitarMatches(gtype) 함수에서 pdList_guitars.txt를 읽고, 해당 타입 포함된 제품만 출력

    cout << "\n--------일렉기타 구매 입력----------\n";
    createGuitarOrder(clientManager, productManager, orderManager);
    // 기타 주문을 실제로 수행
    // 고객 ID, 제품 코드, 수량을 입력받아 OrderManager::createOrder(...) 호출

    cout << "\n-----------------------<해당 장르에 맞는 이펙터 타입 제품 목록>-----------------------\n";
    cout << "코드       | 모델명           | 브랜드       | 타입           | 가격       | 재고\n";
    cout << "--------------------------------------------------------------------------------------\n";
    loadEffectMatches(etypes);
    // 여러 이펙터 타입에 맞는 제품들을 pdList_effects.txt에서 찾아서 출력
    // 타입은 == 비교이므로 완전 일치하는 경우만 필터링됨

    cout << "\n--------이펙터 구매 입력-----------\n";
    createEffectOrders(etypes, clientManager, productManager, orderManager);
    // etypes 개수만큼 반복해서 주문 입력 받음
    // 각 타입에 대해 제품 코드, 수량 입력 → 주문 생성

    if (orderManager) orderManager->endOrderBundle();
    // 해당 장르에 대한 전체 주문이 끝나면 endOrderBundle() 호출 → 묶음 주문 번호 초기화됨

    cout << "\n주문이 완료되었습니다!\n" << endl;
    // 전체 프로세스가 끝나면 사용자에게 주문 완료 알림
}

// 바로 아래의 두 함수는 각각 .txt 파일에 저장된 기타/이펙터 제품 목록에서 특정 타입 조건을 만족하는 항목만 출력하는 역할을 수행함
void GenreStarterSet::loadGuitarMatches(const string& type) {
    // 입력된 type 문자열이 포함된 기타 제품만 필터링해서 출력하는 함수
    // 예를 들면, type = "Les Paul" 이면 "타입" 칼럼에 "Les Paul"이 포함된 제품만 출력
    ifstream file(GUITAR_FILE); // ifstream으로 기타 목록 파일 열기
    string line;
    while (getline(file, line)) { // 파일을 한 줄씩 읽어서 line에 저장, 각 line은 하나의 제품 정보를 담은 CSV 데이터
        vector<string> tokens;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) tokens.push_back(item);
        // stringstream으로 line에 대해 ","을 기준으로 쪼개서 tokens 벡터에 저장
        if (tokens.size() >= 6 && tokens[3].find(type) != string::npos) { // → 부분일치 검색
            cout << setw(10) << tokens[0] << " | "
                 << setw(15) << tokens[1] << " | "
                 << setw(12) << tokens[2] << " | "
                 << setw(13) << tokens[3] << " | "
                 << setw(10) << addComma(stoi(tokens[4])) << " | "
                 << setw(6) << addComma(stoi(tokens[5])) << endl;
        } // tokens[3]은 제품 타입 칼럼 (예시: "Superstrat"), 입력된 type이 이 문자열에 포함되어 있으면 출력
    }
    // 예를 들면, GTR001,Stratocaster,Fender,Superstrat,2090000,7
    // → tokens[0] = "GTR001", tokens[1] = "Stratocaster", ... tokens[5] = "7"
    file.close(); // 명시적으로 파일 닫기
}

void GenreStarterSet::loadEffectMatches(const vector<string>& types) {
    // types는 추천 이펙터 타입들의 벡터 (예: { "EQ", "Reverb", "Distortion" })
    // 이 중 하나라도 == 일치하는 제품을 pdList_effects.txt에서 찾아 출력
    ifstream file(EFFECT_FILE);
    string line;
    while (getline(file, line)) {
        vector<string> tokens;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) tokens.push_back(item);
        // pdList_effects.txt에서 한 줄씩 읽고, 쉼표(",") 기준으로 쪼개어 tokens에 저장
        if (tokens.size() >= 6) {
            for (const auto& t : types) {
                if (tokens[3] == t) { // 이펙터 타입(tokens[3])이 types 목록 중 하나와 정확히 일치하면 출력
                    // == 비교이므로 부분 일치가 아니라 완전 일치
                    cout << setw(10) << tokens[0] << " | "
                         << setw(15) << tokens[1] << " | "
                         << setw(12) << tokens[2] << " | "
                         << setw(13) << tokens[3] << " | "
                         << setw(10) << addComma(stoi(tokens[4])) << " | "
                         << setw(6) << addComma(stoi(tokens[5])) << endl;
                    break;
                }
            }
        }
    }
    file.close();
}

// 바로 아래의 두 함수는 실제로 사용자가 기타 및 이펙터 제품을 주문하는 입력 모션에 대한 기능
void GenreStarterSet::createGuitarOrder(ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    // 사용자가 특정 일렉기타 제품을 선택해서 주문하는 함수
    // ClientManager, ProductManager, OrderManager는 주문 유효성 확인 및 처리에 필요
    int cid; string code; int qty;
    cout << "고객 ID: "; cin >> cid;
    cout << "제품 코드: "; cin >> code;
    cout << "수량: "; cin >> qty;
    orderManager->createOrder(cid, code, qty, *clientManager, *productManager);
    /*
    고객이 존재하는지 확인
    제품 코드가 유효한지 확인
    재고가 충분한지 확인
    유효하면 재고 차감 및 주문 생성
    *clientManager / *productManager는 객체 참조 전달
    */
}

void GenreStarterSet::createEffectOrders(const vector<string>& types, ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    // 장르에 따른 여러 이펙터 타입에 대한 반복 주문 입력 처리
    // types: 이펙터 타입 리스트 (예시: {"Reverb", "Overdrive", "EQ"})
    int cid;
    cout << "고객 ID: "; cin >> cid; // 고객 ID를 먼저 입력받음
    cin.ignore(); // 개행(\n) 문자와 같은 버퍼 정리
    for (size_t i = 0; i < types.size(); ++i) { // 추천된 이펙터 타입 수만큼 반복
        // 이펙터 타입을 출력하여 사용자에게 어떤 제품인지 힌트를 주고, 제품 코드와 수량을 입력받음
        string code;
        int qty;
        cout << types[i] << " 제품 코드: ";
        getline(cin, code); // 띄워쓰기 포함하기 위한 장점을 가진 getline() 함수 사용
        cout << types[i] << " 수량: ";
        cin >> qty;
        cin.ignore(); // 다시 cin.ignore()로 다음 입력 대비 버퍼 정리
        orderManager->createOrder(cid, code, qty, *clientManager, *productManager);
        // 각 이펙터에 대해 개별 주문 생성 요청
        // OrderManager 내부에서 재고, 유효성, 오류 메시지 처리까지 수행
    }
}