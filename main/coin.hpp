#ifndef COIN_HPP
#define COIN_HPP

#include "gamewindow.hpp"
#include "timer.hpp"

class Coin {
private:
    GameWindow* gwin = nullptr;
    SDL_Rect mRect;
    bool mIsClaimed;
    // Animation
    SDL_Rect mClip;
    Timer* mTimer = nullptr;
    int mAnimStage;

public:
    static const int SPRITE_WIDTH = 20;
    static const int SPRITE_HEIGHT = 20;
    static const int SPRITE_STAGES = 9;

    Coin() {};
    Coin (GameWindow* gw, float x, float y);
    void render (SDL_Texture* tex);
    void setPos (float x, float y);
    void claimed();
    void animate();
    bool isClaimed();
    int posX();
    int posY();
    SDL_Rect getRect();
};

#endif // COIN_HPP
