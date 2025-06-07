#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <vector>
#include <string>
#include "order.h"
#include "clientmanager.h"
#include "productmanager.h"

namespace ClientSystem { class ClientManager; }
namespace ProductSystem { class ProductManager; }
// 클래스 선언 전방 선언(forward declaration)
// ClientManager, ProductManager를 전체 포함하지 않고 포인터/참조로만 사용할 수 있도록 함

namespace OrderSystem { // 이 클래스는 여러 개의 주문을 저장하고 관리하는 관리자
class OrderManager { // 주문 추가/환불/저장/불러오기/검색/출력까지 맡음
public:
    OrderManager(); // nextOrderId 등의 초기값을 설정하고, 필요 시 파일을 읽어 주문을 불러올 수 있도록 구성

    bool createOrder(int clientId, const std::string& productCode, int quantity,
                     ClientSystem::ClientManager& clientManager, ProductSystem::ProductManager& productManager);
                     // 주문을 생성하는 함수, 유효한 고객 ID와 제품 코드인지 검사 후 주문 생성, 수량 확인 및 재고 차감 포함

    bool refundOrder(int orderId, ProductSystem::ProductManager& productManager);
    // 특정 주문을 환불, 상태를 "환불완료"로 바꾸고, 제품 재고를 다시 증가시킴

    void loadFromFile();
    void saveToFile() const;

    void showAllOrders(const ClientSystem::ClientManager& clientManager,
                       const ProductSystem::ProductManager& productManager) const;
                       // 전체 주문 목록을 출력, 고객 이름, 제품명까지 함께 보여주기 위해 외부 매니저를 받음

    int getNextOrderId() const; // 다음 주문 번호를 반환 (1, 2, 3 등 자동 증가)
    std::string getTodayDateTime() const; // 현재 시간과 날짜를 문자열로 반환

    Order* findOrderById(int orderId); // 특정 주문번호로 주문을 검색하여 포인터 반환
    void startNewOrderBundle();
    void endOrderBundle();
    // 복수 주문을 한 고객 기준으로 묶어서 처리할 때 시작/종료를 표시

private:
    std::vector<Order*> orders; // 모든 주문을 저장한 리스트 (포인터 사용)
    int nextOrderId; // 자동 증가되는 주문 번호
    int currentBundleOrderId = 0; // 묶음 주문 처리 시 동일 주문번호 유지용 (ex: 장바구니처럼 사용 가능)

    std::string getClientNameById(int clientId, const ClientSystem::ClientManager& cm) const;
    // 고객 ID를 통해 고객 이름을 얻기 위한 헬퍼 함수
};
}

#endif // ORDERMANAGER_H
