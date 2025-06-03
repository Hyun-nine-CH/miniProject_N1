#ifndef GENRESTARTERSET_H
#define GENRESTARTERSET_H

#include <string>
#include <vector>
#include <map>

namespace ClientSystem { class ClientManager; }
namespace ProductSystem { class ProductManager; }
namespace OrderSystem  { class OrderManager; }

namespace GenreStarter {

class GenreStarterSet {
public:
    GenreStarterSet();

    void displayGenreMenu(ClientSystem::ClientManager* clientManager,
                          ProductSystem::ProductManager* productManager,
                          OrderSystem::OrderManager* orderManager);

private:
    struct GenreConfig {
        std::string guitarType;
        std::vector<std::string> effectTypes;
    };

    std::map<int, std::string> genreNames;
    std::map<int, GenreConfig> genreConfigs;

    void processGenreSelection(int genreId,
                               ClientSystem::ClientManager* clientManager,
                               ProductSystem::ProductManager* productManager,
                               OrderSystem::OrderManager* orderManager);

    void loadGuitarMatches(const std::string& type);
    void loadEffectMatches(const std::vector<std::string>& types);

    void createGuitarOrder(ClientSystem::ClientManager* clientManager,
                           ProductSystem::ProductManager* productManager,
                           OrderSystem::OrderManager* orderManager);

    void createEffectOrders(const std::vector<std::string>& types,
                            ClientSystem::ClientManager* clientManager,
                            ProductSystem::ProductManager* productManager,
                            OrderSystem::OrderManager* orderManager);
};

}

#endif // GENRESTARTERSET_H
