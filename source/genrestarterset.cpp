#include "genrestarterset.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "productmanager.h"
#include "ordermanager.h"
#include "clientmanager.h"

using namespace std;
using namespace GenreStarter;
using namespace ProductSystem;
using namespace OrderSystem;
using namespace ClientSystem;

namespace  {
inline std::string addComma(int number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << std::fixed << number;
    return oss.str();
}
}

const std::string GUITAR_FILE = "data/pdList_guitars.txt";
const std::string EFFECT_FILE = "data/pdList_effects.txt";

GenreStarterSet::GenreStarterSet() {
    genreNames = {
        {1, "포크 록"}, {2, "블루스 록"}, {3, "프로그레시브 록"}, {4, "사이키델릭 록"},
        {5, "하드 록"}, {6, "팝 록"}, {7, "소프트 록"}, {8, "아트 록"},
        {9, "루츠 록"}, {10, "메탈"}, {11, "일렉트로닉 록"}, {12, "펑크 록"},
        {13, "얼터너티브 록"}, {14, "모던 록"}
    };

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
    };
}

void GenreStarterSet::displayGenreMenu(ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    while (true) {
        cout << "\n--- Genre Starter Pack ---\n";
        for (const auto& pair : genreNames) {
            cout << pair.first << ". " << pair.second << endl;
        }
        cout << "15. 이전 메뉴로 돌아가기" << endl;
        cout << "선택: ";
        int genreId;
        cin >> genreId;

        if (genreId == 15) return;
        if (genreNames.count(genreId)) {
            processGenreSelection(genreId, clientManager, productManager, orderManager);
        } else {
            cout << "잘못된 선택입니다." << endl;
        }
    }
}

void GenreStarterSet::processGenreSelection(int genreId, ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    string gtype = genreConfigs[genreId].guitarType;
    vector<string> etypes = genreConfigs[genreId].effectTypes;

    cout << "\n해당 장르는 일렉기타 1개와 이펙터의 타입 종류 총 " << etypes.size() << "개입니다. 구매를 진행하시겠습니까?\n";
    cout << "1. 예\n2. 아니오\n선택: ";
    int yn; cin >> yn;
    if (yn != 1) return;

    if (orderManager) orderManager->startNewOrderBundle();

    cout << "\n----------------------<해당 장르에 맞는 일렉기타 타입 제품 목록>----------------------\n";
    cout << "코드       | 모델명           | 브랜드       | 타입           | 가격       | 재고\n";
    cout << "--------------------------------------------------------------------------------------\n";
    loadGuitarMatches(gtype);

    cout << "\n--------일렉기타 구매 입력----------\n";
    createGuitarOrder(clientManager, productManager, orderManager);

    cout << "\n-----------------------<해당 장르에 맞는 이펙터 타입 제품 목록>-----------------------\n";
    cout << "코드       | 모델명           | 브랜드       | 타입           | 가격       | 재고\n";
    cout << "--------------------------------------------------------------------------------------\n";
    loadEffectMatches(etypes);

    cout << "\n--------이펙터 구매 입력-----------\n";
    createEffectOrders(etypes, clientManager, productManager, orderManager);

    if (orderManager) orderManager->endOrderBundle();

    cout << "\n주문이 완료되었습니다!\n" << endl;
}

void GenreStarterSet::loadGuitarMatches(const string& type) {
    ifstream file(GUITAR_FILE);
    string line;
    while (getline(file, line)) {
        vector<string> tokens;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) tokens.push_back(item);
        if (tokens.size() >= 6 && tokens[3].find(type) != string::npos) {
            cout << setw(10) << tokens[0] << " | "
                 << setw(15) << tokens[1] << " | "
                 << setw(12) << tokens[2] << " | "
                 << setw(13) << tokens[3] << " | "
                 << setw(10) << addComma(stoi(tokens[4])) << " | "
                 << setw(6) << addComma(stoi(tokens[5])) << endl;
        }
    }
    file.close();
}

void GenreStarterSet::loadEffectMatches(const vector<string>& types) {
    ifstream file(EFFECT_FILE);
    string line;
    while (getline(file, line)) {
        vector<string> tokens;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) tokens.push_back(item);
        if (tokens.size() >= 6) {
            for (const auto& t : types) {
                if (tokens[3] == t) {
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

void GenreStarterSet::createGuitarOrder(ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    int cid; string code; int qty;
    cout << "고객 ID: "; cin >> cid;
    cout << "제품 코드: "; cin >> code;
    cout << "수량: "; cin >> qty;
    orderManager->createOrder(cid, code, qty, *clientManager, *productManager);
}

void GenreStarterSet::createEffectOrders(const vector<string>& types, ClientManager* clientManager, ProductManager* productManager, OrderManager* orderManager) {
    int cid;
    cout << "고객 ID: "; cin >> cid;
    cin.ignore();
    for (size_t i = 0; i < types.size(); ++i) {
        string code;
        int qty;
        cout << types[i] << " 제품 코드: ";
        getline(cin, code);
        cout << types[i] << " 수량: ";
        cin >> qty;
        cin.ignore();
        orderManager->createOrder(cid, code, qty, *clientManager, *productManager);
    }
}