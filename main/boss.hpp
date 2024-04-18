#ifndef BOSS_HPP
#define BOSS_HPP

#include "gamewindow.hpp"
#include "timer.hpp"

class Boss {
private:
    GameWindow* gwin = nullptr;
    SDL_Texture* mTex = nullptr;
    SDL_Rect mRect;
    SDL_Rect mClip;
    SDL_Rect mUltimateRect;
    SDL_Point destination;
    Timer* mCooldownTimer = nullptr;
    Timer* mAnimationTimer = nullptr;
    Timer* mUltTimer = nullptr;

public:
    Boss() {};
    Boss (GameWindow* gw, SDL_Texture* tex);
    void move (int x, int y);
    void animate();
    void ult();
    void render();
    bool isInCoolDown();
};

#endif // BOSS_HPP
