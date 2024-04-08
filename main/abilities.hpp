#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include "timer.hpp"
#include "gamestate.hpp"
#include "hud.hpp"

struct Ability {
    std::string name;
    std::string desc;
    int coins;
    int id;
    Timer* timer = nullptr;
    int duration; // in seconds
    bool isActive;

    Ability() {};
    Ability (std::string _name, std::string _desc, int _coins, int _duration, int _id);
};

extern std::vector<std::vector<Ability>> abils;

void loadAbilitiesFromFiles();
void activeAbility (GameState* gState, int tier, int id);
void renderActiveAbilities (GameWindow* gWin, HUD* gHud);
void updateActiveAbilities();

#endif // ABILITIES_HPP
