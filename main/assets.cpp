#include "assets.hpp"

void clipObstacles();

void loadMedia (GameWindow* win) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    backgroundTextures[0]           = win->loadTexture("assets/images/road_5.png");
    backgroundTextures[1]           = win->loadTexture("assets/images/road_6.png");
    carTexture                      = win->loadTexture("assets/images/car.png");
    carInvisibleTexture             = win->loadTexture("assets/images/car_invisible.png");
    obstacleSpriteTexture           = win->loadTexture("assets/images/cars.png");
    obstacleCrashedSpriteTexture    = win->loadTexture("assets/images/cars_crashed.png");
    coinSprite                      = win->loadTexture("assets/images/items/coin.png");
    goldenFontTexture               = win->loadTexture("assets/fonts/golden.png");
    whiteFontTexture                = win->loadTexture("assets/fonts/white.png");
    metalFontTexture                = win->loadTexture("assets/fonts/metal.png");
    heartSymbolTexture              = win->loadTexture("assets/images/HUD/heart.png");
    clipObstacles();
}

void clipObstacles() {
    obstaclesClipRect.push_back({253, 9, 49, 92});
    obstaclesClipRect.push_back({314, 9, 49, 92});
    obstaclesClipRect.push_back({373, 9, 49, 92});
    obstaclesClipRect.push_back({69, 335, 46, 87});
    obstaclesClipRect.push_back({127, 335, 46, 87});
}
