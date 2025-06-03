#include <iostream>
#include "clientmanager.h"
#include "productmanager.h"
#include "ordermanager.h"
#include "genrestarterset.h"

using namespace std;
using namespace ClientSystem;
using namespace ProductSystem;
using namespace OrderSystem;
using namespace GenreStarter;

int main() {
    ClientManager clientManager;
    ProductManager productManager;
    OrderManager orderManager;
    GenreStarterSet starterSet;

    while (true) {
        cout << "\n========= Main Menu =========\n";
        cout << "1. 고객 관리\n";
        cout << "2. 제품 관리\n";
        cout << "3. 주문 생성\n";
        cout << "4. 주문 목록 보기\n";
        cout << "5. 주문 환불 처리\n";
        cout << "6. Genre Starter Set\n";  // ✅ 추가된 메뉴
        cout << "7. 종료\n";
        cout << "=============================\n";
        cout << "선택: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                while (clientManager.displayMenu());
                break;
            case 2:
                while (productManager.displayMenu());
                break;
            case 3: {
                int clientId, quantity;
                string productCode;

                cout << "고객 ID: ";
                cin >> clientId;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "숫자 입력이 필요합니다. 주문 생성을 취소합니다.\n";
                    break;
                }
                cout << "제품 코드: ";
                cin >> productCode;
                cout << "수량: ";
                cin >> quantity;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "숫자 입력이 필요합니다. 주문 생성을 취소합니다.\n";
                    break;
                }

                orderManager.createOrder(clientId, productCode, quantity, clientManager, productManager);
                break;
            }
            case 4:
                orderManager.showAllOrders(clientManager, productManager);
                break;
            case 5: {
                int orderId;
                cout << "환불할 주문번호 입력: ";
                cin >> orderId;
                orderManager.refundOrder(orderId, productManager);
                break;
            }
            case 6:
                starterSet.displayGenreMenu(&clientManager, &productManager, &orderManager);
                break;
            case 7:
                cout << "프로그램을 종료합니다.\n";
                return 0;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}