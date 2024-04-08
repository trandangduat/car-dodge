#include "abilities.hpp"
#include <fstream>
#include <iostream>

std::vector<std::vector<Ability>> abils(3);

Ability::Ability (std::string _name, std::string _desc, int _coins, int _duration, int _id) {
    this->name = _name;
    this->desc = _desc;
    this->coins = _coins;
    this->timer = new Timer;
    this->duration = _duration;
    this->id = _id;
    this->isActive = 0;
}


void loadAbilitiesFromFiles () {
    for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
        std::string path = "abilities/tier";
        path += (char) (tier + 1 + '0');
        path += ".txt";
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
}

void activeAbility (GameState* gState, int tier, int id) {
    if (!abils[tier][id].isActive) {
        abils[tier][id].isActive = 1;
    }
    abils[tier][id].timer->start();
    gState->updateCoins(gState->currentCoins() - abils[tier][id].coins);
}

void renderActiveAbilities (GameWindow* gWin, HUD* gHud) {
    int y = SCREEN_HEIGHT - 50;
    for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
        for (Ability& A : abils[tier]) {
            if (A.isActive) {
                gHud->drawTTFText(gWin->AvenuePixel, A.name, 25, 30, y, {255, 255, 255});
                y -= 25;
            }
        }
    }
}

void updateActiveAbilities() {
    for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
        for (Ability& A : abils[tier]) {
            /*
                If the ability is deactivated or reaching its duration,
                we deactive it
            */
            if (!A.isActive || A.timer->elapsedTime() >= A.duration * 1000) {
                A.isActive = 0;
            }
        }
    }
}
