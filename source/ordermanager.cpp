#include "ordermanager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime> // C 표준 라이브러리 <time.h>의 C++ 버전
#include <iomanip>
#include <algorithm>
// 필요한 표준 라이브러리 포함: 파일 I/O, 문자열 스트림, 시간 처리, 출력 포맷 등을 위해 사용

namespace OrderSystem { // OrderSystem이라는 네임스페이스 안에서 모든 코드가 정의됨
const std::string ORDER_FILE = "data/orderlist.txt"; // 주문 데이터 저장 위치를 문자열 상수로 정의

// 숫자에 3자리마다 ","를 찍어주는 함수를 추가했는데, 한국인들의 눈에 맞게 숫자를 읽기 쉽게 하기 위함으로 추가한 것이다.
// 다만, 입력할 때는 ","를 찍지 않고 입력받아야 하므로, 출력할 때만 나타나도록 구현했다.
inline std::string addComma(int number) {
    std::ostringstream oss; // ostringstream은 문자열에 데이터를 포맷팅하여 저장할 때 사용
    oss.imbue(std::locale(""));
    // imbue()는 스트림(std::ostringstream, std::cin, std::cout 등)에 새로운 지역(locale)을 설정하는 함수
    // std::locale("")는 현재 시스템의 로케일을 사용하여 숫자 형식(예: 천 단위 구분 기호)을 설정
    oss << std::fixed << number;
    // 여기서 fixed는 double 또는 float과 같은 부동소수점 숫자가 과학적 표기법이 아닌 소수점 이하 자릿수가 고정된 일반적인 표기법으로 출력하게 하는 함수
    return oss.str();
} // inline은 짧고 자주 호출되는 함수의 호출 비용을 줄이기 위해, 컴파일러에게 해당 함수의 본문을 함수 호출 부분에 직접 삽입하도록 지시하는 키워드

OrderManager::OrderManager() : nextOrderId(1), currentBundleOrderId(0) { // 객체 생성 시, nextOrderId를 1부터 시작하고, 묶음 주문 ID는 0으로 초기화
    loadFromFile(); // 프로그램 시작 시 저장된 주문 목록 불러옴
}

void OrderManager::startNewOrderBundle() {
    currentBundleOrderId = nextOrderId++;
} // 묶음 주문 시작 시 현재 주문 ID를 currentBundleOrderId에 저장하고 증가

void OrderManager::endOrderBundle() {
    currentBundleOrderId = 0;
} // 묶음 주문 종료 시, 다시 0으로 리셋

bool OrderManager::createOrder(int clientId, const std::string& productCode, int qty,
                               ClientSystem::ClientManager& clientManager, ProductSystem::ProductManager& productManager) {
    Client* client = clientManager.findClientById(clientId);
    if (!client) {
        std::cerr << "\n존재하지 않는 고객입니다.\n";
        return false;
    }
    // clientId : 주문을 요청한 고객의 ID, productCode : 주문할 제품의 코드, qty : 주문 수량, clientManager : 고객 확인을 위해 사용, productManager : 제품 재고 확인 및 차감을 위해 사용
    // 반환값: 성공 시 true, 실패 시 false

    auto& gm  = productManager.getGuitarManager();
    auto& em  = productManager.getEffectManager();
    auto& tm  = productManager.getTunerManager();
    auto& am  = productManager.getAmpManager();
    auto& pm  = productManager.getPowerSupplyManager();
    auto& cm  = productManager.getCableManager();
    auto& acm = productManager.getAccessoryManager();
    // 제품 매니저 참조 받기

    Guitar*      g  = gm.findByCode(productCode);
    Effect*      e  = em.findByCode(productCode);
    Tuner*       t  = tm.findByCode(productCode);
    Amp*         a  = am.findByCode(productCode);
    PowerSupply* p  = pm.findByCode(productCode);
    Cable*       c  = cm.findByCode(productCode);
    Accessory*   ac = acm.findByCode(productCode);
    // 입력받은 productCode가 어떤 제품군에 속하는지 전수 조사
    // 제품군마다 findByCode() 호출 → 해당 제품이 있으면 포인터로 반환, 없으면 nullptr

    std::vector<std::function<bool()>> actions = {
    // actions는 bool을 반환하고 인자를 받지 않는 함수(또는 람다)의 리스트
    // 해당 리스트는 순차적으로 실행할 수 있는 작업들을 담기 위한 용도
    // std::function<bool()>는 다음과 같은 형태의 함수 타입을 저장
    /*
    예를 들면,
    bool f(); 일반 함수
    bool operator()(); 함수 객체
    [](){ return true; } 람다

    C++은 호출 가능한 객체(callable object)가 여러 타입이 존재
    일반 함수: bool myFunc();
    람다: [](){ return true; }
    함수 포인터: bool (*fp)();
    함수 객체(Functor): struct F { bool operator()() {...} };
    이들은 서로 타입이 달라서 std::vector 같은 컨테이너에 저장하려면 하나로 묶는 공통 타입이 필요
    그것이 바로 std::function<T>
    std::function은 다양한 호출 가능한 객체를 하나의 타입으로 묶어주는 역할을 함
    std::function<bool()>는 “반환형 bool, 인자 없음”인 어떤 callable도 저장할 수 있는 래퍼(wrapper)

    std::function<bool()> f1 = [](){ return true; };
    std::function<bool()> f2 = myFunctionPointer;
    std::function<bool()> f3 = MyFunctor{};

    서로 다른 타입의 callable 객체들을 std::function<bool()>로 vector에 저장할 수 있음
    vector, map 등 STL에 넣어서 관리
    나중에 함수 구현을 바꿔도 타입 걱정 없이 교체 가능

    캡처 없는 람다는 암시적으로 함수 포인터로 변환이 가능하지만, 캡처가 들어간 람다는 내부적으로 익명 클래스 객체이기 때문에 함수 포인터로 대체할 수 없음
    람다와 함수 포인터를 병행하려 했지만 불가능했다.
    */
        [&]() { if (g  && g->isStockAvailable(qty)) { g->reduceStock(qty); gm.saveToFile(); return true; } return false; },
        [&]() { if (e  && e->isStockAvailable(qty)) { e->reduceStock(qty); em.saveToFile(); return true; } return false; },
        [&]() { if (t  && t->isStockAvailable(qty)) { t->reduceStock(qty); tm.saveToFile(); return true; } return false; },
        [&]() { if (a  && a->isStockAvailable(qty)) { a->reduceStock(qty); am.saveToFile(); return true; } return false; },
        [&]() { if (p  && p->isStockAvailable(qty)) { p->reduceStock(qty); pm.saveToFile(); return true; } return false; },
        [&]() { if (c  && c->isStockAvailable(qty)) { c->reduceStock(qty); cm.saveToFile(); return true; } return false; },
        [&]() { if (ac && ac->isStockAvailable(qty)) { ac->reduceStock(qty); acm.saveToFile(); return true; } return false; }
    }; // 7개의 람다 함수로 구성된 벡터 생성 후, 그 결과를 actions 벡터에 넣어둠
    /*
    제품이 존재하는지 g, e, t, a, p, c, ac 순서로 확인하고,
    재고가 충분할 때(isStockAvailable(qty)),
    재고를 줄이고(reduceStock(qty)),
    변경 내용을 파일로 저장(saveToFile())
    위 조건을 만족하면 true, 아니면 false
    */
    // 람다 함수는 [&]() → 현재 스코프의 모든 변수를 레퍼런스(참조, [&])로 캡처하여 사용
    // actions 벡터는 각 제품군에 대한 주문 가능 여부를 확인하는 함수들을 담고 있음
    // 굉장히 일괄적이고 직관적으로 보기 좋고 매우 간결함 이것이 람다 함수의 장점

    bool success = false; // 기본적으로 주문 성공 여부를 false로 초기화
    for (auto& act : actions) { // actions 벡터를 순회하면서 첫 번째로 성공하는 람다(true 반환)를 실행
        if (act()) { // 순서대로 람다를 실행 (act() 호출)
            success = true; // true가 반환되는 첫 람다에서 멈춤 → 그 제품이 유효하고 재고 충분했단 의미
            break; // success = true가 되면 반복문 종료
        }
    }

    if (!success) {
        std::cerr << "\n존재하지 않거나 재고가 부족한 제품입니다.\n";
        return false;
    } // 7개 제품군 중 어떤 제품도 조건을 충족하지 못하면 → 실패 메시지 출력 후 false

    // 아래와 같이, "묶음 주문 중이면 현재 묶음 ID 사용, 그렇지 않으면 nextOrderId를 사용하고 증가시킴" 삼항 연산자로 나타냄
    int orderIdToUse = (currentBundleOrderId > 0) ? currentBundleOrderId : nextOrderId++;
    Order* order = new Order(orderIdToUse, clientId, productCode, qty, getTodayDateTime());
    // 주문 ID는 currentBundleOrderId가 0보다 크면 묶음 주문 ID를 사용하고, 그렇지 않으면 nextOrderId를 사용하여 새 주문 생성
    // 새 Order 객체 동적 생성: 주문 ID, 고객 ID, 제품 코드, 수량, 현재 날짜 및 시간
    orders.push_back(order); // 생성한 주문을 orders 벡터에 추가
    saveToFile(); // 전체 주문 리스트를 파일에 저장

    std::cout << "\n주문이 완료되었습니다.\n";
    return true; // 주문 성공 메시지 출력 후 true 반환
}

bool OrderManager::refundOrder(int orderId, ProductSystem::ProductManager& productManager) {
    // orderId: 환불하고자 하는 주문 번호
    // productManager: 제품군 매니저 (재고 복원을 위해 필요)
    bool anyRefunded = false; // 한 개라도 환불되었는지를 추적하기 위해 디폴트는 false
    for (auto& order : orders) { // orders 벡터에 있는 모든 주문 객체를 순회, order는 Order* 포인터에 대한 참조
        if (order->getOrderId() == orderId && order->getStatus() != "환불완료") {
            // 주문 번호가 일치하고 "환불완료"가 아닐 시에만 처리
            std::string code = order->getProductCode();
            int qty = order->getQuantity(); // 해당 주문의 제품 코드와 수량을 가져옴

            auto& gm  = productManager.getGuitarManager();
            auto& em  = productManager.getEffectManager();
            auto& tm  = productManager.getTunerManager();
            auto& am  = productManager.getAmpManager();
            auto& pm  = productManager.getPowerSupplyManager();
            auto& cm  = productManager.getCableManager();
            auto& acm = productManager.getAccessoryManager();
            // 제품 코드를 기준으로 어느 매니저가 가진 제품인지 판단하고, 해당 매니저의 재고를 복구해야 함

            if (auto* g = gm.findByCode(code)) { g->addStock(qty); gm.saveToFile(); } // findByCode()로 제품 코드 일치 여부 확인
            else if (auto* e = em.findByCode(code)) { e->addStock(qty); em.saveToFile(); }
            else if (auto* t = tm.findByCode(code)) { t->addStock(qty); tm.saveToFile(); }
            else if (auto* a = am.findByCode(code)) { a->addStock(qty); am.saveToFile(); }
            else if (auto* p = pm.findByCode(code)) { p->addStock(qty); pm.saveToFile(); }
            else if (auto* c = cm.findByCode(code)) { c->addStock(qty); cm.saveToFile(); }
            else if (auto* ac = acm.findByCode(code)) { ac->addStock(qty); acm.saveToFile(); }
            // if-else if 구조라 하나만 복원됨 (제품군 하나에만 속한다는 전제)
            // Lambda 형식으로 안하고 if-else if 구문으로 해봄

            order->setDate(getTodayDateTime());
            order->setStatus("환불완료");
            anyRefunded = true;
        }
    }

    if (anyRefunded) { // 하나라도 환불되었으면
        saveToFile(); // 주문 전체 저장
        std::cout << "\n묶음 주문 전체 환불이 완료되었습니다.\n";
        return true;
    } else { // 조건을 만족하는 주문이 없으면
        std::cerr << "\n해당 주문번호의 환불 가능한 항목이 없습니다.\n";
        return false;
    }
}

// loadFromFile() : 텍스트 → 객체 변환, 벡터에 저장
void OrderManager::loadFromFile() { // 텍스트 파일(orderlist.txt)로부터 주문 정보를 읽어서 메모리에 Order 객체로 저장하는 함수
    std::ifstream file(ORDER_FILE); // ORDER_FILE은 "data/orderlist.txt"라는 상수, 해당 경로의 파일을 읽기 모드로 열기
    if (!file.is_open()) return; // 파일이 존재하지 않거나 열기에 실패하면 그냥 return (로드 생략)

    std::string line;
    while (getline(file, line)) { // 한 줄씩 읽음 (각 줄이 하나의 주문 정보)
        std::stringstream ss(line); // 읽은 문자열 라인을 파싱하기 위해 stringstream으로 감쌈
        int id, clientId, qty;
        std::string productCode, date, status; // 파일에서 읽을 데이터를 받을 지역 변수들 선언

        ss >> id; ss.ignore(); // 주문 ID 읽고 , 남아 있는 \n 제거
        ss >> clientId; ss.ignore(); // 고객 ID 읽고 , 남아 있는 \n 제거
        getline(ss, productCode, ','); // 제품 코드 읽기 (문자열, ','까지)
        ss >> qty; ss.ignore(); // 수량 읽고 , 남아 있는 \n 제거
        getline(ss, date, ','); // 날짜 읽기 (문자열, ','까지)
        getline(ss, status); // 상태 읽기 (마지막 값, 줄 끝까지)
        // 파일 라인 형식 예시
        // 3,102,GTR-001,2,2025/06/07 14:25:13,구매완료

        Order* order = new Order(id, clientId, productCode, qty, date, status);
        // 읽은 값을 바탕으로 Order 객체 생성 → 동적으로 힙에 생성
        orders.push_back(order); // orders 벡터에 포인터 추가
        if (id >= nextOrderId) nextOrderId = id + 1; // 다음 주문 ID 갱신: 현재까지 가장 큰 ID보다 크면 업데이트
    }
}

// saveToFile() : 객체 → 텍스트 파일 출력
void OrderManager::saveToFile() const { // 메모리에 있는 주문 객체들을 orderlist.txt 파일에 저장
    // 프로그램 종료 시 또는 주문 추가/환불 후 호출
    std::ofstream file(ORDER_FILE); // 파일 열기 (기본은 truncate 모드 → 이전 내용 덮어쓰기)
    for (const auto& order : orders) { // 모든 주문 객체 순회 (Order* 포인터)
        file << order->getOrderId() << ","
             << order->getClientId() << ","
             << order->getProductCode() << ","
             << order->getQuantity() << ","
             << order->getDate() << ","
             << order->getStatus() << "\n";
    } // 주문ID,고객ID,제품코드,수량,날짜,상태 ex) 3,102,GTR-001,2,2025/06/07 14:25:13,구매완료
}

/*
OrderManager::showAllOrders()
→
주문 전체 목록을 표 형식으로 출력
이후 키워드 검색 기능 제공
clientManager와 productManager를 통해 고객 이름과 제품명을 함께 조회
const 함수 → 멤버 데이터 수정하지 않음
*/

void OrderManager::showAllOrders(const ClientSystem::ClientManager& clientManager, const ProductSystem::ProductManager& productManager) const {
    while (true) { // return을 통해 빠져나갈 때까지 반복되며, 한 번 실행할 때 전체 주문 → 검색 → 종료 여부를 묻는 구조
        std::cout << "\n주문번호 | 고객ID   | 고객이름      | 제품코드   | 제품명(모델명)           | 수량     | 날짜                | 상태\n";
        std::cout << "------------------------------------------------------------------------------------------------------------------------\n";

        for (const auto& order : orders) { // orders는 Order*들의 벡터, 각 주문의 정보 출력 전, 추가 정보(고객 이름, 제품명)를 파악함
            const Client* client = clientManager.findClientById(order->getClientId());
            // 주문에 있는 고객 ID로 ClientManager에서 고객 찾음, 없으면 "알 수 없음"으로 표시
            std::string clientName = client ? client->getName() : "알 수 없음";
            std::string clientIdStr = client ? std::to_string(client->id()) : "알 수 없음";
            std::string productCode = order->getProductCode();
            // 제품코드로 ProductManager 내부의 각 제품군에서 findByCode() 호출
            std::string productName = "알 수 없음"; // 처음 일치하는 제품군의 모델명을 productName으로 설정

            if (const Guitar* g = productManager.getGuitarManager().findByCode(productCode)) {
                productName = g->getModel();
            } else if (const Effect* e = productManager.getEffectManager().findByCode(productCode)) {
                productName = e->getModel();
            } else if (const Tuner* t = productManager.getTunerManager().findByCode(productCode)) {
                productName = t->getModel();
            } else if (const Amp* a = productManager.getAmpManager().findByCode(productCode)) {
                productName = a->getModel();
            } else if (const PowerSupply* p = productManager.getPowerSupplyManager().findByCode(productCode)) {
                productName = p->getModel();
            } else if (const Cable* c = productManager.getCableManager().findByCode(productCode)) {
                productName = c->getModel();
            } else if (const Accessory* ac = productManager.getAccessoryManager().findByCode(productCode)) {
                productName = ac->getModel();
            }

            std::cout << std::setw(9) << order->getOrderId() << " | "
                      << std::setw(8) << clientIdStr << " | "
                      << std::setw(12) << clientName << " | "
                      << std::setw(10) << productCode << " | "
                      << std::setw(22) << productName << " | "
                      << std::setw(8) << addComma(order->getQuantity()) << " | "
                      << std::setw(19) << order->getDate() << " | "
                      << order->getStatus() << "\n";
        } // std::setw(n)으로 열 너비를 고정해서 정렬된 표 형식으로 출력
        // addComma()를 사용해 수량을 "1,000" 형식으로 출력 (모든 정수형 입력 데이터 자료형은 displays에 한해서 이에 따름)

        std::cout << "\n========= 키워드 검색 =========\n";
        std::cout << "1. 검색하기\n";
        std::cout << "2. 되돌아가기\n";
        std::cout << "선택: ";

        int opt;
        std::cin >> opt; // 사용자 입력을 받아 opt에 저장

        if (opt == 1) {
            std::cin.ignore(); // 버퍼 정리
            std::string keyword; // 키워드를 문자열로 입력받음
            std::cout << "검색할 키워드 입력: ";
            std::getline(std::cin, keyword);

            std::cout << "\n[검색 결과]\n";
            std::cout << "주문번호 | 고객ID   | 고객이름      | 제품코드   | 제품명(모델명)           | 수량     | 날짜                | 상태\n";
            std::cout << "------------------------------------------------------------------------------------------------------------------------\n";

            bool found = false;
            for (const auto& order : orders) {
                const Client* client = clientManager.findClientById(order->getClientId());
                std::string clientName = client ? client->getName() : "알 수 없음";
                std::string clientIdStr = client ? std::to_string(client->id()) : "알 수 없음";
                std::string productCode = order->getProductCode();
                std::string productName = "알 수 없음";

                if (const Guitar* g = productManager.getGuitarManager().findByCode(productCode)) {
                    productName = g->getModel();
                } else if (const Effect* e = productManager.getEffectManager().findByCode(productCode)) {
                    productName = e->getModel();
                } else if (const Tuner* t = productManager.getTunerManager().findByCode(productCode)) {
                    productName = t->getModel();
                } else if (const Amp* a = productManager.getAmpManager().findByCode(productCode)) {
                    productName = a->getModel();
                } else if (const PowerSupply* p = productManager.getPowerSupplyManager().findByCode(productCode)) {
                    productName = p->getModel();
                } else if (const Cable* c = productManager.getCableManager().findByCode(productCode)) {
                    productName = c->getModel();
                } else if (const Accessory* ac = productManager.getAccessoryManager().findByCode(productCode)) {
                    productName = ac->getModel();
                }

                if (std::to_string(order->getOrderId()).find(keyword) != std::string::npos ||
                    clientIdStr.find(keyword) != std::string::npos ||
                    clientName.find(keyword) != std::string::npos ||
                    productCode.find(keyword) != std::string::npos ||
                    productName.find(keyword) != std::string::npos ||
                    order->getStatus().find(keyword) != std::string::npos) {
                    std::cout << std::setw(9) << order->getOrderId() << " | "
                              << std::setw(8) << clientIdStr << " | "
                              << std::setw(12) << clientName << " | "
                              << std::setw(10) << productCode << " | "
                              << std::setw(22) << productName << " | "
                              << std::setw(8) << addComma(order->getQuantity()) << " | "
                              << std::setw(19) << order->getDate() << " | "
                              << order->getStatus() << "\n";
                    found = true;
                } // orderId, 고객ID, 고객이름, 제품코드, 모델명, 상태 중 하나라도 키워드가 포함되어 있으면 출력 대상
            }

            if (!found) {
                std::cout << "\n일치하는 항목이 없습니다.\n";
            }

            std::cout << "\n확인을 했다면 Enter 키를 입력하세요";
            std::cin.get(); // // 대기 후 함수 종료, 다음 행동으로 넘어가는 기폭제 역할이라 설명 드렸음 (char 단위 크기)
            return; // 종료 후 상위 메뉴로 이동

        } else if (opt == 2) {
            return; // 종료 후 상위 메뉴로 이동
        } else {
            std::cout << "\n잘못된 선택입니다.\n";
        }
    }
}

/*
#include <ctime>
시간과 관련된 다음과 같은 함수/구조체를 포함:

    time() – 현재 시간을 time_t 형태로 반환
    localtime() – 지역 시간(tm)으로 변환
    tm – 시간 정보 구조체 (년, 월, 일, 시, 분, 초 등 포함)
    strftime() – 포맷된 문자열로 시간 표현 가능
    difftime(), mktime() 등도 있음
*/

std::string OrderManager::getTodayDateTime() const { // 현재 날짜 및 시간을 문자열로 리턴, 멤버 상태를 변경하지 않음
    time_t now = time(0); // time() 함수는 현재 시간을 time_t 타입으로 반환
    // time(0) 또는 time(nullptr)로 호출
    // time_t는 시스템 내부 시간 단위로, "1970년 1월 1일 자정"부터의 초(second) 수를 의미함
    tm* ltm = localtime(&now); // localtime()은 time_t 값을 받아서 지역시간 구조체(tm*)로 변환
    // 반환값은 다음 필드를 가진 구조체 포인터: int tm_sec;의 범위 0~59, int tm_mon;의 범위 0~11 등
    char buf[32]; // 문자열 버퍼 생성, snprintf()를 이용해 포맷된 문자열을 넣을 임시 배열 (32칸이면 충분)
    snprintf(buf, sizeof(buf), "%04d/%02d/%02d %02d:%02d:%02d",
             1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
             ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
             // snprintf()는 printf()처럼 작동하지만 결과를 buf에 씀 (버퍼 오버플로우 방지)
             // 날짜 형식: "2025/06/08 12:45:01"
    return std::string(buf); // C 스타일 문자열을 C++의 std::string으로 변환하여 반환
}

/*
%04d	네 자리 정수, 앞을 0으로 채움	예: 2025
%02d	두 자리 정수, 앞을 0으로 채움	예: 06, 08, 09, 00
1900 + ltm->tm_year	연도 보정	tm_year는 1900년 이후 경과 연도이므로 반드시 보정
1 + ltm->tm_mon	월 보정	011 → 112로 바꿔야 일반적 월 의미 됨
*/

}