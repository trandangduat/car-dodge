#include "hud.hpp"
#include "assets.hpp"
#include "abilities.hpp"

HUD::HUD (GameWindow* gw, GameState* gs) {
    this->gwin = gw;
    this->gstate = gs;
}

void HUD::drawTTFText (TTF_Font* font, std::string text, int fontSize, int x, int y, SDL_Color textColor, int alignX, int wrapLength) {
    TTF_SetFontSize(font, fontSize);
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), textColor, wrapLength);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(this->gwin->gRenderer, textSurface);
    switch (alignX) {
        case HUD_FLOAT_RIGHT:
            x = SCREEN_WIDTH - x - textSurface->w;
            break;
        case HUD_FLOAT_CENTER:
            x = SCREEN_WIDTH / 2 - textSurface->w / 2;
            break;
    }
    SDL_Rect drect = { x, y, textSurface->w, textSurface->h };
    SDL_FreeSurface(textSurface);
    this->gwin->blit(tex, drect);
}

void HUD::drawText (SDL_Texture* tex, std::string text, float x, float y, int letterWidth, int letterHeight, float SCALE, int alignX) {
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int spriteCols = texW / letterWidth;
    int spriteRows = texH / letterHeight;
    int scaledLetterWidth = SCALE * letterWidth * 0.8;
    int scaledLetterHeight = SCALE * letterHeight;

    float letterSpacing = 0;

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
        if (c == ' ') {
            x += scaledLetterWidth / 2 + letterSpacing;
            continue;
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

void HUD::drawParagraph (std::string text, SDL_Rect drect, SDL_Texture* tex, int letterWidth, int letterHeight, float SCALE) {
    int texW, texH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);
    int spriteCols = texW / letterWidth;
    int spriteRows = texH / letterHeight;
    int scaledLetterWidth = SCALE * letterWidth;
    int scaledLetterHeight = SCALE * letterHeight;

    float lineSpacing = 2;
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
            if (c == ' ') {
                x += scaledLetterWidth / 2;
                continue;
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
        srect.x = (i <= gstate->currentLives() ? 0 : 1) * srect.w;
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
    this->drawFadeOverlay(75);
    this->drawTTFText(this->gwin->KarenFat, "game over", 60, 0, 110, {255, 255, 255}, HUD_FLOAT_CENTER);
    this->drawTTFText(this->gwin->KarenFat, "SCORE", 30, 0, 190, {166, 45, 45}, HUD_FLOAT_CENTER);
    this->drawTTFText(this->gwin->KarenFat, std::to_string(gstate->currentScore()), 40, 0, 215, {255, 255, 255}, HUD_FLOAT_CENTER);
    this->drawTTFText(this->gwin->KarenFat, "HIGHSCORE", 30, 0, 280, {166, 45, 45}, HUD_FLOAT_CENTER);
    this->drawTTFText(this->gwin->KarenFat, std::to_string(gstate->currentHighscore()), 40, 0, 305, {255, 255, 255}, HUD_FLOAT_CENTER);
}

void HUD::renderPauseScreen() {
    this->drawFadeOverlay(75);
    this->drawTTFText(this->gwin->KarenFat, "paused", 40, 0, 35, {255, 255, 255}, HUD_FLOAT_CENTER);
}

void HUD::renderStore (int id_tier[], std::vector<Button> &storeOption, Timer* storeTimer) {
    SDL_Rect box, paraRect;

    box = {100, 80, SCREEN_WIDTH - 100 * 2, 440};
    gwin->blit(frameTexture, box);

    int x = 110;
    int y = 100;
    for (int i = 0; i < 3; i++) {
        box = {x, y, SCREEN_WIDTH - x * 2, 120};
        SDL_SetRenderDrawColor(this->gwin->gRenderer, 217, 98, 98, 100);
        SDL_RenderDrawLine(this->gwin->gRenderer, box.x, box.y + box.h, box.x + box.w, box.y + box.h);
        if (storeOption[i].onHover()) {
            SDL_SetRenderDrawColor(this->gwin->gRenderer, 245, 155, 115, 255);
            SDL_RenderFillRect(this->gwin->gRenderer, &box);
        }
        else if (storeOption[i].onClicked()) {
            SDL_SetRenderDrawColor(this->gwin->gRenderer, 217, 98, 98, 255);
            SDL_RenderFillRect(this->gwin->gRenderer, &box);
            storeOption[i].disable();
        }
        else if (storeOption[i].isDisabled()) {
            SDL_SetRenderDrawColor(this->gwin->gRenderer, 217, 98, 98, 255);
            SDL_RenderFillRect(this->gwin->gRenderer, &box);
        }
        // Ability Name
        this->drawTTFText(
            this->gwin->KarenFat,
            abils[i][id_tier[i]].name,
            32,
            x + 25, y + 15,
            {166, 45, 45}
        );

        // Ability desc
        this->drawTTFText(
            this->gwin->AvenuePixel,
            abils[i][id_tier[i]].desc,
            28,
            x + 25, y + 50,
            {0, 0, 0, 255},
            HUD_FLOAT_LEFT,
            SCREEN_WIDTH - 2 * (x + 25)
        );

        // Ability price
        this->drawTTFText(
            this->gwin->AvenuePixel,
            std::to_string(abils[i][id_tier[i]].coins),
            30,
            x + 25, y + 15,
            {81, 18, 9, 255},
            HUD_FLOAT_RIGHT
        );

        y += box.h;
    }

    this->drawTTFText(
        this->gwin->AvenuePixel,
        "RESET STORE AFTER: " + std::to_string(STORE_DURATION - 1 - (int) (storeTimer->elapsedTime() / 1000.f)),
        25, 0, y + 10,
        {0, 0, 0, 255},
        HUD_FLOAT_CENTER
    );
}
