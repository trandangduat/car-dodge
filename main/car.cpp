#include "car.hpp"

Car::Car (GameWindow* gw, float x, float y, float velocity) {
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, CAR_WIDTH, CAR_HEIGHT};
    this->mVelY = velocity;
    this->mTiltedAngle = 0;
    this->mVisible = true;
    this->mIsHitByBoss = false;
}

void Car::getsHitByBossUltimate (bool state) {
    this->mIsHitByBoss = state;
}

bool Car::isGotHitByBossUltimate() {
    return this->mIsHitByBoss;
}

void Car::moveWithMouse() {
    int mX, mY;
    SDL_GetMouseState(&mX, &mY);
    moveTo(mX - mRect.w / 2, SCREEN_HEIGHT - 2 * mRect.h);
}

void Car::moveTo (float x, float y) {
    if (x < ROADSIDE_WIDTH) {
        x = ROADSIDE_WIDTH;
    }
    if (x + mRect.w > SCREEN_WIDTH - ROADSIDE_WIDTH) {
        x = SCREEN_WIDTH - ROADSIDE_WIDTH - mRect.w;
    }

    int dx = x - this->mRect.x;
    int dy = y - this->mRect.y;

    this->mTiltedAngle = atan(MOVEMENT_DELAY * dx / CAR_HEIGHT) * 180 / M_PI;
    if (this->mTiltedAngle >=  10) this->mTiltedAngle =  10;
    if (this->mTiltedAngle <= -10) this->mTiltedAngle = -10;

    this->mRect.x += MOVEMENT_DELAY * dx;
    this->mRect.y += MOVEMENT_DELAY * dy;
}

void Car::render (SDL_Texture* tex) {
    this->gwin->blit(tex, this->mRect, this->mTiltedAngle);
}

void Car::setVelY (float velocity) {
    this->mVelY = velocity;
}
void Car::setVisible (bool state) {
    this->mVisible = state;
}
void Car::setScale (float scale) {
    this->mRect.w = scale * CAR_WIDTH;
    this->mRect.h = scale * CAR_HEIGHT;
}
float Car::getPosX() {
    return this->mRect.x;
}
float Car::getPosY() {
    return this->mRect.y;
}
float Car::getVelY() {
    return this->mVelY;
}
SDL_Rect Car::getRect() {
    return this->mRect;
}
int Car::isVisible() {
    return this->mVisible;
}
