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
    void drawText (SDL_Texture* tex, std::string text, float x, float y, int letterWidth = 8, int letterHeight = 8, float SCALE = 1, int alignX = HUD_FLOAT_LEFT);
    void drawParagraph (std::string text, SDL_Rect drect, SDL_Texture* tex, int letterWidth = 8, int letterHeight = 8, float SCALE = 1);
    void drawHearts (SDL_Texture* tex, float x, float y, int remainHearts, float scale, int alignX);
    void renderGameOverScreen();
    void renderPauseScreen();
    void renderStore (int id_tier[]);
    void drawFadeOverlay (int fadePercentage);
};

#endif // HUD_HPP
