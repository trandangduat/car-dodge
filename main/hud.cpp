#include "hud.hpp"
#include "assets.hpp"
#include "abilities.hpp"

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

int cnt = 20;

void HUD::drawParagraph (std::string text, SDL_Rect drect, SDL_Texture* tex, int letterWidth, int letterHeight, float SCALE) {
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int spriteCols = texW / letterWidth;
    int spriteRows = texH / letterHeight;
    int lineSpacing = 3;

    int x = drect.x;
    int y = drect.y;
    for (char& c : text) {
        int asciiOrder = int(c - ' ');
        SDL_Rect srect = {(asciiOrder % spriteCols) * letterWidth, (asciiOrder / spriteCols) * letterHeight, letterWidth, letterHeight};
        if (x + SCALE * letterWidth > drect.x + drect.w) {
            x = drect.x;
            y += SCALE * letterHeight + lineSpacing;
        }
        this->gwin->blit(tex, srect, {x, y, SCALE * letterWidth, SCALE * letterHeight});
        x += SCALE * letterWidth;
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

void HUD::drawBox (SDL_Texture* tex, SDL_Rect srect, SDL_Rect drect) {
    this->gwin->blit(tex, srect, drect);
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

void HUD::renderPauseScreen (int t1id, int t2id, int t3id) {
    this->drawFadeOverlay(70);
    this->drawText(whiteFontTexture, 0, 50, "PAUSED", 3, HUD_FLOAT_CENTER);
    SDL_SetRenderDrawColor(this->gwin->gRenderer, 255, 255, 255, 255);
    SDL_Rect box, paraRect;

    box = {50, 100, 400, 150};
    SDL_RenderDrawRect(this->gwin->gRenderer, &box);
    this->drawText(whiteFontTexture, 60, 110, abils[0][t1id - 1].name, 2, HUD_FLOAT_LEFT);
    this->drawText(goldenFontTexture, 60, 110, "#" + std::to_string(abils[0][t1id - 1].coins), 2, HUD_FLOAT_RIGHT);
    paraRect = {60, 140, 380, 140};
    this->drawParagraph(abils[0][t1id - 1].desc, paraRect, plainWhiteFontTexture, 16, 16, 0.8f);

    box = {50, 250, 400, 150};
    SDL_RenderDrawRect(this->gwin->gRenderer, &box);

    box = {50, 400, 400, 150};
    SDL_RenderDrawRect(this->gwin->gRenderer, &box);
}
