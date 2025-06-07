#ifndef GENRESTARTERSET_H
#define GENRESTARTERSET_H

#include <string>
#include <vector>
#include <map>

namespace ClientSystem { class ClientManager; }
namespace ProductSystem { class ProductManager; }
namespace OrderSystem  { class OrderManager; }
// 외부 클래스 전방 선언, 불필요한 헤더 포함 없이 포인터만 사용하기 위해 선언

namespace GenreStarter { // 프로젝트 내에서 GenreStarterSet 관련 기능을 별도 영역으로 구분

class GenreStarterSet { // 주요 기능: 장르 선택 → 추천 세트 → 자동 주문
public:
    GenreStarterSet(); // genreNames, genreConfigs 등을 초기화

    void displayGenreMenu(ClientSystem::ClientManager* clientManager,
                          ProductSystem::ProductManager* productManager,
                          OrderSystem::OrderManager* orderManager);
                          // 콘솔 기반 메뉴 UI 출력, 사용자에게 장르 선택을 유도 (예: 1. 하드락, 2. 펑크록...)

private:
    struct GenreConfig {
        std::string guitarType;
        std::vector<std::string> effectTypes;
    }; // 장르별 추천 세트 구성 구조체
    // 예: 헤비메탈 → "SuperStrat", {"Distortion", "NoiseGate", "Reverb"}

    std::map<int, std::string> genreNames; // genreNames: 메뉴 번호 ↔ 장르명
    std::map<int, GenreConfig> genreConfigs; // genreConfigs: 메뉴 번호 ↔ 구성 (기타 타입 + 이펙터 타입 목록)

    void processGenreSelection(int genreId,
                               ClientSystem::ClientManager* clientManager,
                               ProductSystem::ProductManager* productManager,
                               OrderSystem::OrderManager* orderManager);
                               // 장르를 고르면 해당 기타와 이펙터를 필터링하고 → 주문 생성까지 이어짐

    void loadGuitarMatches(const std::string& type); // 지정된 타입에 해당하는 기타 목록만 추려서 표시
    void loadEffectMatches(const std::vector<std::string>& types);
    // 장르에 필요한 이펙터 타입에 해당하는 모델만 필터링

    void createGuitarOrder(ClientSystem::ClientManager* clientManager,
                           ProductSystem::ProductManager* productManager,
                           OrderSystem::OrderManager* orderManager);

    void createEffectOrders(const std::vector<std::string>& types,
                            ClientSystem::ClientManager* clientManager,
                            ProductSystem::ProductManager* productManager,
                            OrderSystem::OrderManager* orderManager);
                            // 추천된 제품을 기반으로 OrderManager::createOrder 호출
                            // 자동 주문 처리 진행
};
}

#endif // GENRESTARTERSET_H
