#include "hud.hpp"

HUD::HUD (GameWindow* gw, GameState* gs) {
    this->gwin = gw;
    this->gstate = gs;
}

void HUD::drawText (SDL_Texture* tex, float x, float y, std::string text, float scale, bool align) {
    SDL_Rect srect = {0, 0, 8, 8};
    SDL_Rect drect = {x, y, (int) (scale * srect.w), (int) (scale * srect.h)};
    if (align == HUD_FLOAT_RIGHT) {
        drect.x = SCREEN_WIDTH - x - int(text.size()) * drect.w;
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

void HUD::drawHearts (SDL_Texture* tex, float x, float y, int remainHearts, float scale, bool align) {
    SDL_Rect srect = {0, 0, 16, 16};
    SDL_Rect drect = {x, y, (int) (scale * srect.w), (int) (scale * srect.h)};
    if (align == HUD_FLOAT_RIGHT) {
        drect.x = SCREEN_WIDTH - x - NUMBER_OF_LIVES * drect.w;
    }
    for (int i = 1; i <= NUMBER_OF_LIVES; i++) {
        srect.x = (i <= gstate->remainLives() ? 0 : srect.w);
        this->gwin->blit(tex, srect, drect);
        drect.x += drect.w + 5;
    }
}
