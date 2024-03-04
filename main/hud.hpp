#ifndef HUD_HPP
#define HUD_HPP

#include "gamewindow.hpp"
#include "gamestate.hpp"

#define HUD_FLOAT_RIGHT 0
#define HUD_FLOAT_LEFT 1

class HUD {
private:
    GameWindow* gwin = nullptr;
    GameState* gstate = nullptr;

public:
    HUD() {};
    HUD (GameWindow* gw, GameState* gs);
    void drawText (SDL_Texture* tex, float x, float y, std::string text, float scale, bool align);
    void drawHearts (SDL_Texture* tex, float x, float y, int remainHearts, float scale, bool align);
};

#endif // HUD_HPP
