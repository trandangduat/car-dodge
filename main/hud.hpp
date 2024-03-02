#ifndef HUD_HPP
#define HUD_HPP

#include "gamewindow.hpp"
#include "gamestate.hpp"

class HUD {
private:
    GameWindow* gwin = nullptr;
    GameState* gstate = nullptr;

public:
    HUD() {};
    HUD (GameWindow* gw, GameState* gs);
    void drawText (SDL_Texture* tex, float x, float y, std::string text, float scale);
    void drawHearts (SDL_Texture* tex, float x, float y, int remainHearts, float scale);
};

#endif // HUD_HPP
