#include "vfx.hpp"

VFX::VFX (GameWindow* gw, float x, float y, int w, int h, SDL_Texture* tex, int sprite_w, int sprite_h) {
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, w, h};
    this->mClip = {0, 0, sprite_w, sprite_h};
    this->mTex = tex;
    this->mTimer = new Timer;
    this->mTimer->start();
    this->mAnimStage = 0;
}

void VFX::setPos (float x, float y) {
    this->mRect.x = (int) x;
    this->mRect.y = (int) y;
}

void VFX::animate() {
    if (this->mTimer->elapsedTime() >= 50) {
        int textureW, textureH;
        SDL_QueryTexture(this->mTex, nullptr, nullptr, &textureW, &textureH);
        int spriteStages = textureW / this->mClip.w;
        this->mAnimStage = (this->mAnimStage + 1) % spriteStages;
        this->mClip.x = this->mAnimStage * mClip.w;
        this->mTimer->start();
    }
}

void VFX::render (double angle, SDL_RendererFlip flip) {
//    this->gwin->blit(this->mTex, this->mClip, this->mRect);
    SDL_RenderCopyEx(this->gwin->gRenderer, this->mTex, &(this->mClip), &(this->mRect), angle, nullptr, flip);
}
