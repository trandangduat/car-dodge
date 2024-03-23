#include "gamewindow.hpp"

bool GameWindow::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init failed\n";
        return false;
    }
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & (imgFlags))) {
        std::cout << "IMG_Init failed\n";
        return false;
    }
    if (TTF_Init() == -1) {
        std::cout << "TTF_Init failed\n";
        return false;
    }
    this->gWindow = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (this->gWindow == nullptr) {
        std::cout << "SDL_CreateWindow failed\n";
        return false;
    }
    this->gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->gRenderer == nullptr) {
        std::cout << "SDL_CreateRenderer failed\n";
        return false;
    }
    this->KarenFat = TTF_OpenFont("assets/fonts/KarenFat.ttf", 28);
    this->AvenuePixel = TTF_OpenFont("assets/fonts/AvenuePixel.ttf", 28);
    if (this->KarenFat == nullptr || this->AvenuePixel == nullptr) {
        std::cout << "Failed to load font " << TTF_GetError() << '\n';
        return false;
    }
    return true;
}

SDL_Texture* GameWindow::loadTexture (std::string path) {
    SDL_Texture *texture;
    texture = IMG_LoadTexture(gRenderer, path.c_str());
    if (texture == nullptr) {
        std::cout << "Failed to load texture from " << path << ' ' << IMG_GetError() << '\n';
    }
    return texture;
}

void GameWindow::blit (SDL_Texture* texture, SDL_Rect clip, SDL_Rect rect) {
    SDL_Rect srect = clip;
    SDL_Rect drect = rect;
    SDL_RenderCopyEx(gRenderer, texture, &srect, &drect, 0, nullptr, SDL_FLIP_NONE);
}

void GameWindow::blit (SDL_Texture* texture, SDL_Rect rect, double angle) {
    SDL_Rect drect = rect;
    SDL_RenderCopyEx(gRenderer, texture, nullptr, &drect, angle, nullptr, SDL_FLIP_NONE);
}

void GameWindow::blit (SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect drect = {x, y, w, h};
    if (drect.w == -1 && drect.h == -1) {
        SDL_QueryTexture(texture, nullptr, nullptr, &drect.w, &drect.h);
    }
    SDL_RenderCopyEx(gRenderer, texture, nullptr, &drect, 0, nullptr, SDL_FLIP_NONE);
}

void GameWindow::clearRender() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
}

void GameWindow::presentRender() {
    SDL_RenderPresent(gRenderer);
}
