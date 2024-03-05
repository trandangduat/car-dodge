#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "gamewindow.hpp"

std::vector<SDL_Texture*> backgroundTextures(2, nullptr);
SDL_Texture* carTexture = nullptr;
SDL_Texture* carInvisibleTexture = nullptr;
SDL_Texture* obstacleSpriteTexture = nullptr;
SDL_Texture* obstacleCrashedSpriteTexture = nullptr;
SDL_Texture* coinSprite = nullptr;
SDL_Texture* goldenFontTexture = nullptr;
SDL_Texture* whiteFontTexture = nullptr;
SDL_Texture* metalFontTexture = nullptr;
SDL_Texture* heartSymbolTexture = nullptr;
std::vector<SDL_Rect> obstaclesClipRect;

void loadMedia (GameWindow* win);

#endif // ASSETS_HPP
