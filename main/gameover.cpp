#include "gameover.hpp"
#include "assets.hpp"

void renderGameOverScreen (GameWindow* gWin, GameState* gState, HUD* gHud) {
    /* Draw a 70% opacity black rectangle covering the screen */
    SDL_SetRenderDrawColor(gWin->gRenderer, 0, 0, 0, 180);
    SDL_SetRenderDrawBlendMode(gWin->gRenderer, SDL_BLENDMODE_BLEND);
    SDL_Rect fullScreenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(gWin->gRenderer, &fullScreenRect);

    /* Draw game over content */
    gHud->drawText(metalFontTexture, 0, 80, "GAME OVER", 0.6f, HUD_FLOAT_CENTER);
    gHud->drawText(whiteFontTexture, 0, 160, "SCORE", 2, HUD_FLOAT_CENTER);
    gHud->drawText(whiteFontTexture, 0, 185, std::to_string(gState->currentScore()), 4, HUD_FLOAT_CENTER);
}
