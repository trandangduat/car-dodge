#include "assets.hpp"

std::vector<SDL_Texture*> backgroundTextures(2, nullptr);
SDL_Texture* carTexture = nullptr;
SDL_Texture* carInvisibleTexture = nullptr;
SDL_Texture* obstaclesTexture = nullptr;
SDL_Texture* obstaclesCrashedTexture = nullptr;
SDL_Texture* obstaclesCrashedWhiteTexture = nullptr;
SDL_Texture* coinSprite = nullptr;
SDL_Texture* goldenFontTexture = nullptr;
SDL_Texture* blueFontTexture = nullptr;
SDL_Texture* whiteFontTexture = nullptr;
SDL_Texture* metalFontTexture = nullptr;
SDL_Texture* heartSymbolTexture = nullptr;
SDL_Texture* frameTexture = nullptr;
SDL_Texture* bulletTexture = nullptr;
SDL_Texture* bulletIcon = nullptr;
SDL_Texture* gasSmoke = nullptr;
std::vector<SDL_Rect> obstaclesClipRect;

void clipObstacles();

void loadMedia (GameWindow* win) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    backgroundTextures[0]           = win->loadTexture("assets/images/road_7.png");
    backgroundTextures[1]           = win->loadTexture("assets/images/road_8.png");
    carTexture                      = win->loadTexture("assets/images/car.png");
    carInvisibleTexture             = win->loadTexture("assets/images/car_invisible.png");
    obstaclesTexture                = win->loadTexture("assets/images/cars.png");
    obstaclesCrashedTexture         = win->loadTexture("assets/images/cars_crashed.png");
    obstaclesCrashedWhiteTexture    = win->loadTexture("assets/images/cars_crashed_white.png");
    coinSprite                      = win->loadTexture("assets/images/items/coin.png");
    goldenFontTexture               = win->loadTexture("assets/fonts/golden.png");
    blueFontTexture                 = win->loadTexture("assets/fonts/blue.png");
    whiteFontTexture                = win->loadTexture("assets/fonts/white.png");
    metalFontTexture                = win->loadTexture("assets/fonts/metal.png");
    heartSymbolTexture              = win->loadTexture("assets/images/HUD/heart.png");
    frameTexture                    = win->loadTexture("assets/images/HUD/frame2.png");
    bulletTexture                   = win->loadTexture("assets/images/items/bullet.png");
    bulletIcon                      = win->loadTexture("assets/images/HUD/bullet_hud.png");
    gasSmoke                        = win->loadTexture("assets/images/effects/speed_boost.png");
    clipObstacles();
}

void clipObstacles() {
    obstaclesClipRect.push_back({253, 9, 49, 92});
    obstaclesClipRect.push_back({314, 9, 49, 92});
    obstaclesClipRect.push_back({373, 9, 49, 92});
    obstaclesClipRect.push_back({69, 335, 46, 87});
    obstaclesClipRect.push_back({127, 335, 46, 87});
}
