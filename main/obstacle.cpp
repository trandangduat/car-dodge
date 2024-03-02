#include "obstacle.hpp"

Obstacle::Obstacle (GameWindow* gw, float x, float y, float velocity, SDL_Rect clip) {
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
    this->mVelY = velocity;
    this->mIsCrashed = 0;
    this->mClip = clip;
}

void Obstacle::render (SDL_Texture* tex) {
    this->gwin->blit(tex, mClip, mRect);
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
}

bool Obstacle::isCrashed() {
    return this->mIsCrashed;
}


