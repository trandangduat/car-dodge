#ifndef HUD_HPP
#define HUD_HPP

#include "gamewindow.hpp"
#include "gamestate.hpp"
#include "button.hpp"
#include "timer.hpp"

#define HUD_FLOAT_LEFT 0
#define HUD_FLOAT_RIGHT 1
#define HUD_FLOAT_CENTER 2

class HUD {
private:
    GameWindow* gwin = nullptr;
    GameState* gstate = nullptr;
    Timer* mBlinkTimer = nullptr;
    int blinkSwitch;
    int lastBlinkTime;

public:
    HUD() {};
    HUD (GameWindow* gw, GameState* gs);
    void drawTTFText (TTF_Font* font, std::string text, int fontSize, int x, int y, SDL_Color textColor, int alignX = HUD_FLOAT_LEFT, int wrapLength = SCREEN_WIDTH);
    void drawText (SDL_Texture* tex, std::string text, float x, float y, int letterWidth = 8, int letterHeight = 8, float SCALE = 1, int alignX = HUD_FLOAT_LEFT);
    void drawParagraph (std::string text, SDL_Rect drect, SDL_Texture* tex, int letterWidth = 8, int letterHeight = 8, float SCALE = 1);
    void startBlinkingHearts();
    void drawHearts (SDL_Texture* tex, float x, float y, float scale);
    void renderGameOverScreen();
    void renderPauseScreen();
    void renderTransitionScreen (int countdown);
    void renderStore (int id_tier[], std::vector<Button> &storeOption, Timer* storeTimer);
    void drawFadeOverlay (int fadePercentage);
    void drawFadeRectangle (SDL_Rect rect, int fadePercentage);
};

#endif // HUD_HPP
