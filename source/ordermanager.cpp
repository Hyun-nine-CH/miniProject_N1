#include "ordermanager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
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

OrderManager::OrderManager() : nextOrderId(1), currentBundleOrderId(0) {
    loadFromFile();
}

void OrderManager::startNewOrderBundle() {
    currentBundleOrderId = nextOrderId++;
}

void OrderManager::endOrderBundle() {
    currentBundleOrderId = 0;
}

bool OrderManager::createOrder(int clientId, const std::string& productCode, int qty,
                               ClientSystem::ClientManager& clientManager, ProductSystem::ProductManager& productManager) {
    Client* client = clientManager.findClientById(clientId);
    if (!client) {
        std::cerr << "\n존재하지 않는 고객입니다.\n";
        return false;
    }

    auto& gm  = productManager.getGuitarManager();
    auto& em  = productManager.getEffectManager();
    auto& tm  = productManager.getTunerManager();
    auto& am  = productManager.getAmpManager();
    auto& pm  = productManager.getPowerSupplyManager();
    auto& cm  = productManager.getCableManager();
    auto& acm = productManager.getAccessoryManager();

    Guitar*      g  = gm.findByCode(productCode);
    Effect*      e  = em.findByCode(productCode);
    Tuner*       t  = tm.findByCode(productCode);
    Amp*         a  = am.findByCode(productCode);
    PowerSupply* p  = pm.findByCode(productCode);
    Cable*       c  = cm.findByCode(productCode);
    Accessory*   ac = acm.findByCode(productCode);

    std::vector<std::function<bool()>> actions = {
        [&]() { if (g  && g->isStockAvailable(qty)) { g->reduceStock(qty); gm.saveToFile(); return true; } return false; },
        [&]() { if (e  && e->isStockAvailable(qty)) { e->reduceStock(qty); em.saveToFile(); return true; } return false; },
        [&]() { if (t  && t->isStockAvailable(qty)) { t->reduceStock(qty); tm.saveToFile(); return true; } return false; },
        [&]() { if (a  && a->isStockAvailable(qty)) { a->reduceStock(qty); am.saveToFile(); return true; } return false; },
        [&]() { if (p  && p->isStockAvailable(qty)) { p->reduceStock(qty); pm.saveToFile(); return true; } return false; },
        [&]() { if (c  && c->isStockAvailable(qty)) { c->reduceStock(qty); cm.saveToFile(); return true; } return false; },
        [&]() { if (ac && ac->isStockAvailable(qty)) { ac->reduceStock(qty); acm.saveToFile(); return true; } return false; }
    };

    bool success = false;
    for (auto& act : actions) {
        if (act()) {
            success = true;
            break;
        }
    }

    if (!success) {
        std::cerr << "\n존재하지 않거나 재고가 부족한 제품입니다.\n";
        return false;
    }

    int orderIdToUse = (currentBundleOrderId > 0) ? currentBundleOrderId : nextOrderId++;
    Order* order = new Order(orderIdToUse, clientId, productCode, qty, getTodayDateTime());
    orders.push_back(order);
    saveToFile();

    std::cout << "\n주문이 완료되었습니다.\n";
    return true;
}

bool OrderManager::refundOrder(int orderId, ProductSystem::ProductManager& productManager) {
    bool anyRefunded = false;
    for (auto& order : orders) {
        if (order->getOrderId() == orderId && order->getStatus() != "환불완료") {
            std::string code = order->getProductCode();
            int qty = order->getQuantity();

            auto& gm  = productManager.getGuitarManager();
            auto& em  = productManager.getEffectManager();
            auto& tm  = productManager.getTunerManager();
            auto& am  = productManager.getAmpManager();
            auto& pm  = productManager.getPowerSupplyManager();
            auto& cm  = productManager.getCableManager();
            auto& acm = productManager.getAccessoryManager();

            if (auto* g = gm.findByCode(code)) { g->addStock(qty); gm.saveToFile(); }
            else if (auto* e = em.findByCode(code)) { e->addStock(qty); em.saveToFile(); }
            else if (auto* t = tm.findByCode(code)) { t->addStock(qty); tm.saveToFile(); }
            else if (auto* a = am.findByCode(code)) { a->addStock(qty); am.saveToFile(); }
            else if (auto* p = pm.findByCode(code)) { p->addStock(qty); pm.saveToFile(); }
            else if (auto* c = cm.findByCode(code)) { c->addStock(qty); cm.saveToFile(); }
            else if (auto* ac = acm.findByCode(code)) { ac->addStock(qty); acm.saveToFile(); }

            order->setDate(getTodayDateTime());
            order->setStatus("환불완료");
            anyRefunded = true;
        }
    }

    if (anyRefunded) {
        saveToFile();
        std::cout << "\n묶음 주문 전체 환불이 완료되었습니다.\n";
        return true;
    } else {
        std::cerr << "\n해당 주문번호의 환불 가능한 항목이 없습니다.\n";
        return false;
    }
}

void OrderManager::loadFromFile() {
    std::ifstream file(ORDER_FILE);
    if (!file.is_open()) return;

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        int id, clientId, qty;
        std::string productCode, date, status;

        ss >> id; ss.ignore();
        ss >> clientId; ss.ignore();
        getline(ss, productCode, ',');
        ss >> qty; ss.ignore();
        getline(ss, date, ',');
        getline(ss, status);

        Order* order = new Order(id, clientId, productCode, qty, date, status);
        orders.push_back(order);
        if (id >= nextOrderId) nextOrderId = id + 1;
    }
}

void OrderManager::saveToFile() const {
    std::ofstream file(ORDER_FILE);
    for (const auto& order : orders) {
        file << order->getOrderId() << ","
             << order->getClientId() << ","
             << order->getProductCode() << ","
             << order->getQuantity() << ","
             << order->getDate() << ","
             << order->getStatus() << "\n";
    }
}

void OrderManager::showAllOrders(const ClientSystem::ClientManager& clientManager, const ProductSystem::ProductManager& productManager) const {
    while (true) {
        std::cout << "\n주문번호 | 고객ID   | 고객이름      | 제품코드   | 제품명(모델명)           | 수량     | 날짜                | 상태\n";
        std::cout << "------------------------------------------------------------------------------------------------------------------------\n";

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

            std::cout << std::setw(9) << order->getOrderId() << " | "
                      << std::setw(8) << clientIdStr << " | "
                      << std::setw(12) << clientName << " | "
                      << std::setw(10) << productCode << " | "
                      << std::setw(22) << productName << " | "
                      << std::setw(8) << addComma(order->getQuantity()) << " | "
                      << std::setw(19) << order->getDate() << " | "
                      << order->getStatus() << "\n";
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
                }
            }

            if (!found) {
                std::cout << "\n일치하는 항목이 없습니다.\n";
            }

            std::cout << "\n확인을 했다면 Enter 키를 입력하세요";
            std::cin.get();
            return;

        } else if (opt == 2) {
            return;
        } else {
            std::cout << "\n잘못된 선택입니다.\n";
        }
    }
}

std::string OrderManager::getTodayDateTime() const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d/%02d/%02d %02d:%02d:%02d",
             1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
             ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return std::string(buf);
}
}