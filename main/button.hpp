#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "gamewindow.hpp"

#define BUTTON_NORMAL 0
#define BUTTON_HOVER 1
#define BUTTON_CLICKED 2
#define BUTTON_DISABLED 3

class Button {
private:
    SDL_Rect mRect;
    SDL_Rect mClip;
    GameWindow* gwin;
    SDL_Texture* mTex = nullptr;
    int mState;
public:
    Button() {};
    Button (GameWindow* win, SDL_Rect rect);
    void updateTexture (SDL_Texture* tex, int source_button_width, int source_button_height);
    bool isOnHoverByPoint (int x, int y);
    bool isDisabled();
    bool onHover();
    bool onClicked();
    void reset();
    void click();
    void render();
    void disable();
};

#endif // BUTTON_HPP
