#include "assets.hpp"

std::vector<SDL_Texture*> backgroundTextures(2, nullptr);
SDL_Texture* carTexture = nullptr;
SDL_Texture* carInvisibleTexture = nullptr;
SDL_Texture* obstacleSpriteTexture = nullptr;
SDL_Texture* obstacleCrashedSpriteTexture = nullptr;
SDL_Texture* coinSprite = nullptr;
SDL_Texture* goldenFontTexture = nullptr;
SDL_Texture* whiteFontTexture = nullptr;
SDL_Texture* metalFontTexture = nullptr;
SDL_Texture* plainWhiteFontTexture = nullptr;
SDL_Texture* heartSymbolTexture = nullptr;
SDL_Texture* GUISprite = nullptr;
std::vector<SDL_Rect> obstaclesClipRect;
std::vector<SDL_Rect> GUIRect;

void clipObstacles();
void clipGUI();

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
    plainWhiteFontTexture           = win->loadTexture("assets/fonts/plain_white.png");
    heartSymbolTexture              = win->loadTexture("assets/images/HUD/heart.png");
    GUISprite                       = win->loadTexture("assets/images/HUD/GUIsprite.png");
    clipObstacles();
    clipGUI();
}

void clipObstacles() {
    obstaclesClipRect.push_back({253, 9, 49, 92});
    obstaclesClipRect.push_back({314, 9, 49, 92});
    obstaclesClipRect.push_back({373, 9, 49, 92});
    obstaclesClipRect.push_back({69, 335, 46, 87});
    obstaclesClipRect.push_back({127, 335, 46, 87});
}

void clipGUI() {
    GUIRect.push_back({10, 28, 48, 48});
    GUIRect.push_back({78, 87, 64, 64});
}
