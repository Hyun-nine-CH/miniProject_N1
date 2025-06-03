#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <vector>
#include <string>
#include "order.h"
#include "clientmanager.h"
#include "productmanager.h"

namespace ClientSystem { class ClientManager; }
namespace ProductSystem { class ProductManager; }
namespace OrderSystem {
class OrderManager {
public:
    OrderManager();

    bool createOrder(int clientId, const std::string& productCode, int quantity,
                     ClientSystem::ClientManager& clientManager, ProductSystem::ProductManager& productManager); // 주문 생성

    bool refundOrder(int orderId, ProductSystem::ProductManager& productManager); // 환불

    void loadFromFile();
    void saveToFile() const;

    // ✅ 고객 이름 + 제품명 출력용 함수
    void showAllOrders(const ClientSystem::ClientManager& clientManager,
                       const ProductSystem::ProductManager& productManager) const;

    int getNextOrderId() const;
    std::string getTodayDateTime() const;

    Order* findOrderById(int orderId);
    void startNewOrderBundle();
    void endOrderBundle();

private:
    std::vector<Order*> orders;
    int nextOrderId;
    int currentBundleOrderId = 0;

    std::string getClientNameById(int clientId, const ClientSystem::ClientManager& cm) const;
};
}

#endif // ORDERMANAGER_H
