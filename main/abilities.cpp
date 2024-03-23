#include "abilities.hpp"
#include <fstream>
#include <iostream>

Ability::Ability (std::string _name, std::string _desc, int _coins, int _duration, int _id) {
    this->name = _name;
    this->desc = _desc;
    this->coins = _coins;
    this->timer = new Timer;
    this->duration = _duration;
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
        Ability newAbil (name, desc, coins, 10, id++);

        abils[tier].push_back(newAbil);
    }

    file.close();
}

void activeAbility (GameState* gState, int tier, int id) {
    if (!abils[tier][id].isActive) {
        abils[tier][id].isActive = 1;
        activeAbils[tier].push_back(id);
    }
    abils[tier][id].timer->start();
    gState->updateCoins(gState->currentCoins() - abils[tier][id].coins);
}

void renderActiveAbilities (GameWindow* gWin, HUD* gHud) {
    int y = SCREEN_HEIGHT - 50;
    for (int tier = 0; tier < NUMBER_OF_ITEM_TIER; tier++) {
        for (int id : activeAbils[tier]) {
            gHud->drawTTFText(gWin->AvenuePixel, abils[tier][id].name, 25, 30, y, {255, 255, 255});
            y -= 25;
        }
    }
}

void updateActiveAbilities() {
    for (int tier = 0; tier < NUMBER_OF_ITEM_TIER; tier++) {
        std::vector<int> deletedIndex;
        int iter = 0;
        for (int id : activeAbils[tier]) {
            if (abils[tier][id].timer->elapsedTime() >= abils[tier][id].duration * 1000) {
                deletedIndex.push_back(iter);
            }
            iter++;
        }
        for (int i = (int) deletedIndex.size() - 1; i >= 0; i--) {
            std::cout << "ability deleted\n";
            activeAbils[tier].erase(activeAbils[tier].begin() + deletedIndex[i]);
        }
    }
}
