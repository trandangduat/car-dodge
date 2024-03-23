#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include <vector>
#include <deque>
#include <string>
#include "timer.hpp"
#include "gamestate.hpp"

struct Ability {
    std::string name;
    std::string desc;
    int coins;
    int id;
    Timer* timer = new Timer;
    bool isActive;

    Ability() {};
    Ability (std::string _name, std::string _desc, int _coins, int _id);
};

extern std::vector<std::vector<Ability>> abils;
extern std::vector<std::deque<int>> activeAbils;

void loadAbilities (int tier, std::string path);
void activeAbility (GameState* gState, int tier, int id);

#endif // ABILITIES_HPP
