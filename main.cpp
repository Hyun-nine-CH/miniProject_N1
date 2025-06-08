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
// 네임스페이스 안에 있는 클래스/함수를 이름 생략하고 사용할 수 있게 함

int main() {
    ClientManager clientManager;
    ProductManager productManager;
    OrderManager orderManager;
    GenreStarterSet starterSet;
    // 각각의 주요 매니저 클래스 인스턴스를 생성
    // 생성자 안에서 자동으로 파일 로딩이 진행됨 (loadFromFile())

    while (true) {
        cout << "\n========= Main Menu =========\n";
        cout << "1. 고객 관리\n";
        cout << "2. 제품 관리\n";
        cout << "3. 주문 생성\n";
        cout << "4. 주문 목록 보기\n";
        cout << "5. 주문 환불 처리\n";
        cout << "6. Genre Starter Set\n";
        cout << "7. 종료\n";
        cout << "=============================\n";
        cout << "선택: ";

        int choice;
        cin >> choice; // choice에 따라 아래 switch 문으로 분기

        switch (choice) {
            case 1:
                while (clientManager.displayMenu()); // displayMenu()가 true를 리턴하는 한 계속 반복
                break;
            case 2:
                while (productManager.displayMenu()); // 일렉기타, 이펙터, 케이블 등 각각의 제품군 메뉴로 연결됨
                break;
            case 3: {
                int clientId, quantity;
                string productCode;
                // 입력을 받아 주문 생성

                cout << "고객 ID: ";
                cin >> clientId;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "숫자 입력이 필요합니다. 주문 생성을 취소합니다.\n";
                    // cin에 잘못된 값이 들어왔을 때(문자 등) 입력 스트림 초기화 및 에러 메시지 출력
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
                // 고객 이름, 제품 모델명도 함께 표시됨
                break;
            case 5: {
                int orderId; // 특정 주문 ID에 대해 환불 처리
                cout << "환불할 주문번호 입력: ";
                cin >> orderId;
                orderManager.refundOrder(orderId, productManager);
                // 내부에서 해당 제품 재고 복원 + 상태 변경 처리
                break;
            }
            case 6:
                starterSet.displayGenreMenu(&clientManager, &productManager, &orderManager);
                // 자동 장르 기반 스타터 세트 기능 실행
                // 이 안에서 일렉기타/이펙터 추천 + 주문까지 처리됨
                // 포인터를 넘겨서 내부에서 시스템을 제어 가능하게 함
                break;
            case 7:
                cout << "프로그램을 종료합니다.\n";
                return 0; // main() 종료 → 프로그램 완전히 종료됨
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}