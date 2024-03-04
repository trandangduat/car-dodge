#include "logic.hpp"
#include "timer.hpp"
#include "background.hpp"
#include "car.hpp"
#include "obstacle.hpp"
#include "coin.hpp"
#include "hud.hpp"
#include "gamestate.hpp"

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
SDL_Rect colRanges[NUMBER_OF_COLUMNS];
int currentColumnVelocityDif[NUMBER_OF_COLUMNS];

GameWindow win;
GameState state;
Timer frameTimer, // timer to get time per frame
      veloTimer; // timer to change velocity

HUD hud               (&win, &state);
Background background (&win, &backgroundTextures, INIT_VELOCITY);
Car player            (&win, SCREEN_WIDTH/2-CAR_WIDTH/2, SCREEN_HEIGHT-2*CAR_HEIGHT, 0);
std::deque<Obstacle>  obstacles[NUMBER_OF_COLUMNS];
std::deque<Coin>      coins[NUMBER_OF_COLUMNS];

void loadMedia();
void generateColumnRanges();
void updateBgVelocity();

void renderObstacles();
void updateObstacles();
void checkCollisionsWithObstacles();
void manageObstaclesMovement();
void generateObstacles();

void renderCoins();
void updateCoins();
void checkCollisionsWithCoins();
void manageCoinsMovement();
void generateCoins();

int main(int agrc, char* argv[]) {
    srand(time(nullptr));

    win.init();
    loadMedia();

    frameTimer.start();
    veloTimer.start();

    generateColumnRanges();

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (state.isGameOver()) continue;
            player.handleEvent(&e);
        }
        //Render
        win.clearRender();

        background.render();
        renderCoins();
        renderObstacles();
        player.render(carTexture);


        hud.drawText(whiteFontTexture, 30, 30, std::to_string(state.currentScore()), 3, HUD_FLOAT_RIGHT);
        hud.drawText(goldenFontTexture, 30, 65, std::to_string(state.currentCoins()), 2.5f, HUD_FLOAT_RIGHT);
        hud.drawHearts(heartSymbolTexture, 30, 30, state.remainLives(), 2, HUD_FLOAT_LEFT);


        if (state.isGameOver()) {
            SDL_SetRenderDrawColor(win.gRenderer, 0, 0, 0, 180);
            SDL_SetRenderDrawBlendMode(win.gRenderer, SDL_BLENDMODE_BLEND);
            SDL_Rect fullScreenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(win.gRenderer, &fullScreenRect);
            hud.drawText(metalFontTexture, 0, 80, "GAME OVER", 0.6f, HUD_FLOAT_CENTER);
            hud.drawText(whiteFontTexture, 0, 160, "SCORE", 2, HUD_FLOAT_CENTER);
            hud.drawText(whiteFontTexture, 0, 185, std::to_string(state.currentScore()), 4, HUD_FLOAT_CENTER);
        }

        win.presentRender();

        //Update
        background.update(frameTimer.elapsedTime() / 1000.f);
        updateBgVelocity();
        updateObstacles();
        updateCoins();
        state.updateScore(state.currentScore() + background.getVelY() / 60);

        frameTimer.start();
    }

    return 0;
}

void generateColumnRanges() {
    int gap = (SCREEN_WIDTH - OBSTACLE_WIDTH * NUMBER_OF_COLUMNS - 2 * ROADSIDE_WIDTH) / (NUMBER_OF_COLUMNS + 1);
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        colRanges[i].x = ROADSIDE_WIDTH + OBSTACLE_WIDTH * i + gap * (i + 1);
        colRanges[i].w = OBSTACLE_WIDTH;
    }
}

void renderObstacles() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Obstacle& X : obstacles[i]) {
            X.render(X.isCrashed() ? obstacleCrashedSpriteTexture : obstacleSpriteTexture);
        }
    }
}

void updateBgVelocity() {
    if (veloTimer.elapsedTime() >= 5000 && background.getVelY() < 800) {
        state.updateStage(state.currentStage() + 1);
        background.setVelY(background.getVelY() + 30);
        veloTimer.start();
    }
}

void updateObstacles() {
    if (player.isVisible()) {
        checkCollisionsWithObstacles();
    }
    manageObstaclesMovement();
    generateObstacles();
}

void checkCollisionsWithObstacles() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Obstacle& X : obstacles[i]) {
            if (!X.isCrashed() && checkCollision(player.getRect(), X.getRect())) {
                X.crash();
                X.setVelY(background.getVelY());
                state.updateLives(state.remainLives() - 1);

                std::cout << "crashed!\n";
            }
        }
    }
}

void manageObstaclesMovement() {
    // Move obstacles & Remove all obstacles that're off-screen
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Obstacle& X : obstacles[i]) {
            X.setPos(colRanges[i].x, X.getPosY() + X.getVelY() * frameTimer.elapsedTime() / 1000.f);
        }
        while (!obstacles[i].empty() && obstacles[i].front().getPosY() >= SCREEN_HEIGHT) {
            obstacles[i].pop_front();
        }
    }
}

void generateObstacles() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        if (obstacles[i].empty() || obstacles[i].back().getPosY() > OBSTACLE_HEIGHT) {
            if (rand() % 300) continue;
            if (obstacles[i].empty()) {
                int randomNumber = rand() % ((state.currentStage() * 60) / 3) + 60;
                currentColumnVelocityDif[i] = (rand() % 2 ? randomNumber : 0 - randomNumber);
            }
            Obstacle newObstacle (
                &win,
                colRanges[i].x,
                -OBSTACLE_HEIGHT,
                background.getVelY() + currentColumnVelocityDif[i],
                obstaclesClipRect[rand() % (int) obstaclesClipRect.size()]
            );
            obstacles[i].push_back(newObstacle);
        }
    }
}

void renderCoins() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Coin& C : coins[i]) {
            if (!C.isClaimed()) C.render(coinSprite);
        }
    }
}

void updateCoins() {
    checkCollisionsWithCoins();
    manageCoinsMovement();
    generateCoins();
}

void checkCollisionsWithCoins() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Coin& C : coins[i]) {
            if (!C.isClaimed() && checkCollision(player.getRect(), C.getRect())) {
                C.claimed();
                state.updateCoins(state.currentCoins() + 1);
            }
        }
    }
}

void manageCoinsMovement() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Coin& C : coins[i]) {
            /*
                using 'gap' offset to put the coin in the center of the road column
            */
            float gap = (colRanges[i].w - COIN_WIDTH) / 2;
            C.setPos(colRanges[i].x + gap, C.posY() + background.getVelY() * frameTimer.elapsedTime() / 1000.f);
            C.animate();
        }
        while (!coins[i].empty() && coins[i].front().posY() >= SCREEN_HEIGHT) {
            coins[i].pop_front();
        }
    }
}

void generateCoins() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        if (coins[i].empty()) {
            if (rand() % 500) continue;
            int numberOfCoins = rand() % 6 + 5;
            int gap = 25;
            float current_y = 0;
            while (numberOfCoins--) {
                current_y -= COIN_HEIGHT;
                Coin C(&win, colRanges[i].x, current_y);
                coins[i].push_back(C);
                current_y -= gap;
            }
        }
    }
}

void loadMedia() {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    backgroundTextures[0]           = win.loadTexture("assets/images/road_5.png");
    backgroundTextures[1]           = win.loadTexture("assets/images/road_6.png");
    carTexture                      = win.loadTexture("assets/images/car.png");
    carInvisibleTexture             = win.loadTexture("assets/images/car_invisible.png");
    obstacleSpriteTexture           = win.loadTexture("assets/images/cars.png");
    obstacleCrashedSpriteTexture    = win.loadTexture("assets/images/cars_crashed.png");
    coinSprite                      = win.loadTexture("assets/images/items/coin.png");
    goldenFontTexture               = win.loadTexture("assets/fonts/golden.png");
    whiteFontTexture                = win.loadTexture("assets/fonts/white.png");
    metalFontTexture                = win.loadTexture("assets/fonts/metal.png");
    heartSymbolTexture              = win.loadTexture("assets/images/HUD/heart.png");
    obstaclesClipRect.push_back({253, 9, 49, 92});
    obstaclesClipRect.push_back({314, 9, 49, 92});
    obstaclesClipRect.push_back({373, 9, 49, 92});
    obstaclesClipRect.push_back({69, 335, 46, 87});
    obstaclesClipRect.push_back({127, 335, 46, 87});
}
