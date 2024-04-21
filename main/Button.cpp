#include "button.hpp"

Button::Button (GameWindow* win, SDL_Rect rect) {
    this->gwin = win;
    this->mRect = rect;
    this->mState = BUTTON_NORMAL;
}

void Button::updateTexture (SDL_Texture* tex, int source_button_width, int source_button_height) {
    this->mClip = {0, 0, source_button_width, source_button_height};
    this->mTex = tex;
}

bool Button::isOnHoverByPoint (int x, int y) {
    bool check = (mRect.x <= x && x <= mRect.x + mRect.w) and
                (mRect.y <= y && y <= mRect.y + mRect.h);

    if (check && this->mState == BUTTON_NORMAL) {
        this->mState = BUTTON_HOVER;
    }
    else if (!check && this->mState == BUTTON_HOVER) {
        this->mState = BUTTON_NORMAL;
    }

    return check;
}

bool Button::isDisabled() {
    return this->mState == BUTTON_DISABLED;
}

bool Button::onHover() {
    return this->mState == BUTTON_HOVER;
}

bool Button::onClicked() {
    return this->mState == BUTTON_CLICKED;
}

void Button::reset() {
    this->mState = BUTTON_NORMAL;
}

void Button::click() {
    if (this->mState == BUTTON_DISABLED) return;
    this->mState = BUTTON_CLICKED;
}

void Button::render() {
    if (this->mTex == nullptr) return;
    int texture_w, texture_h;
    SDL_QueryTexture(this->mTex, nullptr, nullptr, &texture_w, &texture_h);

    this->mClip.y = this->mClip.h * this->mState;
    if (this->mClip.y > texture_h) {
        this->mClip.y = texture_h - this->mClip.h;
    }
    this->gwin->blit(this->mTex, this->mClip, this->mRect);
}

void Button::disable() {
    this->mState = BUTTON_DISABLED;
}
