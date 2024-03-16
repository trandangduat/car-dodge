#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "gamewindow.hpp"

enum {
    BUTTON_NORMAL,
    BUTTON_HOVER,
    BUTTON_CLICKED,
    BUTTON_DISABLED,
};

class Button {
private:
    SDL_Rect mRect;
    GameWindow* gwin;
    SDL_Texture* mNormal = nullptr;
    SDL_Texture* mHover = nullptr;
    SDL_Texture* mClicked = nullptr;
    int mState;
public:
    Button() {};
    Button (GameWindow* win, SDL_Rect rect, SDL_Texture* normalTex, SDL_Texture* hoverTex, SDL_Texture* clickedTex);
    bool isPointInsideButton (int x, int y);
    bool isDisabled();
    bool onHover();
    bool onClicked();
    void reset();
    void click();
    void render();
    void disable();
};

#endif // BUTTON_HPP
