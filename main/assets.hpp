#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "gamewindow.hpp"

extern std::vector<SDL_Texture*> backgroundTextures;
extern SDL_Texture* carTexture;
extern SDL_Texture* obstaclesTexture;
extern SDL_Texture* obstaclesCrashedTexture;
extern SDL_Texture* obstaclesCrashedWhiteTexture;
extern SDL_Texture* coinSprite;
extern SDL_Texture* goldenFontTexture;
extern SDL_Texture* blueFontTexture;
extern SDL_Texture* whiteFontTexture;
extern SDL_Texture* metalFontTexture;
extern SDL_Texture* heartSymbolTexture;
extern SDL_Texture* frameTexture;
extern SDL_Texture* bulletTexture;
extern SDL_Texture* bulletIcon;
extern SDL_Texture* gasSmoke;
extern SDL_Texture* nitroHUD;
extern std::vector<SDL_Rect> obstaclesClipRect;

void loadMedia (GameWindow* win);

#endif // ASSETS_HPP
