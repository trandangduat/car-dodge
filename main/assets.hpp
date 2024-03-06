#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "gamewindow.hpp"

extern std::vector<SDL_Texture*> backgroundTextures;
extern SDL_Texture* carTexture;
extern SDL_Texture* carInvisibleTexture;
extern SDL_Texture* obstacleSpriteTexture;
extern SDL_Texture* obstacleCrashedSpriteTexture;
extern SDL_Texture* coinSprite;
extern SDL_Texture* goldenFontTexture;
extern SDL_Texture* whiteFontTexture;
extern SDL_Texture* metalFontTexture;
extern SDL_Texture* heartSymbolTexture;
extern std::vector<SDL_Rect> obstaclesClipRect;

void loadMedia (GameWindow* win);

#endif // ASSETS_HPP
