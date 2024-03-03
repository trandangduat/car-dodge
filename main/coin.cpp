#include "coin.hpp"

Coin::Coin (GameWindow* gw, float x, float y) {
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, COIN_WIDTH, COIN_HEIGHT};
    this->mIsClaimed = 0;
    this->mClip = {0, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
    this->mAnimStage = 0;
    this->mTimer = new Timer;
    this->mTimer->start();
}

void Coin::render (SDL_Texture* tex) {
    this->gwin->blit(tex, this->mClip, this->mRect);
}

void Coin::setPos (float x, float y) {
    this->mRect.x = (int) x;
    this->mRect.y = (int) y;
}

void Coin::claimed() {
    this->mIsClaimed = 1;
}

void Coin::animate() {
    if (this->mTimer->elapsedTime() >= 100) {
        this->mAnimStage = (this->mAnimStage + 1) % SPRITE_STAGES;
        this->mClip.x = this->mAnimStage * SPRITE_WIDTH;
        this->mTimer->start();
    }
}

bool Coin::isClaimed() {
    return this->mIsClaimed;
}

int Coin::posX() {
    return this->mRect.x;
}

int Coin::posY() {
    return this->mRect.y;
}

SDL_Rect Coin::getRect() {
    return this->mRect;
}
