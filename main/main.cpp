#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<deque>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<algorithm>

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 600;
const int ROADSIDE_WIDTH = 90;
const int CAR_WIDTH = 50;
const int CAR_HEIGHT = 100;
const int OBSTACLE_WIDTH = 50;
const int OBSTACLE_HEIGHT = 100;
const int NUMBER_OF_COLUMNS = 4;
const int ITEM_WIDTH = 50;
const int ITEM_HEIGHT = ITEM_WIDTH;
enum item_type {
    SPEED_BOOST_ITEM,
    INVISIBLE_ITEM,
    TOTAL_OF_ITEMS
};
const int SPEED_BOOST_AMOUNT = 4;

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
    Car();
    Car (int x, int y, int velocity);
    void loadTexture (std::string path);
    void handleEvent (SDL_Event* e);
    void moveTo (int x, int y);
    void render (SDL_Texture* texture);
    void setVelY (int velocity);
    void setVisible (bool state);
    int getPosX();
    int getPosY();
    int getVelY();
    int getVisibleState();

private:
    SDL_Rect mRect;
    int mVelY;
    bool mVisible;
};

class Obstacle {
public:
    Obstacle();
    Obstacle (int x, int y, int vel_y, SDL_Rect clip, bool isCrashed = false);
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
    SDL_Rect mClip;
    int mVelY;
    bool mIsCrashed;
};

class Item {
public:
    Item();
    Item (int type, int duration, int velocity, int x, int y);
    void render (SDL_Texture* texture);
    void setPos (int x, int y);
    void setVelY (int velocity);
    int getPosX();
    int getPosY();
    int getVelY();
    int getType();
    int getDuration();
    bool checkClaimed();
    void claim();

private:
    int mType;
    int mDuration; // in frames
    int mVelY;
    bool mIsClaimed;
    SDL_Rect mRect;
};

struct columnRange {
    int startX;
    int width;
};

bool init();
bool loadMedia();
void render();
void updateObstacles();
void updateItems();
void turnOffItem (int item);
void turnOnItem (int item);
void update();
void close();

SDL_Texture* loadTexture (std::string path);
SDL_Texture* loadTexture (TTF_Font* font, std::string text, SDL_Color textColor);
void blit (SDL_Texture* texture, SDL_Rect clip, SDL_Rect rect);
void blit (SDL_Texture* texture, SDL_Rect rect);
void blit (SDL_Texture* texture, int x, int y, int w = -1, int h = -1);

void generateColumnRanges();
bool checkCollision (SDL_Rect A, SDL_Rect B);
bool checkCollision (Car A, Obstacle B);
bool checkCollision (Car A, Item B);
std::string toString (int num);

LWindow gWindow;
SDL_Renderer* gRenderer = nullptr;

SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* carTexture = nullptr;
SDL_Texture* carInvisibleTexture = nullptr;
SDL_Texture* obstacleSpriteTexture = nullptr;
SDL_Texture* obstacleCrashedSpriteTexture = nullptr;
SDL_Texture* crashesCounterTexture = nullptr;
SDL_Texture* itemTextures[TOTAL_OF_ITEMS];

std::vector<SDL_Rect> obstaclesClipRect;

TTF_Font* gFont = nullptr;

Car yourCar(0,0, 6);
columnRange colRanges[NUMBER_OF_COLUMNS];

std::deque<Obstacle>  obstacles[NUMBER_OF_COLUMNS];
std::deque<Item>      items[NUMBER_OF_COLUMNS];

int currentItemsDuration[TOTAL_OF_ITEMS];
int crashesCounter = 0;
int offsetY = 0;

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

    generateColumnRanges();
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

Car::Car (int x, int y, int velocity) {
    mRect.x = x;
    mRect.y = y;
    mRect.w = CAR_WIDTH;
    mRect.h = CAR_HEIGHT;
    mVelY = velocity;
    mVisible = true;
}
void Car::handleEvent (SDL_Event *e) {
    if (e->type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        moveTo(x - CAR_WIDTH / 2, gWindow.getHeight() - 2 * CAR_HEIGHT);
    }
}
void Car::moveTo (int x, int y) {
    if (x < ROADSIDE_WIDTH) {
        x = ROADSIDE_WIDTH;
    }
    if (x + CAR_WIDTH > gWindow.getWidth() - ROADSIDE_WIDTH) {
        x = gWindow.getWidth() - ROADSIDE_WIDTH - CAR_WIDTH;
    }
    mRect.x = x;
    mRect.y = y;
}
void Car::render(SDL_Texture* texture) {
    blit(texture, mRect);
}
void Car::setVelY (int velocity) {
    mVelY = velocity;
}
void Car::setVisible (bool state) {
    mVisible = state;
}
int Car::getPosX() {
    return mRect.x;
}
int Car::getPosY() {
    return mRect.y;
}
int Car::getVelY() {
    return mVelY;
}
int Car::getVisibleState() {
    return mVisible;
}

Obstacle::Obstacle (int x, int y, int vel_y, SDL_Rect clip, bool isCrashed) {
    mRect.x = x;
    mRect.y = y;
    mRect.w = OBSTACLE_WIDTH;
    mRect.h = OBSTACLE_HEIGHT;
    mClip = clip;
    mVelY = vel_y;
    mIsCrashed = isCrashed;
}
void Obstacle::render (SDL_Texture* texture) {
    blit(texture, mClip, mRect);
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

Item::Item (int type, int duration, int velocity, int x, int y) {
    mType = type;
    mDuration = duration;
    mVelY = velocity;
    mRect.x = x;
    mRect.y = y;
    mRect.w = ITEM_WIDTH;
    mRect.h = ITEM_HEIGHT;
}
void Item::render (SDL_Texture* texture) {
    blit(texture, mRect);
}
void Item::setPos (int x, int y) {
    mRect.x = x;
    mRect.y = y;
}
void Item::setVelY (int velocity) {
    mVelY = velocity;
}
int Item::getPosX() {
    return mRect.x;
}
int Item::getPosY() {
    return mRect.y;
}
int Item::getVelY() {
    return mVelY;
}
int Item::getType() {
    return mType;
}
int Item::getDuration() {
    return mDuration;
}
bool Item::checkClaimed() {
    return mIsClaimed;
}
void Item::claim() {
    mIsClaimed = true;
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
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "IMG_Init failed: " << IMG_GetError() << '\n';
        return false;
    }
    if (TTF_Init() == -1) {
        std::cout << "TTF_Init failed: " << TTF_GetError() << '\n';
        return false;
    }
    return true;
}

bool loadMedia() {
    bool success = true;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    backgroundTexture               = loadTexture("assets/images/road_3.png");
    carTexture                      = loadTexture("assets/images/car.png");
    carInvisibleTexture             = loadTexture("assets/images/car_invisible.png");
    obstacleSpriteTexture           = loadTexture("assets/images/cars.png");
    obstacleCrashedSpriteTexture    = loadTexture("assets/images/cars_crashed.png");
    itemTextures[SPEED_BOOST_ITEM]  = loadTexture("assets/images/items/speed_boost.png");
    itemTextures[INVISIBLE_ITEM]    = loadTexture("assets/images/items/invisible.png");
    gFont                           = TTF_OpenFont("assets/fonts/OpenSans.ttf", 20);
    crashesCounterTexture           = loadTexture(gFont, "Crashes counter: 0", {255, 255, 255, 255});

    obstaclesClipRect.push_back({72, 6, 46, 91});
    obstaclesClipRect.push_back({253, 9, 49, 92});
    obstaclesClipRect.push_back({314, 9, 49, 92});
    obstaclesClipRect.push_back({373, 9, 49, 92});

    obstaclesClipRect.push_back({14, 335, 46, 87});
    obstaclesClipRect.push_back({69, 335, 46, 87});
    obstaclesClipRect.push_back({127, 335, 46, 87});

    if (gFont == nullptr)
        std::cout << "Failed to load font: " << TTF_GetError() << '\n';

    success &= (backgroundTexture != nullptr);
    success &= (carTexture != nullptr);
    success &= (obstacleSpriteTexture != nullptr);
    success &= (obstacleCrashedSpriteTexture != nullptr);
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
            obstacles[i][j].render(obstacles[i][j].checkCrashed() ? obstacleCrashedSpriteTexture : obstacleSpriteTexture);
        }
    }

    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (int j = 0; j < (int) items[i].size(); j++) {
            items[i][j].render(itemTextures[items[i][j].getType()]);
        }
    }

    yourCar.render((yourCar.getVisibleState() == false ? carInvisibleTexture : carTexture));
    blit(crashesCounterTexture, 0, 0);

    SDL_RenderPresent(gRenderer);
}

void updateObstacles() {
    // Check collisions
    if (yourCar.getVisibleState() == true) {
        for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
            for (int j = 0; j < (int) obstacles[i].size(); j++) {
                if (!obstacles[i][j].checkCrashed() && checkCollision(yourCar, obstacles[i][j])) {
                    obstacles[i][j].crash();
                    obstacles[i][j].setVelY(yourCar.getVelY());
                    crashesCounter++;
                    crashesCounterTexture = loadTexture(gFont,
                                                        "Crashes counter: " + toString(crashesCounter),
                                                        {255, 255, 255, 255}
                                                        );
                }
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
        if (rand() % 300) continue;
        if ((obstacles[i].empty() || obstacles[i].back().getPosY() > OBSTACLE_HEIGHT) && (items[i].empty() || items[i].back().getPosY() > OBSTACLE_HEIGHT)) {
            Obstacle newObstacle (colRanges[i].startX, -OBSTACLE_HEIGHT, 4, obstaclesClipRect[rand() % (int) obstaclesClipRect.size()]);
            obstacles[i].push_back(newObstacle);
        }
    }
}

void updateItems() {
    // Decrease items' duration by 1
    for (int i = 0; i < TOTAL_OF_ITEMS; i++) {
        if (currentItemsDuration[i] - 1 == 0) {
            turnOffItem(i);
        }
        currentItemsDuration[i] = std::max(currentItemsDuration[i] - 1, 0);
    }
    // Check collisions
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (int j = 0; j < (int) items[i].size(); j++) {
            if (!items[i][j].checkClaimed() && checkCollision(yourCar, items[i][j])) {
                items[i][j].claim();
                if (!currentItemsDuration[items[i][j].getType()]) {
                    turnOnItem(items[i][j].getType());
                }
                currentItemsDuration[items[i][j].getType()] = items[i][j].getDuration(); // reset duration
            }
        }
    }
    // Move down items & Remove all items that're off-screen or claimed
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (int j = 0; j < (int) items[i].size(); j++) {
            items[i][j].setPos(colRanges[i].startX, items[i][j].getPosY() + items[i][j].getVelY());
        }
        for (int j = 0; j < (int) items[i].size(); j++) {
            if (!items[i].empty() && (items[i].front().getPosY() >= gWindow.getHeight() || items[i].front().checkClaimed())) {
                items[i].pop_front();
            }
        }
    }
    // Generate new items
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        if (rand() % 1000) continue;
        if ((obstacles[i].empty() || obstacles[i].back().getPosY() > ITEM_HEIGHT) && (items[i].empty() || items[i].back().getPosY() > ITEM_HEIGHT)) {
            Item newItem (rand() % TOTAL_OF_ITEMS, 360, 4, colRanges[i].startX, -ITEM_HEIGHT);
            items[i].push_back(newItem);
        }
    }
}

void turnOffItem (int item) {
    switch (item) {
        case SPEED_BOOST_ITEM:
            yourCar.setVelY(yourCar.getVelY() - SPEED_BOOST_AMOUNT);
            break;

        case INVISIBLE_ITEM:
            yourCar.setVisible(true);
            break;
    }
}

void turnOnItem (int item) {
    switch (item) {
        case SPEED_BOOST_ITEM:
            yourCar.setVelY(yourCar.getVelY() + SPEED_BOOST_AMOUNT);
            break;

        case INVISIBLE_ITEM:
            yourCar.setVisible(false);
            break;
    }
}

void update() {
    updateItems();
    updateObstacles();

    offsetY += yourCar.getVelY();
    if (offsetY > gWindow.getHeight()) {
        offsetY = 0;
    }
}

void close() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(carTexture);
    SDL_DestroyTexture(carInvisibleTexture);
    SDL_DestroyTexture(obstacleSpriteTexture);
    SDL_DestroyTexture(obstacleCrashedSpriteTexture);
    SDL_DestroyTexture(crashesCounterTexture);
    for (int i = 0; i < TOTAL_OF_ITEMS; i++) {
        SDL_DestroyTexture(itemTextures[i]);
    }

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
void blit (SDL_Texture* texture, SDL_Rect clip, SDL_Rect rect) {
    SDL_Rect srect = clip;
    SDL_Rect drect = rect;
    SDL_RenderCopy(gRenderer, texture, &srect, &drect);
}
void blit (SDL_Texture* texture, SDL_Rect rect) {
    SDL_Rect drect = rect;
    SDL_RenderCopy(gRenderer, texture, nullptr, &drect);
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
    int gap = (gWindow.getWidth() - OBSTACLE_WIDTH * NUMBER_OF_COLUMNS - 2 * ROADSIDE_WIDTH) / 5;
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        colRanges[i].startX = ROADSIDE_WIDTH + OBSTACLE_WIDTH * i + gap * (i + 1);
        colRanges[i].width = OBSTACLE_WIDTH;
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
    a.w = CAR_WIDTH;
    a.h = CAR_HEIGHT;

    SDL_Rect b;
    b.x = B.getPosX();
    b.y = B.getPosY();
    b.w = OBSTACLE_WIDTH;
    b.h = OBSTACLE_HEIGHT;

    return checkCollision(a, b);
}
bool checkCollision (Car A, Item B) {
    SDL_Rect a;
    a.x = A.getPosX();
    a.y = A.getPosY();
    a.w = CAR_WIDTH;
    a.h = CAR_HEIGHT;

    SDL_Rect b;
    b.x = B.getPosX();
    b.y = B.getPosY();
    b.w = ITEM_WIDTH;
    b.h = ITEM_HEIGHT;

    return checkCollision(a, b);
}


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


// https://www.youtube.com/watch?v=xAgUYyosqVM --> this song's fire af
