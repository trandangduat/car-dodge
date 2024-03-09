#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include <vector>
#include <string>
#include "timer.hpp"

struct Ability {
    std::string name;
    std::string desc;
    int coins;
    int id;
    Timer* timer = nullptr;

    Ability() {};
    Ability (std::string _name, std::string _desc, int _coins, int _id);
};

extern std::vector<std::vector<Ability>> abils;

void loadAbilities (int tier, std::string path);

#endif // ABILITIES_HPP
