#include "gamewindow.hpp"

bool GameWindow::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init failed\n";
        return false;
    }
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (IMG_Init(imgFlags) & (imgFlags) == 0) {
        std::cout << "IMG_Init failed\n";
        return false;
    }
    gWindow = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (gWindow == nullptr) {
        std::cout << "SDL_CreateWindow failed\n";
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == nullptr) {
        std::cout << "SDL_CreateRenderer failed\n";
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
    SDL_RenderCopy(gRenderer, texture, &srect, &drect);
}

void GameWindow::blit (SDL_Texture* texture, SDL_Rect rect, double angle) {
    SDL_Rect drect = rect;
    SDL_RenderCopyEx(gRenderer, texture, nullptr, &drect, angle, nullptr, SDL_FLIP_NONE);
}

void GameWindow::blit (SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect dest = {x, y, w, h};
    if (dest.w == -1 && dest.h == -1) {
        SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
    }
    SDL_RenderCopy(gRenderer, texture, nullptr, &dest);
}

void GameWindow::clearRender() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
}

void GameWindow::presentRender() {
    SDL_RenderPresent(gRenderer);
}
