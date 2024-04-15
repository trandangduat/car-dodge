#ifndef BULLET_HPP
#define BULLET_HPP

#include "gamewindow.hpp"
#include "gamestate.hpp"
#include "assets.hpp"
#include "timer.hpp"

#define BULLET_NORMAL 0
#define BULLET_EXPLODED 1

class Bullet {
private:
    GameWindow* gWin = nullptr;
    GameState* gState = nullptr;
    SDL_Rect mRect;
    int mVelY;
    int mState;

public:
    Bullet() {};
    Bullet (GameWindow* gw, GameState* gs, int x, int y);
    void move (SDL_Point target, float dTime);
    void render();
    SDL_Rect getRect();
    void setState (int _state);
    int getState();
};

#endif // BULLET_HPP
