#include "obstacle.hpp"

Obstacle::Obstacle (GameWindow* gw, SDL_Texture* normalTex, SDL_Texture* crashedTex, SDL_Texture* crashedWhiteTex, float x, float y, float velocity, SDL_Rect clip) {
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
    this->mNormalTex = normalTex;
    this->mCrashedTex = crashedTex;
    this->mCrashedWhiteTex = crashedWhiteTex;
    this->mVelY = velocity;
    this->mIsCrashed = 0;
    this->mClip = clip;
    this->blinkTimer = new Timer;
    this->blinkSwitch = 0;
}

void Obstacle::render() {
    if (this->blinkTimer->elapsedTime() >= 200) return;
    SDL_Texture* renderTex = mNormalTex;
    if (this->isCrashed()) {
        if (this->blinkTimer->elapsedTime() % 50 == 0) {
            this->blinkSwitch = 1 - this->blinkSwitch;
        }
        renderTex = (!this->blinkSwitch ? mCrashedWhiteTex : mCrashedTex);
    }
    this->gwin->blit(renderTex, mClip, mRect);
}

void Obstacle::setPos (float x, float y) {
    this->mRect.x = (int) x;
    this->mRect.y = (int) y;
}

void Obstacle::setVelY (float velocity) {
    this->mVelY = velocity;
}

float Obstacle::getPosX() {
    return this->mRect.x;
}

float Obstacle::getPosY() {
    return this->mRect.y;
}

float Obstacle::getVelY() {
    return this->mVelY;
}

SDL_Rect Obstacle::getRect() {
    return this->mRect;
}

void Obstacle::crash() {
    this->mIsCrashed = 1;
    this->blinkTimer->start();
}

bool Obstacle::isCrashed() {
    return this->mIsCrashed;
}
