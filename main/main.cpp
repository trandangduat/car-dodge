#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;

class LWindow {
public:
    LWindow();
    bool init();

    SDL_Renderer* createRenderer();
    void free();

    int getWidth();
    int getHeight();

private:
    SDL_Window* mWindow;

    int mWidth;
    int mHeight;
};

class LTexture {
public:
    LTexture();
    ~LTexture();

    bool loadFromFile (std::string path);
    void free();
    void render (int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

class Car {
public:
    static const int CAR_WIDTH = 50;
    static const int CAR_HEIGHT = 100;

    Car();

    void handleEvent (SDL_Event* e);
    void moveTo (int x, int y);
    void render();

private:
    int mPosX, mPosY;
};

class Obstacle {
public:
    static const int OBSTACLE_WIDTH = 50;
    static const int OBSTACLE_HEIGHT = 100;

    Obstacle();

    void render();

private:
    int mPosX;
    int mPosY;

    int mVelY;

};

bool init();
bool loadMedia();
void close();

LWindow gWindow;
SDL_Renderer* gRenderer = NULL;
LTexture yourCarTexture;
Car yourCar;

//--------------------------------------------------------------------//

LWindow::LWindow() {
    mWindow = NULL;
    mWidth = 0;
    mHeight = 0;
}
bool LWindow::init() {
    mWindow = SDL_CreateWindow("Car Dodge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == NULL) {
        return false;
    }
    mWidth = SCREEN_WIDTH;
    mHeight = SCREEN_HEIGHT;
    return true;
}
SDL_Renderer* LWindow::createRenderer() {
    return SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}
void LWindow::free() {
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
    mWidth = 0;
    mHeight = 0;
}
int LWindow::getWidth() {
    return mWidth;
}
int LWindow::getHeight() {
    return mHeight;
}

LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture() {
    free();
}
bool LTexture::loadFromFile (std::string path) {
    return true;
}
void LTexture::free() {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
void LTexture::render (int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(gRenderer, &renderQuad);
//    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}
int LTexture::getWidth() {
	return mWidth;
}
int LTexture::getHeight() {
	return mHeight;
}

Car::Car() {
    mPosX = 0;
    mPosY = 0;
}
void Car::handleEvent (SDL_Event *e) {
    if (e->type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        moveTo(x - CAR_WIDTH / 2, gWindow.getHeight() - 2 * CAR_HEIGHT);
    }
}
void Car::moveTo (int x, int y) {
    if (x < 0) x = 0;
    if (x + CAR_WIDTH >= gWindow.getWidth()) x = gWindow.getWidth() - CAR_WIDTH;
    mPosX = x;
    mPosY = y;
}
void Car::render() {
    SDL_Rect clipQuad;
    clipQuad.w = CAR_WIDTH;
    clipQuad.h = CAR_HEIGHT;
    yourCarTexture.render(mPosX, mPosY, &clipQuad);
}

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << '\n';
        success = false;
    }
    else {
        if (!gWindow.init()) {
            std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
            success = false;
        }
        else {
            gRenderer = gWindow.createRenderer();
            if (gRenderer == NULL) {
                std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << '\n';
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;
    return success;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    gWindow.free();

    SDL_Quit();
}

int main(int agrc, char* argv[]) {
    if (!init()) std::cout << "Failed to init\n";
    else {
        if (!loadMedia()) std::cout << "Failed to load media\n";
        else {
            bool quit = false;
            SDL_Event e;

            // Main game loop
            while (!quit) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    // Handle events here
                    yourCar.handleEvent(&e);
                }
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                yourCar.render();


                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();

    return 0;
}
