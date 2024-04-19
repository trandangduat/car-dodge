#include "vfx.hpp"

VFX::VFX (GameWindow* gw, float x, float y, int w, int h, SDL_Texture* tex, int sprite_w, int sprite_h, int animation_delay) {
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, w, h};
    this->mClip = {0, 0, sprite_w, sprite_h};
    this->mTex = tex;
    this->mTimer = new Timer;
    this->mTimer->start();
    this->mAnimStage = 0;
    this->mAnimDelay = animation_delay;
}

void VFX::setPos (float x, float y) {
    this->mRect.x = (int) x;
    this->mRect.y = (int) y;
}

void VFX::animate() {
    if (this->mTimer->elapsedTime() >= this->mAnimDelay) {
        int textureW, textureH;
        SDL_QueryTexture(this->mTex, nullptr, nullptr, &textureW, &textureH);
        bool horizontalSprite = (textureH == this->mClip.h);
        int spriteStages;
        if (horizontalSprite) {
            spriteStages = textureW / this->mClip.w;
            this->mAnimStage = (this->mAnimStage + 1) % spriteStages;
            this->mClip.x = this->mAnimStage * mClip.w;
        }
        else {
            spriteStages = textureH / this->mClip.h;
            this->mAnimStage = (this->mAnimStage + 1) % spriteStages;
            this->mClip.y = this->mAnimStage * mClip.h;
        }
        this->mTimer->start();
    }
}

void VFX::render (double angle, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(this->gwin->gRenderer, this->mTex, &(this->mClip), &(this->mRect), angle, nullptr, flip);
}
