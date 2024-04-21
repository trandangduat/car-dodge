#ifndef CAR_HPP
#define CAR_HPP

#include "gamewindow.hpp"

class Car {
private:
    GameWindow* gwin = nullptr;
    SDL_Rect mRect;
    float mVelY;
    int mTiltedAngle;
    bool mVisible;
    bool mIsHitByBoss;

public:
    Car() {};
    Car (GameWindow* gw);
    void reset();
    void moveWithMouse();
    void moveTo (float x, float y);
    void render (SDL_Texture* tex);
    void setVelY (float velocity);
    void setVisible (bool state);
    void setScale (float scale);
    float getPosX();
    float getPosY();
    float getVelY();
    SDL_Rect getRect();
    int isVisible();
    void getsHitByBossUltimate (bool state);
    bool isGotHitByBossUltimate();
};

#endif // CAR_HPP
