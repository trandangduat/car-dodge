#include "boss.hpp"

Boss::Boss (GameWindow* gw, SDL_Texture* tex) {
    this->gwin = gw;
    this->mTex = tex;
    this->mRect = {0, 0, BOSS_WIDTH, BOSS_HEIGHT};
    this->mClip = {0, 0, 0, 0};
    this->mTimer = new Timer;
    this->mAnimationTimer = new Timer;
    this->mAnimationTimer->start();
    this->mTimer->start();
    this->mState = BOSS_MOVING;
}

void Boss::move (int x, int y) {
    if (this->mState == BOSS_MOVING && this->mTimer->elapsedTime() >= 5000) {
        this->mState = BOSS_DELAY;
        this->mTimer->start();
    }
    if (this->mState == BOSS_DELAY && this->mTimer->elapsedTime() >= 2000) {
        this->mState = BOSS_ULTING;
        this->mTimer->start();
    }
    if (this->mState == BOSS_ULTING) {
        return;
    }
    float speed = 0.04;
    if (abs(this->mRect.x - destination.x) <= 1 / speed && abs(this->mRect.y - destination.y) <= 1 / speed) {
        if (this->mState == BOSS_MOVING || (this->mState == BOSS_DELAY && this->mTimer->elapsedTime() < 1000)) {
            if (this->mState == BOSS_DELAY) {
                this->destination = {x, y};
            }
            else {
                // if the boss is in cooldown then it moves randomly
                this->destination = {rand() % SCREEN_WIDTH, rand() % 200};
            }
        }
    }
    float dx = this->destination.x - this->mRect.x;
    float dy = this->destination.y - this->mRect.y;
    this->mRect.x += speed * dx;
    this->mRect.y += speed * dy;
}

void Boss::animate() {

}

void Boss::ult() {
//    std::cout << "ULTTTTTT\n";
    this->mUltimateRect = {this->mRect.x, this->mRect.y + this->mRect.h, this->mRect.w, SCREEN_HEIGHT};
    if (this->mState == BOSS_ULTING && this->mTimer->elapsedTime() >= 1500) {
        this->mState = BOSS_MOVING;
        this->mTimer->start();
    }
}

void Boss::render() {
    SDL_RenderDrawRect(this->gwin->gRenderer, &this->mRect);
    if (this->mState == BOSS_ULTING) {
        SDL_SetRenderDrawColor(this->gwin->gRenderer, 200, 100, 60, 255);
        SDL_RenderFillRect(this->gwin->gRenderer, &this->mUltimateRect);
    }
}

int Boss::getState() {
    return this->mState;
}
