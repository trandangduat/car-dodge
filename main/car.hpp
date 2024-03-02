#ifndef CAR_HPP
#define CAR_HPP

#include "gamewindow.hpp"

class Car {
private:
    GameWindow* gwin = nullptr;
    SDL_Rect mRect;
    float mVelY;
    double mTiltedAngle;
    bool mVisible;

public:
    Car() {};
    Car (GameWindow* gw, float x, float y, float velocity);
    void handleEvent (SDL_Event* e);
    void moveTo (float x, float y);
    void render (SDL_Texture* tex);
    void setVelY (float velocity);
    void setVisible (bool state);
    float getPosX();
    float getPosY();
    float getVelY();
    SDL_Rect getRect();
    int isVisible();
};

#endif // CAR_HPP
