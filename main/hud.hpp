#ifndef HUD_HPP
#define HUD_HPP

#include "gamewindow.hpp"
#include "gamestate.hpp"

#define HUD_FLOAT_LEFT 0
#define HUD_FLOAT_RIGHT 1
#define HUD_FLOAT_CENTER 2

class HUD {
private:
    GameWindow* gwin = nullptr;
    GameState* gstate = nullptr;

public:
    HUD() {};
    HUD (GameWindow* gw, GameState* gs);
    void drawText (SDL_Texture* tex, float x, float y, std::string text, float scale, int alignX);
    void drawHearts (SDL_Texture* tex, float x, float y, int remainHearts, float scale, int alignX);
};

#endif // HUD_HPP
