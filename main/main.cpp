#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<deque>
#include<cstdlib>
#include<ctime>
#include<algorithm>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int NUMBER_OF_COLUMNS = 4;

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

class Car {
public:
    static const int CAR_WIDTH = 50;
    static const int CAR_HEIGHT = 100;

    Car();

    void loadTexture (std::string path);
    void handleEvent (SDL_Event* e);
    void moveTo (int x, int y);
    void render (SDL_Texture* texture);
    int getPosX();
    int getPosY();

private:
    SDL_Rect mRect;
};

class Obstacle {
public:
    static const int OBSTACLE_WIDTH = 50;
    static const int OBSTACLE_HEIGHT = 100;

    Obstacle();
    Obstacle (int x, int y, int vel_y, bool isCrashed = false);

    void render (SDL_Texture* texture);
    void setPos (int x, int y);
    void setVelY (int velY);
    void crash();
    bool checkCrashed();

    int getVelY();
    int getPosX();
    int getPosY();

private:
    SDL_Rect mRect;
    int mVelY;
    bool mIsCrashed;
};

struct columnRange {
    int startX;
    int width;
};

bool init();
bool loadMedia();
void render();
void update();
void close();

SDL_Texture* loadTexture (std::string path);
SDL_Texture* loadTexture (TTF_Font* font, std::string text, SDL_Color textColor);
void blit (SDL_Texture* texture, SDL_Rect rect);
void blit (SDL_Texture* texture, int x, int y, int w = -1, int h = -1);

void generateColumnRanges();
bool checkCollision (SDL_Rect A, SDL_Rect B);
bool checkCollision (Car A, Obstacle B);
std::string toString (int num) {
    if (num == 0) return "0";
    std::string str = "";
    while (num) {
        str += (char) (num % 10 + '0');
        num /= 10;
    }
    std::reverse(str.begin(), str.end());
    return str;
}

LWindow gWindow;
SDL_Renderer* gRenderer = nullptr;

SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* carTexture = nullptr;
SDL_Texture* obstacleTexture = nullptr;
SDL_Texture* obstacleCrashedTexture = nullptr;
SDL_Texture* crashesCounterTexture = nullptr;

//Globally used font
TTF_Font* gFont = nullptr;

int crashesCounter = 0;

Car yourCar;
columnRange colRanges[4];
std::deque<Obstacle> obstacles[4];
int offsetY = 0;
int offsetVel = 7;

//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//

LWindow::LWindow() {
    mWindow = nullptr;
    mWidth = 0;
    mHeight = 0;
}
bool LWindow::init() {
    mWindow = SDL_CreateWindow("Car Dodge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == nullptr) {
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
    mWindow = nullptr;
    mWidth = 0;
    mHeight = 0;
}
int LWindow::getWidth() {
    return mWidth;
}
int LWindow::getHeight() {
    return mHeight;
}

Car::Car() {
    mRect.x = 0;
    mRect.y = 0;
    mRect.w = Car::CAR_WIDTH;
    mRect.h = Car::CAR_HEIGHT;
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
    mRect.x = x;
    mRect.y = y;
}
void Car::render(SDL_Texture* texture) {
    blit(texture, mRect);
}
int Car::getPosX() {
    return mRect.x;
}
int Car::getPosY() {
    return mRect.y;
}

Obstacle::Obstacle() {
    mRect.x = 0;
    mRect.y = 0;
    mRect.w = Obstacle::OBSTACLE_WIDTH;
    mRect.h = Obstacle::OBSTACLE_HEIGHT;
    mVelY = 0;
    mIsCrashed = false;
}
Obstacle::Obstacle (int x, int y, int vel_y, bool isCrashed) {
    mRect.x = x;
    mRect.y = y;
    mRect.w = Obstacle::OBSTACLE_WIDTH;
    mRect.h = Obstacle::OBSTACLE_HEIGHT;
    mVelY = vel_y;
    mIsCrashed = isCrashed;
}
void Obstacle::render (SDL_Texture* texture) {
    blit(texture, mRect);
}
void Obstacle::setPos (int x, int y) {
    mRect.x = x;
    mRect.y = y;
}
void Obstacle::setVelY (int velY) {
    mVelY = velY;
}
void Obstacle::crash() {
    mIsCrashed = true;
}
int Obstacle::getPosX() {
    return mRect.x;
}
int Obstacle::getPosY() {
    return mRect.y;
}
int Obstacle::getVelY() {
    return mVelY;
}
bool Obstacle::checkCrashed() {
    return mIsCrashed;
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
    if (gRenderer == nullptr) {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << '\n';
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //Initialize PNG & JPG loading
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!( IMG_Init(imgFlags) & imgFlags )) {
        std::cout << "IMG_Init failed: " << IMG_GetError() << '\n';
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cout << "TTF_Init failed: " << TTF_GetError() << '\n';
        return false;
    }
    return true;
}

bool loadMedia() {
    bool success = true;
    backgroundTexture       = loadTexture("assets/images/road.png");
    carTexture              = loadTexture("assets/images/car.png");
    obstacleTexture         = loadTexture("assets/images/obstacle.png");
    obstacleCrashedTexture  = loadTexture("assets/images/obstacle_crashed.png");

    gFont = TTF_OpenFont("assets/fonts/OpenSans.ttf", 20);
    if (gFont == nullptr) {
        std::cout << "Failed to load font: " << TTF_GetError() << '\n';
    }

    crashesCounterTexture = loadTexture(gFont, "Crashes counter: 0", {255, 255, 255, 255});

    success &= (backgroundTexture != nullptr);
    success &= (carTexture != nullptr);
    success &= (obstacleTexture != nullptr);
    success &= (obstacleCrashedTexture != nullptr);
    success &= (gFont != nullptr);
    success &= (crashesCounterTexture != nullptr);

    return success;
}

void render() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    // Infinite scrolling background
    // TODO: create background class, change 'gWindow.getHeight()' to 'background.getHeight()'
    blit(backgroundTexture, 0, -gWindow.getHeight() + offsetY);
    blit(backgroundTexture, 0, offsetY);

    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (int j = 0; j < (int) obstacles[i].size(); j++) {
            obstacles[i][j].render(obstacles[i][j].checkCrashed() ? obstacleCrashedTexture : obstacleTexture);
        }
    }

    yourCar.render(carTexture);
    blit(crashesCounterTexture, 0, 0);

    SDL_RenderPresent(gRenderer);
}

void update() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (int j = 0; j < (int) obstacles[i].size(); j++) {
            if (!obstacles[i][j].checkCrashed() && checkCollision(yourCar, obstacles[i][j])) {
                obstacles[i][j].crash();
                crashesCounter++;
//                SDL_DestroyTexture(crashesCounterTexture);
                crashesCounterTexture = loadTexture(gFont,
                                                    "Crashes counter: " + toString(crashesCounter),
                                                    {255, 255, 255, 255}
                                                    );
            }
        }
    }
    // Move down obstacles & Remove all obstacles that're off-screen
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (int j = 0; j < (int) obstacles[i].size(); j++) {
            obstacles[i][j].setPos(colRanges[i].startX, obstacles[i][j].getPosY() + obstacles[i][j].getVelY());
        }
        for (int j = 0; j < (int) obstacles[i].size(); j++) {
            if (!obstacles[i].empty() && obstacles[i].front().getPosY() >= gWindow.getHeight()) {
                obstacles[i].pop_front();
            }
        }
    }
    // Generate new obstacles
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        if (rand() % 100) continue;
        if (obstacles[i].empty() || obstacles[i].back().getPosY() > Obstacle::OBSTACLE_HEIGHT) {
            Obstacle newObstacle (colRanges[i].startX,
                                  -Obstacle::OBSTACLE_HEIGHT,
                                  offsetVel - 2);

            obstacles[i].push_back(newObstacle);
        }
    }

    offsetY += offsetVel;
    if (offsetY > gWindow.getHeight()) {
        offsetY = 0;
    }
}

void close() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(carTexture);
    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyTexture(obstacleCrashedTexture);

    SDL_DestroyRenderer(gRenderer);
    TTF_CloseFont(gFont);
    gWindow.free();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture (std::string path) {
    SDL_Texture *texture;
    texture = IMG_LoadTexture(gRenderer, path.c_str());
    if (texture == nullptr) {
        std::cout << "Failed to load texture from " << path << ' ' << IMG_GetError() << '\n';
    }
    return texture;
}
SDL_Texture* loadTexture (TTF_Font* font, std::string text, SDL_Color textColor) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (surface == nullptr) {
        std::cout << "Failed to load text surface " << SDL_GetError() << '\n';
    }
    else {
        texture = SDL_CreateTextureFromSurface(gRenderer, surface);
        if (texture == nullptr) {
            std::cout << "Failed to convert text surface to texture " << SDL_GetError() << '\n';
        }
    }
    SDL_FreeSurface(surface);
    return texture;
}
void blit (SDL_Texture* texture, SDL_Rect rect) {
    SDL_Rect dest = rect;
    SDL_RenderCopy(gRenderer, texture, nullptr, &dest);
}
void blit (SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    if (dest.w == -1 && dest.h == -1) {
        SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
    }
    SDL_RenderCopy(gRenderer, texture, nullptr, &dest);
}

// Generate column ranges for obstacles
void generateColumnRanges() {
    int gap = (gWindow.getWidth() - Obstacle::OBSTACLE_WIDTH * NUMBER_OF_COLUMNS) / 5;
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        colRanges[i].startX = (gap + Obstacle::OBSTACLE_WIDTH) * i + gap;
        colRanges[i].width = Obstacle::OBSTACLE_WIDTH;
    }
}

bool checkCollision (SDL_Rect A, SDL_Rect B) {
    return (A.x + A.w >= B.x && B.x + B.w >= A.x &&
            A.y + A.h >= B.y && B.y + B.h >= A.y);
}
bool checkCollision (Car A, Obstacle B) {
    SDL_Rect a;
    a.x = A.getPosX();
    a.y = A.getPosY();
    a.w = Car::CAR_WIDTH;
    a.h = Car::CAR_HEIGHT;

    SDL_Rect b;
    b.x = B.getPosX();
    b.y = B.getPosY();
    b.w = Obstacle::OBSTACLE_WIDTH;
    b.h = Obstacle::OBSTACLE_HEIGHT;

    return checkCollision(a, b);
}

int main(int agrc, char* argv[]) {
    srand(time(nullptr));

    if (!init()) {
        std::cout << "Failed to init\n";
        return 0;
    }
    if (!loadMedia()) {
        std::cout << "Failed to load media\n";
        return 0;
    }

    generateColumnRanges();

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // Handle events here
            yourCar.handleEvent(&e);
        }
        render();
        update();
    }

    close();

    return 0;
}
