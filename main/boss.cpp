#include "boss.hpp"

Boss::Boss (GameWindow* gw, SDL_Texture* tex) {
    this->gwin = gw;
    this->mTex = tex;
    this->mRect = {0, 0, BOSS_WIDTH, BOSS_HEIGHT};
    this->mClip = {0, 0, 0, 0};
    this->mCooldownTimer = new Timer;
    this->mAnimationTimer = new Timer;
    this->mUltTimer = new Timer;
    this->mCooldownTimer->start();
    this->mAnimationTimer->start();
}

void Boss::move (int x, int y) {
    if (this->mCooldownTimer->elapsedTime() >= 5000) {
        this->mCooldownTimer->reset();
    }
    float speed = 0.05; // the lower this is, the slower the boss moves
    if (abs(this->mRect.x - destination.x) <= 1 / speed && abs(this->mRect.y - destination.y) <= 1 / speed) {
        if (this->mCooldownTimer->isPaused())
            this->destination = {x, y};
        else
            // if the boss is in cooldown then it moves randomly
            this->destination = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT};
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
    if (this->mUltTimer->isPaused()) {
        this->mUltTimer->reset();
        this->mUltTimer->start();
    }
    this->mUltimateRect = {this->mRect.x, this->mRect.y + this->mRect.h, this->mRect.w, SCREEN_HEIGHT};
    if (this->mUltTimer->elapsedTime() >= 2000) {
        this->mUltTimer->pause();
        this->mCooldownTimer->reset();
        this->mCooldownTimer->start();
    }
}

void Boss::render() {
    SDL_RenderDrawRect(this->gwin->gRenderer, &this->mRect);
    if (!this->isInCoolDown()) {
        SDL_SetRenderDrawColor(this->gwin->gRenderer, 200, 100, 60, 255);
        SDL_RenderFillRect(this->gwin->gRenderer, &this->mUltimateRect);
    }
}

bool Boss::isInCoolDown() {
    return this->mCooldownTimer->isPaused() == false;
}
