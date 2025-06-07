#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include <string>

#include "guitarmanager.h"
#include "effectmanager.h"
#include "tunermanager.h"
#include "ampmanager.h"
#include "powersupplymanager.h"
#include "cablemanager.h"
#include "accessorymanager.h"
// 각각의 제품군 매니저 클래스를 포함, 이 클래스는 다양한 장비(앰프, 이펙터, 튜너 등) 를 모두 다루기 때문에 전체 포함 필요

namespace ProductSystem { // 전체 제품군은 ProductSystem 아래로 묶어서 이름 충돌 방지 + 구조화
class ProductManager { // 앰프, 케이블, 기타, 이펙터 등 모든 제품군을 총괄 관리하는 클래스
private:
    GuitarManager guitarManager;
    EffectManager effectManager;
    TunerManager tunerManager;
    AmpManager ampManager;
    PowerSupplyManager powerSupplyManager;
    CableManager cableManager;
    AccessoryManager accessoryManager;
    // 각 제품군 매니저들을 객체로 보유 → 객체가 포함되어 있기 때문에 생성자에서 자동 초기화

public:
    ProductManager(); // 파일 로딩/초기화 등을 위한 생성자
    ~ProductManager(); // 소멸자에서는 필요시 정리 작업 가능

    void saveToFile();                     // 각 제품군 파일로 저장
    void loadFromFile();                   // 각 제품군 파일로부터 로드
    bool displayMenu();                    // 콘솔 기반 메뉴

    GuitarManager& getGuitarManager() { return guitarManager; }
    const GuitarManager& getGuitarManager() const { return guitarManager; }

    EffectManager& getEffectManager() { return effectManager; }
    const EffectManager& getEffectManager() const { return effectManager; }

    TunerManager& getTunerManager() { return tunerManager; }
    const TunerManager& getTunerManager() const { return tunerManager; }

    AmpManager& getAmpManager() { return ampManager; }
    const AmpManager& getAmpManager() const { return ampManager; }

    PowerSupplyManager& getPowerSupplyManager() { return powerSupplyManager; }
    const PowerSupplyManager& getPowerSupplyManager() const { return powerSupplyManager; }

    CableManager& getCableManager() { return cableManager; }
    const CableManager& getCableManager() const { return cableManager; }

    AccessoryManager& getAccessoryManager() { return accessoryManager; }
    const AccessoryManager& getAccessoryManager() const { return accessoryManager; }

    // 각 제품군을 외부에서 접근할 수 있도록 참조 반환
    // const 버전과 비-const 버전을 함께 제공해서 사용 상황에 맞게 사용 가능

};
}
// 이 클래스는 OrderManager, GenreStarterSet 등과 연계하여 제품 조회/주문의 중심 역할을 수행

#endif // PRODUCTMANAGER_H