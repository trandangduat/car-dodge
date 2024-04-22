#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include "params.hpp"

#include <iostream>
#include <deque>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class GameWindow {
private:
    void free();

public:
    GameWindow() {};
    ~GameWindow();
    SDL_Window* gWindow = nullptr;
    SDL_Renderer* gRenderer = nullptr;
    TTF_Font* KarenFat = nullptr;
    TTF_Font* AvenuePixel = nullptr;
    bool init();
    SDL_Texture* loadTexture (std::string path);
    void blit (SDL_Texture* tex, SDL_Rect clip, SDL_Rect rect);
    void blit (SDL_Texture* tex, SDL_Rect rect, double angle = 0);
    void blit (SDL_Texture* tex, int x, int y, int w = -1, int h = -1);
    void clearRender();
    void presentRender();
};

#endif // GAMEWINDOW_HPP
