#include "hud.hpp"
#include "assets.hpp"
#include "abilities.hpp"

HUD::HUD (GameWindow* gw, GameState* gs) {
    this->gwin = gw;
    this->gstate = gs;
}

void HUD::drawText (SDL_Texture* tex, std::string text, float x, float y, int letterWidth, int letterHeight, float SCALE, int alignX) {
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int spriteCols = texW / letterWidth;
    int spriteRows = texH / letterHeight;
    int scaledLetterWidth = SCALE * letterWidth;
    int scaledLetterHeight = SCALE * letterHeight;

    float letterSpacing = -3;

    int totalLength = scaledLetterWidth * int(text.size());
    switch (alignX) {
        case HUD_FLOAT_RIGHT:
            x = SCREEN_WIDTH - x - totalLength;
            break;
        case HUD_FLOAT_CENTER:
            x = SCREEN_WIDTH / 2 - totalLength / 2;
            break;
    }

    for (char &c : text) {
        if ('a' <= c && c <= 'z') {
            c += ('A' - 'a');
        }
        int asciiOrder = int(c - ' ');
        SDL_Rect srect = {
            (asciiOrder % spriteCols) * letterWidth,
            (asciiOrder / spriteCols) * letterHeight,
            letterWidth,
            letterHeight
        };
        this->gwin->blit(tex, srect, {x, y, scaledLetterWidth, scaledLetterHeight});
        x += scaledLetterWidth + letterSpacing;
    }
}

int cnt = 20;

void HUD::drawParagraph (std::string text, SDL_Rect drect, SDL_Texture* tex, int letterWidth, int letterHeight, float SCALE) {
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int spriteCols = texW / letterWidth;
    int spriteRows = texH / letterHeight;
    int scaledLetterWidth = SCALE * letterWidth;
    int scaledLetterHeight = SCALE * letterHeight;

    float lineSpacing = -2;
    float letterSpacing = 0;

    float x = drect.x;
    float y = drect.y;
    for (int i = 0; i < (int) text.size(); i++) {
        std::string word = "";
        int j;
        for (j = i; j < (int) text.size(); j++) {
            word += text[j];
            if (text[j] == ' ') break;
        }
        if (x + scaledLetterWidth * ((int) word.size() - 1) > drect.x + drect.w) {
            x = drect.x;
            y += scaledLetterHeight + lineSpacing;
        }
        for (char &c : word) {
            int asciiOrder = int(c - ' ');
            SDL_Rect srect = {
                (asciiOrder % spriteCols) * letterWidth,
                (asciiOrder / spriteCols) * letterHeight,
                letterWidth,
                letterHeight
            };
            this->gwin->blit(tex, srect, {x, y, scaledLetterWidth, scaledLetterHeight});
            x += scaledLetterWidth + letterSpacing;
        }
        i = j;
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
    this->drawText(metalFontTexture, "GAME OVER", 0, 80, 64, 64, 0.6f, HUD_FLOAT_CENTER);
    this->drawText(whiteFontTexture, "SCORE", 0, 160, 8, 8, 2.0f, HUD_FLOAT_CENTER);
    this->drawText(whiteFontTexture, std::to_string(gstate->currentScore()), 0, 185, 8, 8, 4.0f, HUD_FLOAT_CENTER);
}

void HUD::renderPauseScreen() {
    this->drawFadeOverlay(75);
    this->drawText(whiteFontTexture, "PAUSED", 0, 35, 8, 8, 3.0f, HUD_FLOAT_CENTER);
}

void HUD::renderStore (int id_tier[]) {
    SDL_Rect box, paraRect;

    box = {20, 70, 460, 500};
    gwin->blit(frameTexture, box);

    int x = 40;
    int y = 100;
    for (int i = 0; i < 3; i++) {
        box = {x, y, 420, 150};
        SDL_SetRenderDrawColor(this->gwin->gRenderer, 222, 159, 71, 255);
        if (i < 2) {
            SDL_RenderDrawLine(this->gwin->gRenderer, box.x, box.y + box.h, box.x + box.w, box.y + box.h);
        }
//        SDL_RenderDrawRect(this->gwin->gRenderer, &box);

        this->drawText(
            plainWhiteFontTexture,
            abils[i][id_tier[i] - 1].name,
            60, y + 15, 16, 16, 1.2f
        );

        this->drawText(
            goldenFontTexture,
            std::to_string(abils[i][id_tier[i] - 1].coins),
            60, y + 15, 8, 8, 2.0f,
            HUD_FLOAT_RIGHT
        );

        paraRect = {60, y + 40, 380, 90};
        this->drawParagraph(abils[i][id_tier[i] - 1].desc, paraRect, plainBlackFontTexture, 6, 12, 2);

        y += box.h;
    }
}
