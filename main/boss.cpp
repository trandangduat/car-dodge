#include "boss.hpp"

Boss::Boss (GameWindow* gw) {
    this->gwin = gw;
    this->mRect = {0, 0, BOSS_WIDTH, BOSS_HEIGHT};
    this->mTimer = new Timer;
    this->mAnimationTimer = new Timer;
    this->reset();
}

void Boss::reset() {
    this->mTimer->start();
    this->mAnimationTimer->start();
    this->mState = BOSS_MOVING;
    this->mAnimStage = 0;
    this->mFlip = SDL_FLIP_NONE;
    this->mAngle = 0;
}

void Boss::updateTexture (SDL_Texture* tex, SDL_Texture* nearUltTex, int sprite_width, int sprite_height) {
    this->mTex = tex;
    this->mNearUltTex = nearUltTex;
    this->mClip = {0, 0, sprite_width, sprite_width};
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
    float speed = 0.03;
    if (abs(this->mRect.x - destination.x) <= 1 / speed && abs(this->mRect.y - destination.y) <= 1 / speed) {
        if (this->mState == BOSS_MOVING || (this->mState == BOSS_DELAY && this->mTimer->elapsedTime() < 1000)) {
            if (this->mState == BOSS_DELAY) {
                this->destination = {x, y};
            }
            else {
                this->destination = {rand() % SCREEN_WIDTH, rand() % 200};
            }
        }
    }
    float dx = this->destination.x - this->mRect.x;
    float dy = this->destination.y - this->mRect.y;
    this->mRect.x += speed * dx;
    this->mRect.y += speed * dy;
    this->mFlip = (dx < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    this->mAngle = atan2(abs(dy), abs(dx)) * 180 / M_PI;
}

void Boss::animate() {
    if (this->mAnimationTimer->elapsedTime() >= 100) {
        int textureW, textureH;
        SDL_QueryTexture(
            this->mState == BOSS_MOVING ? this->mTex : this->mNearUltTex,
            nullptr,
            nullptr,
            &textureW,
            &textureH
        );
        int spriteStages = textureW / this->mClip.w;
        this->mAnimStage = (this->mAnimStage + 1) % spriteStages;
        this->mClip.x = this->mAnimStage * mClip.w;
        this->mAnimationTimer->start();
    }
}

void Boss::ult() {
    this->mUltimateRect = {
        this->mRect.x + this->mRect.w / 2,
        this->mRect.y + this->mRect.h - 10,
        this->mRect.w / 3,
        this->mRect.h * 5
    };
    this->mUltimateRect.x -= this->mUltimateRect.w / 2;
    this->mAngle = (this->mFlip == SDL_FLIP_NONE ? 60 : -60);
    if (this->mState == BOSS_ULTING && this->mTimer->elapsedTime() >= 1500) {
        this->mState = BOSS_MOVING;
        this->mTimer->start();
    }
}

void Boss::render() {
//    SDL_RenderDrawRect(this->gwin->gRenderer, &this->mRect);
//    if (this->mState == BOSS_ULTING) {
//        SDL_SetRenderDrawColor(this->gwin->gRenderer, 0, 0, 0, 255);
//        SDL_RenderFillRect(this->gwin->gRenderer, &this->mUltimateRect);
//    }
    SDL_RenderCopyEx(
        this->gwin->gRenderer,
        this->mState == BOSS_MOVING ? this->mTex : this->mNearUltTex,
        &this->mClip,
        &this->mRect,
        this->mAngle,
        nullptr,
        this->mFlip
    );
}

int Boss::getState() {
    return this->mState;
}

SDL_Rect Boss::getUltRect() {
    return this->mUltimateRect;
}
