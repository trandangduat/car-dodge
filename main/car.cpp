#include "car.hpp"

Car::Car (GameWindow* gw, float x, float y, float velocity) {
    this->gwin = gw;
    this->mRect = {(int) x, (int) y, CAR_WIDTH, CAR_HEIGHT};
    this->mVelY = velocity;
    this->mTiltedAngle = 0;
    this->mVisible = true;
}

void Car::handleEvent (SDL_Event* e) {
    if (e->type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        moveTo(x - CAR_WIDTH / 2, SCREEN_HEIGHT - 2 * CAR_HEIGHT);
    }
}

void Car::moveTo (float x, float y) {
    if (x < ROADSIDE_WIDTH) {
        x = ROADSIDE_WIDTH;
    }
    if (x + CAR_WIDTH > SCREEN_WIDTH - ROADSIDE_WIDTH) {
        x = SCREEN_WIDTH - ROADSIDE_WIDTH - CAR_WIDTH;
    }
    if (this->mRect.x < x) {
        this->mTiltedAngle = std::min(7.0, atan(1.0 * (x - mRect.x) / CAR_HEIGHT) * 180 / PI);
    }
    else {
        this->mTiltedAngle = - std::min(7.0, atan(1.0 * (mRect.x - x) / CAR_HEIGHT) * 180 / PI);
    }
    if (fabs(this->mTiltedAngle) < 5) {
        this->mTiltedAngle = 0;
    }
    this->mRect.x = x;
    this->mRect.y = y;
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
