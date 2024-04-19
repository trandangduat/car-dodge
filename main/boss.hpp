#ifndef BOSS_HPP
#define BOSS_HPP

#define BOSS_MOVING 1
#define BOSS_DELAY 2
#define BOSS_ULTING 3

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
    Timer* mTimer = nullptr;
    Timer* mAnimationTimer = nullptr;
    int mState;

public:
    Boss() {};
    Boss (GameWindow* gw, SDL_Texture* tex);
    void move (int x, int y);
    void animate();
    void ult();
    void render();
    int getState();
    SDL_Rect getUltRect();
};

#endif // BOSS_HPP
