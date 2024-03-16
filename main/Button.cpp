#include "button.hpp"

Button::Button (GameWindow* win, SDL_Rect rect, SDL_Texture* normalTex, SDL_Texture* hoverTex, SDL_Texture* clickedTex) {
    this->gwin = win;
    this->mRect = rect;
    this->mNormal = normalTex;
    this->mHover = hoverTex;
    this->mClicked = clickedTex;
    this->mState = BUTTON_NORMAL;
}

bool Button::isPointInsideButton (int x, int y) {
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
    SDL_Texture* tex = nullptr;
    switch (this->mState) {
        case BUTTON_NORMAL:
            tex = this->mNormal;
        case BUTTON_HOVER:
            tex = this->mHover;
        case BUTTON_CLICKED:
            tex = this->mClicked;
    }
    this->gwin->blit(tex, this->mRect);
}

void Button::disable() {
    this->mState = BUTTON_DISABLED;
}
