#include "bullet.hpp"

Bullet::Bullet (GameWindow* gw, GameState* gs, int x, int y) {
    this->gWin = gw;
    this->gState = gs;
    this->mRect = {x, y, BULLET_WIDTH, BULLET_HEIGHT};
    this->mVelY = - BULLET_VELOCITY;
    this->mState = BULLET_NORMAL;
}

void Bullet::move (SDL_Point target, float dTime) {
    if (target.x == oo || target.y == oo) {
        this->mRect.y += this->mVelY * dTime;
        return;
    }
    float dx = target.x - this->mRect.x;
    float dy = target.y - this->mRect.y;
    float delay = 0.2f;
    this->mRect.x += dx * delay;
    this->mRect.y += dy * delay;
}

void Bullet::render() {
    this->gWin->blit(bulletTexture, this->mRect);
}

SDL_Rect Bullet::getRect() {
    return this->mRect;
}

void Bullet::setState (int _state) {
    this->mState = _state;
}

int Bullet::getState() {
    return this->mState;
}
