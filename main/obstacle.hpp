#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "gamewindow.hpp"

class Obstacle {
private:
    GameWindow* gwin = nullptr;
    SDL_Rect mRect, mClip;
    float mVelY;
    bool mIsCrashed;

public:
    Obstacle() {};
    Obstacle (GameWindow* gw, float x, float y, float velocity, SDL_Rect clip);
    void render (SDL_Texture* tex);
    void setPos (float x, float y);
    void setVelY (float velocity);
    float getPosX();
    float getPosY();
    float getVelY();
    SDL_Rect getRect();
    void crash();
    bool isCrashed();
};

#endif // OBSTACLE_HPP
