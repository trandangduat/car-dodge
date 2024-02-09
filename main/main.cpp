#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<deque>
#include<cstdlib> // for rand() and srand()
#include<ctime> // for time()

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int NUMBER_OF_ROWS = 4;

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

    bool loadFromFile (std::string path, int w = -1, int h = -1);
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
    Obstacle (int posX, int posY, int velY);

    void loadTextureImage (std::string path);
    void render();
    void setPos (int x, int y);
    void setVelY (int velY);

    int getVelY();
    int getPosX();
    int getPosY();

private:
    LTexture mTexture;
    int mPosX;
    int mPosY;
    int mVelY;
};

struct columnRange {
    int startX;
    int width;
};

bool init();
bool loadMedia();
void close();
void generateColumnRanges();

LWindow gWindow;
SDL_Renderer* gRenderer = NULL;
LTexture yourCarTexture;
LTexture bgTexture;
Car yourCar;
columnRange colRanges[4];
std::deque<Obstacle> obstacles[4];

//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
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
bool LTexture::loadFromFile (std::string path, int w, int h) {
    free();

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL) {
        std::cout << "Unable to load surface: " << IMG_GetError() << '\n';
        return false;
    }
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) {
        std::cout << "Unable to create texture from " << path << ": " << SDL_GetError() << '\n';
        return false;
    }
    mWidth = (w == -1 ? loadedSurface->w : w);
    mHeight = (h == -1 ? loadedSurface->h : h);

    SDL_FreeSurface(loadedSurface);

    mTexture = newTexture;
    return mTexture != NULL;
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
    SDL_RenderCopyEx(gRenderer, mTexture, NULL, &renderQuad, angle, center, flip);
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
    yourCarTexture.render(mPosX, mPosY);
}

Obstacle::Obstacle() {
    mPosX = 0;
    mPosY = 0;
    mVelY = 0;
}
Obstacle::Obstacle (int posX, int posY, int velY) {
    mPosX = posX;
    mPosY = posY;
    mVelY = velY;
}
void Obstacle::loadTextureImage (std::string path) {
    mTexture.loadFromFile(path, Obstacle::OBSTACLE_WIDTH, Obstacle::OBSTACLE_HEIGHT);
}
void Obstacle::render() {
    mTexture.render(mPosX, mPosY);
}
void Obstacle::setPos (int x, int y) {
    mPosX = x;
    mPosY = y;
}
void Obstacle::setVelY (int velY) {
    mVelY = velY;
}
int Obstacle::getPosX() {
    return mPosX;
}
int Obstacle::getPosY() {
    return mPosY;
}
int Obstacle::getVelY() {
    return mVelY;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << '\n';
        return false;
    }
    if (!gWindow.init()) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        return false;
    }
    gRenderer = gWindow.createRenderer();
    if (gRenderer == NULL) {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << '\n';
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!( IMG_Init(imgFlags) & imgFlags )) {
        std::cout << "IMG_Init failed: " << IMG_GetError() << '\n';
        return false;
    }
    return true;
}

bool loadMedia() {
    bool success = true;
    if (!bgTexture.loadFromFile("assets/images/road.png")) {
        std::cout << "Failed to load bg texture\n";
        success = false;
    }
    if (!yourCarTexture.loadFromFile("assets/images/car.png", yourCar.CAR_WIDTH, yourCar.CAR_HEIGHT)) {
        std::cout << "Failed to load car texture\n";
        success = false;
    }
    return success;
}

void close() {
    yourCarTexture.free();
    bgTexture.free();

    SDL_DestroyRenderer(gRenderer);
    gWindow.free();

    IMG_Quit();
    SDL_Quit();
}

// Generate column ranges for obstacles
void generateColumnRanges() {
    int gap = (gWindow.getWidth() - Obstacle::OBSTACLE_WIDTH * NUMBER_OF_ROWS) / 5;
    for (int i = 0; i < NUMBER_OF_ROWS; i++) {
        colRanges[i].startX = (gap + Obstacle::OBSTACLE_WIDTH) * i + gap;
        colRanges[i].width = Obstacle::OBSTACLE_WIDTH;
    }
}

int main(int agrc, char* argv[]) {
    srand(time(NULL));

    if (!init()) {
        std::cout << "Failed to init\n";
        return 0;
    }
    if (!loadMedia()) {
        std::cout << "Failed to load media\n";
        return 0;
    }

    bool quit = false;
    SDL_Event e;

    int offsetY = 0;
    int offsetVel = 5;
    generateColumnRanges();

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

        // Infinite scrolling background
        bgTexture.render(0, -bgTexture.getHeight() + offsetY);
        bgTexture.render(0, offsetY);

        for (int i = 0; i < NUMBER_OF_ROWS; i++) {
            if (rand() % 100) continue;
            if (obstacles[i].empty() || obstacles[i].back().getPosY() > Obstacle::OBSTACLE_HEIGHT) {
                Obstacle newObstacle (colRanges[i].startX, 0 - Obstacle::OBSTACLE_HEIGHT, offsetVel + 3);
                obstacles[i].push_back(newObstacle);
                obstacles[i].back().loadTextureImage("assets/images/obstacle.png");
            }
        }
        for (int i = 0; i < NUMBER_OF_ROWS; i++) {
            for (int j = 0; j < (int) obstacles[i].size(); j++) {
                obstacles[i][j].render();
            }
        }
        // Move all obstacles down obstacle's velocity/ frame
        for (int i = 0; i < NUMBER_OF_ROWS; i++) {
            for (int j = 0; j < (int) obstacles[i].size(); j++) {
                obstacles[i][j].setPos(colRanges[i].startX, obstacles[i][j].getPosY() + obstacles[i][j].getVelY());
            }
        }
        // Remove all obstacles that're out of screen
        for (int i = 0; i < NUMBER_OF_ROWS; i++) {
            if (!obstacles[i].empty() && obstacles[i].front().getPosY() >= gWindow.getHeight()) {
                obstacles[i].pop_front();
            }
        }

        yourCar.render();

        SDL_RenderPresent(gRenderer);
        offsetY += offsetVel;
        if (offsetY > bgTexture.getHeight()) {
            offsetY = 0;
        }
    }

    close();

    return 0;
}
