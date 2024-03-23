#include "logic.hpp"
#include "timer.hpp"
#include "assets.hpp"
#include "background.hpp"
#include "car.hpp"
#include "obstacle.hpp"
#include "coin.hpp"
#include "hud.hpp"
#include "gamestate.hpp"
#include "abilities.hpp"
#include "button.hpp"

SDL_Rect column[NUMBER_OF_COLUMNS];
int colVelocity[NUMBER_OF_COLUMNS];

GameWindow win;
GameState state;
Timer frameTimer, // timer to get time per frame
      veloTimer, // timer to change velocity
      storeTimer; // timer to reset store's items

HUD hud               (&win, &state);
Background background (&win, &backgroundTextures, INIT_VELOCITY);
Car player            (&win, SCREEN_WIDTH/2-CAR_WIDTH/2, SCREEN_HEIGHT-2*CAR_HEIGHT, 0);
std::deque<Obstacle>  obstacles[NUMBER_OF_COLUMNS];
std::deque<Coin>      coins[NUMBER_OF_COLUMNS];
std::vector<Button>   storeOption;
int storeItemsId[NUMBER_OF_ITEM_TIER];

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
    loadMedia(&win);

    frameTimer.start();
    veloTimer.start();
    storeTimer.start();

    generateColumnRanges();

    // load abilities
    for (int i = 0; i < NUMBER_OF_ITEM_TIER; i++) {
        std::string path = "abilities/tier";
        path += (char) (i + 1 + '0');
        path += ".txt";
        loadAbilities (i, path);
    }
    // init store
    for (int i = 0; i < NUMBER_OF_ITEM_TIER; i++) {
        storeItemsId[i] = rand() % (int) abils[i].size();
    }
    int x = 40;
    int y = 100;
    for (int i = 0; i < 3; i++) {
        SDL_Rect box = {x, y, 420, 120};
        Button newButton (&win, box, nullptr, nullptr, nullptr);
        storeOption.push_back(newButton);
        y += box.h;
    }

    state.updateCoins(1000); // cheats

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (!state.isPausing()) {
                            std::cout << "pause!\n";
                            state.pause();
                        }
                        else {
                            std::cout << "unpause!\n";
                            state.unpause();
                        }
                        break;
                }
            }
            else if (e.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                for (Button& B : storeOption) {
                    if (!state.isPausing()) continue;
                    B.isPointInsideButton(x, y);
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int tier = 0;
                for (Button& B : storeOption) {
                    if (!state.isPausing()) continue;
                    if (!B.isDisabled()
                        && state.currentCoins() >= abils[tier][storeItemsId[tier]].coins
                        && B.isPointInsideButton(x, y)
                    ) {
                        B.click();
                        activeAbility(&state, tier, storeItemsId[tier]);
                    }
                    tier++;
                }
            }
        }

        if (!state.isPausing() && !state.isGameOver()) {
            player.moveWithMouse();
        }

        //Render
        win.clearRender();
        {
            background.render();
            renderCoins();
            renderObstacles();
            player.render(carTexture);

            hud.drawText(whiteFontTexture, std::to_string(state.currentScore()), 30, 30, 8, 8, 3.0f, HUD_FLOAT_RIGHT);
            hud.drawText(goldenFontTexture, std::to_string(state.currentCoins()), 30, 65, 8, 8, 2.5f, HUD_FLOAT_RIGHT);
            hud.drawHearts(heartSymbolTexture, 30, 30, state.remainLives(), 2.0f, HUD_FLOAT_LEFT);

            renderActiveAbilities(&win, &hud);

            if (state.isGameOver()) {
                hud.renderGameOverScreen();
            }

            if (state.isPausing()) {
                hud.renderPauseScreen();
                hud.renderStore(storeItemsId, storeOption, &storeTimer);
            }
        }
        win.presentRender();

        //Update
        if (!state.isPausing() && !state.isGameOver()) {
            background.update(frameTimer.elapsedTime() / 1000.f);
            updateBgVelocity();
            updateObstacles();
            updateCoins();
            state.updateScore(state.currentScore() + background.getVelY() / 60);
        }

        if (storeTimer.elapsedTime() >= STORE_DURATION * 1000) {
            std::cout << "store reset!\n";
            for (int i = 0; i < NUMBER_OF_ITEM_TIER; i++) {
                storeItemsId[i] = rand() % (int) abils[i].size();
                storeOption[i].reset();
            }
            storeTimer.start();
        }

        updateActiveAbilities();

        frameTimer.start();
    }

    return 0;
}

void generateColumnRanges() {
    int gap = (SCREEN_WIDTH - OBSTACLE_WIDTH * NUMBER_OF_COLUMNS - 2 * ROADSIDE_WIDTH) / (NUMBER_OF_COLUMNS + 1);
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        column[i].x = ROADSIDE_WIDTH + OBSTACLE_WIDTH * i + gap * (i + 1);
        column[i].w = OBSTACLE_WIDTH;
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
            X.setPos(column[i].x, X.getPosY() + X.getVelY() * frameTimer.elapsedTime() / 1000.f);
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
                colVelocity[i] = (rand() % 2 ? randomNumber : 0 - randomNumber);
            }
            Obstacle newObstacle (
                &win,
                column[i].x,
                -OBSTACLE_HEIGHT,
                background.getVelY() + colVelocity[i],
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
            float gap = (column[i].w - COIN_WIDTH) / 2;
            C.setPos(column[i].x + gap, C.posY() + background.getVelY() * frameTimer.elapsedTime() / 1000.f);
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
                Coin C(&win, column[i].x, current_y);
                coins[i].push_back(C);
                current_y -= gap;
            }
        }
    }
}
