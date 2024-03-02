#ifndef PARAMS_HPP
#define PARAMS_HPP

#define GAME_TITLE "CarDodge"

const double PI = 3.14159265;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 600;
const int ROADSIDE_WIDTH = 90;
const int CAR_WIDTH = 50;
const int CAR_HEIGHT = 100;
const int INIT_VELOCITY = 6 * 60;
const int OBSTACLE_WIDTH = 50;
const int OBSTACLE_HEIGHT = 100;
const int NUMBER_OF_COLUMNS = 4;
const int ITEM_WIDTH = 50;
const int ITEM_HEIGHT = ITEM_WIDTH;
const int NUMBER_OF_LIVES = 3;
const int SPEED_BOOST_AMOUNT = 3 * 60;
enum item_type {
    SPEED_BOOST_ITEM,
    INVISIBLE_ITEM,
    TOTAL_OF_ITEMS
};

#endif // PARAMS_HPP
