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

namespace ProductSystem {
class ProductManager {
private:
    GuitarManager guitarManager;
    EffectManager effectManager;
    TunerManager tunerManager;
    AmpManager ampManager;
    PowerSupplyManager powerSupplyManager;
    CableManager cableManager;
    AccessoryManager accessoryManager;

public:
    ProductManager();
    ~ProductManager();

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

};
}

#endif // PRODUCTMANAGER_H