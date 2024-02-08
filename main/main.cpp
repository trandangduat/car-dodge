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

bool init();
bool loadMedia();
void close();

LWindow gWindow;
SDL_Renderer* gRenderer = NULL;

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

            SDL_Rect yourCar = {5, 5, 50, 100};

            // Main game loop
            while (!quit) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    // Handle events here

                }
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(gRenderer, &yourCar);


                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();

    return 0;
}
