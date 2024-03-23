#include "abilities.hpp"
#include <fstream>
#include <iostream>

Ability::Ability (std::string _name, std::string _desc, int _coins, int _id) {
    this->name = _name;
    this->desc = _desc;
    this->coins = _coins;
    this->id = _id;
    this->isActive = 0;
}

std::vector<std::vector<Ability>> abils(3);
std::vector<std::deque<int>> activeAbils(3);

void loadAbilities (int tier, std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open: " << path << '\n';
        return;
    }

    int id = 0;
    std::string line;

    while (std::getline(file, line)) {
        std::string name, desc;
        int coins = 0;
        int i = 0;
        while (i < (int) line.size() && line[i] != '$') {
            name += line[i];
            i++;
        }
        i++;
        while (i < (int) line.size() && line[i] != '$') {
            desc += line[i];
            i++;
        }
        i++;
        while (i < (int) line.size()) {
            coins = coins * 10 + (int) (line[i] - '0');
            i++;
        }
        Ability newAbil (name, desc, coins, ++id);

        abils[tier].push_back(newAbil);
    }

    file.close();
}

void activeAbility (GameState* gState, int tier, int id) {
    if (!abils[tier][id - 1].isActive) {
        abils[tier][id - 1].isActive = 1;
        activeAbils[tier].push_back(id);
    }
    abils[tier][id - 1].timer->start();
    gState->updateCoins(gState->currentCoins() - abils[tier][id - 1].coins);
}
