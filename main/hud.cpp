#include "hud.hpp"
#include "assets.hpp"

HUD::HUD (GameWindow* gw, GameState* gs) {
    this->gwin = gw;
    this->gstate = gs;
}

void HUD::drawText (SDL_Texture* tex, float x, float y, std::string text, float scale, int alignX) {
    /*
        Get the width, height of the font sprite,
        then the letter size is the minimum of the two
    */
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int letterSize = std::min(texW, texH);
    SDL_Rect srect = {0, 0, letterSize, letterSize};
    SDL_Rect drect = {x, y, (int) (scale * srect.w), (int) (scale * srect.h)};
    int totalLength = drect.w * int(text.size());
    switch (alignX) {
        case HUD_FLOAT_RIGHT:
            drect.x = SCREEN_WIDTH - x - totalLength;
            break;
        case HUD_FLOAT_CENTER:
            drect.x = SCREEN_WIDTH / 2 - totalLength / 2;
            break;
    }
    for (char &c : text) {
        if ('a' <= c && c <= 'z') {
            c += ('A' - 'a');
        }
        srect.x = int(c - ' ') * srect.w;
        this->gwin->blit(tex, srect, drect);
        drect.x += drect.w;
    }
}

void HUD::drawHearts (SDL_Texture* tex, float x, float y, int remainHearts, float scale, int alignX) {
    SDL_Rect srect = {0, 0, 16, 16};
    SDL_Rect drect = {x, y, (int) (scale * srect.w), (int) (scale * srect.h)};
    int totalLength = drect.w * NUMBER_OF_LIVES;
    switch (alignX) {
        case HUD_FLOAT_RIGHT:
            drect.x = SCREEN_WIDTH - x - totalLength;
            break;
        case HUD_FLOAT_CENTER:
            drect.x = SCREEN_WIDTH / 2 - totalLength / 2;
            break;
    }
    for (int i = 1; i <= NUMBER_OF_LIVES; i++) {
        srect.x = (i <= gstate->remainLives() ? 0 : srect.w);
        this->gwin->blit(tex, srect, drect);
        drect.x += drect.w + 5;
    }
}

void HUD::drawFadeOverlay (int fadePercentage) {
    /* Draw a 'fadePercentage' opacity black rectangle covering the screen */
    SDL_SetRenderDrawColor(gwin->gRenderer, 0, 0, 0, fadePercentage * 255 / 100);
    SDL_SetRenderDrawBlendMode(gwin->gRenderer, SDL_BLENDMODE_BLEND);
    SDL_Rect fullScreenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(gwin->gRenderer, &fullScreenRect);
}

void HUD::renderGameOverScreen() {
    this->drawFadeOverlay(70);
    this->drawText(metalFontTexture, 0, 80, "GAME OVER", 0.6f, HUD_FLOAT_CENTER);
    this->drawText(whiteFontTexture, 0, 160, "SCORE", 2, HUD_FLOAT_CENTER);
    this->drawText(whiteFontTexture, 0, 185, std::to_string(gstate->currentScore()), 4, HUD_FLOAT_CENTER);
}

void HUD::renderPauseScreen() {
    this->drawFadeOverlay(70);
    this->drawText(whiteFontTexture, 0, 100, "PAUSED", 3, HUD_FLOAT_CENTER);
}
