#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "gamewindow.hpp"
#include "timer.hpp"

class Obstacle {
private:
    GameWindow* gwin = nullptr;
    SDL_Rect mRect, mClip;
    float mVelY;
    bool mIsCrashed;
    Timer* blinkTimer = nullptr;
    int blinkSwitch;
    SDL_Texture* mNormalTex = nullptr;
    SDL_Texture* mCrashedTex = nullptr;
    SDL_Texture* mCrashedWhiteTex = nullptr;


public:
    Obstacle() {};
    Obstacle (GameWindow* gw, SDL_Texture* normalTex, SDL_Texture* crashedTex, SDL_Texture* crashedWhiteTex, float x, float y, float velocity, SDL_Rect clip);
    void render();
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
