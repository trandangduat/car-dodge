#include "assets.hpp"

std::vector<SDL_Texture*> backgroundTextures(2, nullptr);
SDL_Texture* carTexture = nullptr;
SDL_Texture* obstaclesTexture = nullptr;
SDL_Texture* obstaclesCrashedTexture = nullptr;
SDL_Texture* obstaclesCrashedWhiteTexture = nullptr;
SDL_Texture* coinSprite = nullptr;
SDL_Texture* goldenFontTexture = nullptr;
SDL_Texture* blueFontTexture = nullptr;
SDL_Texture* whiteFontTexture = nullptr;
SDL_Texture* heartSymbolTexture = nullptr;
SDL_Texture* frameTexture = nullptr;
SDL_Texture* bulletTexture = nullptr;
SDL_Texture* bulletIcon = nullptr;
SDL_Texture* gasSmoke = nullptr;
SDL_Texture* bossLaser = nullptr;
SDL_Texture* bossSprite = nullptr;
SDL_Texture* bossNearUltingSprite = nullptr;
SDL_Texture* gameTitleSprite = nullptr;
SDL_Texture* playButtonSprite = nullptr;
SDL_Texture* homeButtonSprite = nullptr;
std::vector<SDL_Rect> obstaclesClipRect;

Mix_Music* bgMusic = nullptr;
Mix_Chunk* hoverSfx = nullptr;
Mix_Chunk* clickSfx = nullptr;
Mix_Chunk* cashoutSfx = nullptr;
Mix_Chunk* coinCollectSfx = nullptr;
Mix_Chunk* shootSfx = nullptr;
Mix_Chunk* explodeSfx = nullptr;
Mix_Chunk* bossRoarSfx = nullptr;
Mix_Chunk* bossWarningSfx = nullptr;
Mix_Chunk* gruntSfx = nullptr;
Mix_Chunk* honkSfx = nullptr;

#define BG_PATH "assets/images/road_"

void clipObstacles();

void loadMedia (GameWindow* win) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    std::string bgPath1, bgPath2;
    int type = rand() % 3;
    bgPath1 = BG_PATH;
    bgPath1 += char(type * 2 + 1 + '0');
    bgPath1 += ".png";
    bgPath2 = BG_PATH;
    bgPath2 += char(type * 2 + 2 + '0');
    bgPath2 += ".png";

    backgroundTextures[0]           = win->loadTexture(bgPath1);
    backgroundTextures[1]           = win->loadTexture(bgPath2);
    carTexture                      = win->loadTexture("assets/images/car.png");
    obstaclesTexture                = win->loadTexture("assets/images/cars.png");
    obstaclesCrashedTexture         = win->loadTexture("assets/images/cars_crashed.png");
    obstaclesCrashedWhiteTexture    = win->loadTexture("assets/images/cars_crashed_white.png");
    coinSprite                      = win->loadTexture("assets/images/items/coin.png");
    goldenFontTexture               = win->loadTexture("assets/fonts/golden.png");
    blueFontTexture                 = win->loadTexture("assets/fonts/blue.png");
    whiteFontTexture                = win->loadTexture("assets/fonts/white.png");
    heartSymbolTexture              = win->loadTexture("assets/images/HUD/heart.png");
    frameTexture                    = win->loadTexture("assets/images/HUD/frame.png");
    bulletTexture                   = win->loadTexture("assets/images/items/bullet.png");
    bulletIcon                      = win->loadTexture("assets/images/HUD/bullet_hud.png");
    gasSmoke                        = win->loadTexture("assets/images/effects/speed_boost.png");
    bossSprite                      = win->loadTexture("assets/images/boss_sprite.png");
    bossNearUltingSprite            = win->loadTexture("assets/images/boss_near_ulting_sprite.png");
    bossLaser                       = win->loadTexture("assets/images/effects/laser_beam_sprite.png");
    gameTitleSprite                 = win->loadTexture("assets/images/start_screen/logo.png");
    playButtonSprite                = win->loadTexture("assets/images/start_screen/play_button.png");
    homeButtonSprite                = win->loadTexture("assets/images/game_over/home_button.png");
    clipObstacles();

    bgMusic                         = Mix_LoadMUS("assets/sfx/bgmusic.mp3");
    hoverSfx                        = Mix_LoadWAV("assets/sfx/button_hover.wav");
    clickSfx                        = Mix_LoadWAV("assets/sfx/button_click.wav");
    cashoutSfx                      = Mix_LoadWAV("assets/sfx/cashout.wav");
    coinCollectSfx                  = Mix_LoadWAV("assets/sfx/coin_collect.wav");
    shootSfx                        = Mix_LoadWAV("assets/sfx/shoot.wav");
    explodeSfx                      = Mix_LoadWAV("assets/sfx/explode.wav");
    bossRoarSfx                     = Mix_LoadWAV("assets/sfx/boss_roar.wav");
    bossWarningSfx                  = Mix_LoadWAV("assets/sfx/boss_warning.wav");
    gruntSfx                        = Mix_LoadWAV("assets/sfx/grunt.wav");
    honkSfx                         = Mix_LoadWAV("assets/sfx/honk.wav");
}

void clipObstacles() {
    obstaclesClipRect.push_back({253, 9, 49, 92});
    obstaclesClipRect.push_back({314, 9, 49, 92});
    obstaclesClipRect.push_back({373, 9, 49, 92});
    obstaclesClipRect.push_back({69, 335, 46, 87});
    obstaclesClipRect.push_back({127, 335, 46, 87});
}

void freeMedia() {
    SDL_DestroyTexture(carTexture);
    SDL_DestroyTexture(obstaclesTexture);
    SDL_DestroyTexture(obstaclesCrashedTexture);
    SDL_DestroyTexture(obstaclesCrashedWhiteTexture);
    SDL_DestroyTexture(coinSprite);
    SDL_DestroyTexture(goldenFontTexture);
    SDL_DestroyTexture(blueFontTexture);
    SDL_DestroyTexture(whiteFontTexture);
    SDL_DestroyTexture(heartSymbolTexture);
    SDL_DestroyTexture(frameTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(bulletIcon);
    SDL_DestroyTexture(gasSmoke);
    SDL_DestroyTexture(bossLaser);
    SDL_DestroyTexture(bossSprite);
    SDL_DestroyTexture(bossNearUltingSprite);
    SDL_DestroyTexture(gameTitleSprite);
    SDL_DestroyTexture(playButtonSprite);
    SDL_DestroyTexture(homeButtonSprite);

    for (SDL_Texture* texture : backgroundTextures) {
        SDL_DestroyTexture(texture);
    }
    backgroundTextures.clear();

    Mix_FreeMusic(bgMusic);
    Mix_FreeChunk(hoverSfx);
    Mix_FreeChunk(clickSfx);
    Mix_FreeChunk(cashoutSfx);
    Mix_FreeChunk(coinCollectSfx);
    Mix_FreeChunk(shootSfx);
    Mix_FreeChunk(explodeSfx);
    Mix_FreeChunk(bossRoarSfx);
    Mix_FreeChunk(bossWarningSfx);
    Mix_FreeChunk(gruntSfx);
    Mix_FreeChunk(honkSfx);
}
