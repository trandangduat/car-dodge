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
#include "bullet.hpp"
#include "vfx.hpp"
#include "boss.hpp"

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
VFX speedBoostEffect,
    bossUltimateFx;
Boss* boss = new Boss(&win);

std::deque<Obstacle>  obstacles[NUMBER_OF_COLUMNS];
std::deque<Coin>      coins[NUMBER_OF_COLUMNS];
std::deque<Bullet>    firedBullets;
std::vector<Button>   storeOption;
int storeItemsId[NUMBER_OF_ABILITY_TIER];

void generateColumnRanges();
void updateBgVelocity();
SDL_Point getNearestObstacle (float x, float y);

void renderObstacles();
void updateObstacles();
void checkCollisionsWithPlayer();
void checkCollisionsWithBullets();
void manageObstaclesMovement();
void generateObstacles();

void renderCoins();
void updateCoins();
void checkCollisionsWithCoins();
void manageCoinsMovement();
void generateCoins();

void renderBullets();
void updateBullets();
void generateBullet();

void renderAIBoss();
void updateAIBoss();
void checkCollisionWithBossUltimate();

void useAbilities();

int main(int agrc, char* argv[]) {
    srand(time(nullptr));

    win.init();
    loadMedia(&win);

    frameTimer.start();
    veloTimer.start();
    storeTimer.start();

    generateColumnRanges();
    loadAbilitiesFromFiles();

    speedBoostEffect    = VFX(&win, 0, 0, 0, 0, gasSmoke, 32, 32, 50);
    bossUltimateFx      = VFX(&win, 0, 0, 0, 0, bossLaser, 10, 48, 50);
    boss->updateTexture(bossSprite, 48, 41);

    // init store
    for (int i = 0; i < NUMBER_OF_ABILITY_TIER; i++) {
        storeItemsId[i] = rand() % (int) abils[i].size();
    }
    int x = 100;
    int y = 100;
    for (int i = 0; i < 3; i++) {
        SDL_Rect box = {x, y, SCREEN_WIDTH - x * 2, 120};
        Button newButton (&win, box, nullptr, nullptr, nullptr);
        storeOption.push_back(newButton);
        y += box.h;
    }

    state.updateCoins(9999); // cheats
    state.updateBullets(999);

    bool quit = false;
    SDL_Event e;
    std::deque<std::pair<int, int>> abilitiesToActive;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (!state.isPausing()) {
                            std::clog << "pause!\n";
                            state.pause();
                        }
                        else {
                            std::clog << "unpause!\n";
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
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                switch (e.button.button) {
                    case SDL_BUTTON_LEFT: {
                        if (!state.isPausing()) {
                            if (state.currentBullets() > 0) {
                                generateBullet();
                                state.updateBullets(state.currentBullets() - 1);
                            }
                        }
                        else {
                            int tier = 0;
                            for (Button& B : storeOption) {
                                if (!B.isDisabled()
                                    && state.currentCoins() >= abils[tier][storeItemsId[tier]].coins
                                    && B.isPointInsideButton(mouseX, mouseY)
                                ) {
                                    B.click();
                                    abilitiesToActive.push_back({tier, storeItemsId[tier]});
                                }
                                tier++;
                            }
                        }
                    }
                }
            }
        }

        if (!state.isPausing() && !state.isGameOver()) {
            player.moveWithMouse();
            while (!abilitiesToActive.empty()) {
                int tier = abilitiesToActive.front().first;
                int id   = abilitiesToActive.front().second;
                activeAbility(&state, tier, id);
                abilitiesToActive.pop_front();
            }
            for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
                for (Ability& A : abils[tier]) {
                    if (A.isActive) A.timer->unpause();
                }
            }
        }
        else if (state.isPausing()) {
            for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
                for (Ability& A : abils[tier]) {
                    if (A.isActive) A.timer->pause();
                }
            }
        }

        //Render
        win.clearRender();
        {
            background.render();
            renderCoins();
            renderObstacles();
            if (state.speedBoostIsEnabled()) {
                speedBoostEffect.render(0, SDL_FLIP_VERTICAL);
            }
            player.render(carTexture);
            renderBullets();
            renderAIBoss();

            // LEFT
            hud.drawHearts(heartSymbolTexture, 30, 30, state.remainLives(), 2.0f, HUD_FLOAT_LEFT);

            // RIGHT
            hud.drawText(
                state.speedBoostIsEnabled() ? blueFontTexture : whiteFontTexture,
                std::to_string(state.currentScore()),
                27, 30, 8, 8, 3.5f,
                HUD_FLOAT_RIGHT
            );
            hud.drawText(
                goldenFontTexture,
                std::to_string(state.currentCoins()) + " #",
                20, 65, 8, 8, 2.5f,
                HUD_FLOAT_RIGHT
            );
            win.blit(bulletIcon, {SCREEN_WIDTH - 30, SCREEN_HEIGHT - 55, 12, 35});
            hud.drawText(whiteFontTexture, std::to_string(state.currentBullets()), 35, SCREEN_HEIGHT - 45, 8, 8, 2.5f, HUD_FLOAT_RIGHT);

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
            updateBullets();
            updateAIBoss();
            if (state.speedBoostIsEnabled()) {
                speedBoostEffect.animate();
                speedBoostEffect.mRect.w = player.getRect().w;
                speedBoostEffect.mRect.h = player.getRect().h;
                speedBoostEffect.setPos(
                    player.getPosX() + player.getRect().w / 2 - speedBoostEffect.mRect.w / 2,
                    player.getPosY() + player.getRect().h - 10
                );
            }

            state.updateScore(state.currentScore() + background.getVelY() / 60);
        }

        if (storeTimer.elapsedTime() >= STORE_DURATION * 1000) {
            std::clog << "store reset!\n";
            for (int i = 0; i < NUMBER_OF_ABILITY_TIER; i++) {
                storeItemsId[i] = rand() % (int) abils[i].size();
                storeOption[i].reset();
            }
            storeTimer.start();
        }

        updateActiveAbilities();
        useAbilities();

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
            X.render();
        }
    }
}

SDL_Point getNearestObstacle (float x, float y) {
    SDL_Point ret = {oo, oo};
    float shortestDistanceSq = oo;
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Obstacle& O : obstacles[i]) {
            if (O.isCrashed()) continue;

            float distanceSq = (x - O.getPosX()) * (x - O.getPosX()) + (y - O.getPosY()) * (y - O.getPosY());
            if (distanceSq < shortestDistanceSq) {
                ret.x = O.getPosX();
                ret.y = O.getPosY();
                shortestDistanceSq = distanceSq;
            }
        }
    }
    return ret;
}

void updateBgVelocity() {
    if (veloTimer.elapsedTime() >= 5000 && background.getVelY() < 800) {
        state.updateStage(state.currentStage() + 1);
        background.setVelY(background.getVelY() + 30);
        veloTimer.start();
    }
}

void updateObstacles() {
    /*
        if the player is invisible then we do not check
        collisions with the obstacles
    */
    if (player.isVisible()) {
        checkCollisionsWithPlayer();
    }
    checkCollisionsWithBullets();
    manageObstaclesMovement();
    generateObstacles();
}

void checkCollisionsWithPlayer() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Obstacle& X : obstacles[i]) {
            if (!X.isCrashed() && checkCollision(player.getRect(), X.getRect())) {
                X.crash();
                X.setVelY(background.getVelY());
                state.updateLives(state.remainLives() - 1);

                std::clog << "crashed!\n";
            }
        }
    }
}

void checkCollisionsWithBullets() {
    for (Bullet& B : firedBullets) {
        if (B.getState() == BULLET_EXPLODED) continue;
        bool bulletHitsObstacle = false;
        for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
            for (Obstacle& X : obstacles[i]) {
                if (!X.isCrashed() && checkCollision(B.getRect(), X.getRect())) {
                    X.crash();
                    X.setVelY(background.getVelY());
                    bulletHitsObstacle = true;
                    std::clog << "bullet hits obstacle!";
                    break;
                }
            }
            if (bulletHitsObstacle) {
                break;
            }
        }
        if (bulletHitsObstacle) {
            B.setState(BULLET_EXPLODED);
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
                int randomNumber = rand() % ((state.currentStage() * 60) / 4) + 60;
                colVelocity[i] = (rand() % 2 ? randomNumber : 0 - randomNumber);
            }
            Obstacle newObstacle (
                &win,
                obstaclesTexture,
                obstaclesCrashedTexture,
                obstaclesCrashedWhiteTexture,
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
                state.updateCoins(state.currentCoins() + COIN_MULTIPLIER);
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
            if (!C.isClaimed() && state.magnetIsEnabled() && abs(C.posY() - player.getPosY()) <= 50) {
                float dx = player.getPosX() + player.getRect().w / 2 - C.posX();
                float dy = player.getPosY() - C.posY();
                C.setPos(
                    C.posX() + dx * MOVEMENT_DELAY,
                    C.posY() + dy * MOVEMENT_DELAY
                );
            } else {
                C.setPos(
                    column[i].x + gap,
                    C.posY() + background.getVelY() * frameTimer.elapsedTime() / 1000.f
                );
            }
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

void renderBullets() {
    for (Bullet& B : firedBullets) {
        if (B.getState() != BULLET_EXPLODED) {
            B.render();
        }
    }
}

void updateBullets() {
    for (Bullet& B : firedBullets) {
        if (B.getRect().y < 0 || B.getRect().y > SCREEN_HEIGHT) {
            B.setState(BULLET_EXPLODED);
            continue;
        }
        SDL_Point nearestObstacle = getNearestObstacle(B.getRect().x, B.getRect().y);
        B.move(nearestObstacle, frameTimer.elapsedTime() / 1000.f);
    }
    while (!firedBullets.empty() && firedBullets.back().getState() == BULLET_EXPLODED) {
        firedBullets.pop_front();
    }
}

void generateBullet() {
    Bullet bullet (
        &win,
        &state,
        player.getPosX() + player.getRect().w / 2 - BULLET_WIDTH / 2,
        player.getPosY()
    );
    firedBullets.push_back(bullet);
}

void renderAIBoss() {
    boss->render();
    if (boss->getState() == BOSS_ULTING) {
        bossUltimateFx.render(0, SDL_FLIP_NONE);
    }
}

void updateAIBoss() {
    boss->animate();
    boss->move(player.getPosX(), rand() % 100);
    if (boss->getState() == BOSS_MOVING) {
        player.getsHitByBossUltimate(false);
    }
    if (boss->getState() == BOSS_ULTING) {
        bossUltimateFx.animate();
        bossUltimateFx.mRect = boss->getUltRect();
        boss->ult();
        checkCollisionWithBossUltimate();
    }
}

void checkCollisionWithBossUltimate() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Obstacle& X : obstacles[i]) {
            if (!X.isCrashed() && checkCollision(boss->getUltRect(), X.getRect())) {
                X.crash();
                X.setVelY(background.getVelY());
            }
        }
    }
    if (player.isVisible() && !player.isGotHitByBossUltimate() && checkCollision(boss->getUltRect(), player.getRect())) {
        state.updateLives(state.remainLives() - 1);
        player.getsHitByBossUltimate(true);
        std::clog << "hit by boss ultimate\n";
    }
}

void useAbilities() {
    for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
        for (Ability& A : abils[tier]) {
            switch (tier) {
                case 0:
                    switch (A.id) {
                        case 0: // Invisibility
                            if (A.isActive) {
                                player.setVisible(false);
                                SDL_SetTextureAlphaMod(carTexture, 150);
                            }
                            else {
                                player.setVisible(true);
                                SDL_SetTextureAlphaMod(carTexture, 255);
                            }
                            break;

                        case 1: // x10 Bullets
                            if (A.isActive)
                                state.updateBullets(state.currentBullets() + 10);
                            A.isActive = 0;
                            break;

                        case 2: // Extra Life
                            if (A.isActive)
                                state.updateLives(state.remainLives() + 1);
                            A.isActive = 0;
                            break;
                    }
                    break;

                case 1:
                    switch (A.id) {
                        case 0: // Coin Magnet
                            if (A.isActive)
                                state.updateMagnet(true);
                            else
                                state.updateMagnet(false);
                            break;

                        case 1: // Double Coins
                            COIN_MULTIPLIER = (A.isActive ? 2 : 1);
                            break;

                        case 2: // x5 Bullets
                            if (A.isActive)
                                state.updateBullets(state.currentBullets() + 5);
                            A.isActive = 0;
                            break;

                        case 3: // Shrink
                            if (A.isActive)
                                player.setScale(0.75);
                            else
                                player.setScale(1);
                            break;
                    }
                    break;

                case 2:
                    switch (A.id) {
                        case 0: // Speed Boost
                            if (A.isActive && !state.speedBoostIsEnabled()) {
                                background.setVelY(background.getVelY() + 5 * 60);
                                state.updateSpeedBoost(true);
                            }
                            else if (!A.isActive && state.speedBoostIsEnabled()) {
                                background.setVelY(background.getVelY() - 5 * 60);
                                state.updateSpeedBoost(false);
                            }
                            break;
                    }
                    break;
            }
        }
    }
}
