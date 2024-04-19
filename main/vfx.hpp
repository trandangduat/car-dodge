#ifndef VFX_HPP
#define VFX_HPP

#include "gamewindow.hpp"
#include "timer.hpp"

struct VFX {
    SDL_Rect mRect;
    SDL_Rect mClip;
    GameWindow* gwin = nullptr;
    Timer* mTimer = nullptr;
    SDL_Texture* mTex = nullptr;
    int mAnimStage;
    int mAnimDelay;

    VFX() {};
    VFX (GameWindow* gw, float x, float y, int w, int h, SDL_Texture* tex, int sprite_w, int sprite_h, int animation_delay);
    void setPos (float x, float y);
    void animate();
    void render (double angle, SDL_RendererFlip flip);
};

#endif // VFX_HPP
