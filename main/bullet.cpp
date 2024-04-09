#include "bullet.hpp"

Bullet::Bullet (GameWindow* gw, GameState* gs, int x, int y) {
    this->gWin = gw;
    this->gState = gs;
    this->mRect = {x, y, BULLET_WIDTH, BULLET_HEIGHT};
    this->mVelY = - BULLET_VELOCITY;
    this->mState = BULLET_NORMAL;
}

void Bullet::move (float dTime) {
    this->mRect.y += this->mVelY * dTime;
}

void Bullet::render() {
    this->gWin->blit(bulletTexture, this->mRect, -90);
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
