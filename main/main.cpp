#include "logic.hpp"
#include "timer.hpp"
#include "assets.hpp"
#include "background.hpp"
#include "obstacle.hpp"
#include "coin.hpp"
#include "hud.hpp"
#include "gamestate.hpp"
#include "abilities.hpp"
#include "button.hpp"
#include "bullet.hpp"
#include "vfx.hpp"
#include "boss.hpp"
#include "car.hpp"

SDL_Rect column[NUMBER_OF_COLUMNS];
int colVelocity[NUMBER_OF_COLUMNS];

GameWindow win;
GameState state;
Timer frameTimer, // timer to get time per frame
      veloTimer, // timer to change velocity
      storeTimer; // timer to reset store's items

HUD hud               (&win, &state);
Background background (&win, &backgroundTextures);
Car player            (&win);
VFX speedBoostEffect,
    bossUltimateFx,
    logoTitle;

Boss* boss = new Boss(&win);
Button* playButton = new Button(&win, {SCREEN_WIDTH / 2 - 150 / 2, 300, 150, 50});
Button* homeButton = new Button(&win, {SCREEN_WIDTH / 2 - 150 / 2, 420, 150, 50});
Button* LeftArrowButton  = new Button(&win, {SCREEN_WIDTH / 2 - 90 - 30, 390, 30, 30});
Button* RightArrowButton = new Button(&win, {SCREEN_WIDTH / 2 + 90     , 390, 30, 30});

std::deque<Obstacle>  obstacles[NUMBER_OF_COLUMNS];
std::deque<Coin>      coins[NUMBER_OF_COLUMNS];
std::deque<Bullet>    firedBullets;
std::vector<Button>   storeOption;
int storeItemsId[NUMBER_OF_ABILITY_TIER];
std::deque<std::pair<int, int>> abilitiesToActive;

void resetGame();
bool handleEvent (SDL_Event e);
void closeGame();

void render();
void update();

void generateColumnRanges();
void updateBgVelocity();
SDL_Point getNearestObstacle (float x, float y);

void renderObstacles();
void updateObstacles();
void checkCollisionsWithObstacles();
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
    bossUltimateFx      = VFX(&win, 0, 0, 0, 0, bossLaser, 20, 96, 65);
    logoTitle           = VFX(&win, 0, 100, 420, 130, gameTitleSprite, 420, 130, 100);
    logoTitle.mRect.x = SCREEN_WIDTH / 2 - logoTitle.mRect.w / 2;
    boss->updateTexture(bossSprite, bossNearUltingSprite, 48, 41);
    playButton->updateTexture(playButtonSprite, 150, 50);
    homeButton->updateTexture(homeButtonSprite, 150, 50);
    LeftArrowButton->updateTexture(arrowHUD, 25, 25);
    RightArrowButton->updateTexture(arrowHUD, 25, 25);

    // init store
    for (int i = 0; i < NUMBER_OF_ABILITY_TIER; i++) {
        storeItemsId[i] = rand() % (int) abils[i].size();
    }
    int x = 100;
    int y = 100;
    for (int i = 0; i < 3; i++) {
        SDL_Rect box = {x, y, SCREEN_WIDTH - x * 2, 120};
        Button newButton (&win, box);
        storeOption.push_back(newButton);
        y += box.h;
    }

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) if (!handleEvent(e)) quit = true;

        switch (state.currentState()) {
            case GSTATE_STARTMENU:
                break;

            case GSTATE_PLAYING:
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
                break;

            case GSTATE_PAUSING:
                for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
                    for (Ability& A : abils[tier])
                        if (A.isActive) A.timer->pause();
                }
                break;

            case GSTATE_GAMEOVER:
                break;
        }

        render();
        update();
    }

    return 0;
}

void resetGame() {
    state.reset();
    /////////
    background.reset(state.currentStage());
    player.reset();
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        while (!obstacles[i].empty()) {
            obstacles[i].pop_back();
        }
        while (!coins[i].empty()) {
            coins[i].pop_back();
        }
    }
    abilitiesToActive.clear();
    for (int tier = 0; tier < NUMBER_OF_ABILITY_TIER; tier++) {
        for (Ability& A : abils[tier]) {
            A.timer->reset();
            A.isActive = 0;
        }
    }
    for (Button& B : storeOption) {
        B.reset();
    }
    firedBullets.clear();
    boss->reset();
    veloTimer.start();
    storeTimer.start();
}

bool handleEvent (SDL_Event e) {
    if (e.type == SDL_QUIT) return false;
    switch (state.currentState()) {
        case GSTATE_STARTMENU: {
            switch (e.type) {
                case SDL_MOUSEMOTION: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    playButton->isOnHoverByPoint(x, y);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if (playButton->isOnHoverByPoint(x, y)) {
                        Mix_PlayChannel(-1, clickSfx, 0);
                        resetGame();
                        state.updateState(GSTATE_TRANSITION);
                        state.transitionTimer->start();
                    }
                    if (LeftArrowButton->isOnHoverByPoint(x, y)) {
                        state.updateDifficulty((state.currentDifficulty() - 1 + NUMBER_OF_DIFFICULTIES) % NUMBER_OF_DIFFICULTIES);
                        LeftArrowButton->reset();
                    }
                    if (RightArrowButton->isOnHoverByPoint(x, y)) {
                        state.updateDifficulty((state.currentDifficulty() + 1) % NUMBER_OF_DIFFICULTIES);
                        RightArrowButton->reset();
                    }
                }
            }
            break;
        }
        case GSTATE_PLAYING: {
            switch (e.type) {
                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_SPACE:
                            state.updateState(GSTATE_PAUSING);
                            break;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT: {
                            if (state.currentBullets() > 0) {
                                Mix_PlayChannel(-1, shootSfx, 0);
                                generateBullet();
                                state.updateBullets(state.currentBullets() - 1);
                            }
                        }
                    }
                    break;
                }
            }
            break;
        }
        case GSTATE_PAUSING: {
            switch (e.type) {
                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_SPACE:
                            state.updateState(GSTATE_PLAYING);
                            break;
                    }
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    for (Button& B : storeOption) {
                        B.isOnHoverByPoint(x, y);
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT: {
                            int tier = 0;
                            for (Button& B : storeOption) {
                                if (!B.isDisabled()
                                    && state.currentCoins() >= abils[tier][storeItemsId[tier]].coins
                                    && B.isOnHoverByPoint(mouseX, mouseY)
                                ) {
                                    B.click();
                                    Mix_PlayChannel(-1, cashoutSfx, 0);
                                    state.updateCoins(state.currentCoins() - abils[tier][storeItemsId[tier]].coins);
                                    abilitiesToActive.push_back({tier, storeItemsId[tier]});
                                }
                                tier++;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case GSTATE_GAMEOVER: {
            switch (e.type) {
                case SDL_MOUSEMOTION: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    homeButton->isOnHoverByPoint(x, y);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if (homeButton->isOnHoverByPoint(x, y)) {
                        Mix_PlayChannel(-1, clickSfx, 0);
                        state.updateState(GSTATE_STARTMENU);
                        resetGame();
                    }
                }
            }
            break;
        }
    }
    return true;
}

void closeGame() {
    freeMedia();
    TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

void render() {
    //Render
    win.clearRender();
    switch (state.currentState()) {
        case GSTATE_STARTMENU: {
            background.render();
            logoTitle.render(0, SDL_FLIP_NONE);
            playButton->render();
            // Difficulty Switch
            std::string difType;
            switch (state.currentDifficulty()) {
                case DIFFICULITY_EASY: difType = "EASY"; break;
                case DIFFICULITY_MEDIUM: difType = "MEDIUM"; break;
                case DIFFICULITY_HARD: difType = "HARD"; break;
                case DIFFICULITY_ASIAN: difType = "ASIAN"; break;
            }
            hud.drawFadeRectangle({0, 375, SCREEN_WIDTH, 60}, 75);
            hud.drawTTFText(win.KarenFat, difType, 30, 0, 380, {255, 255, 255}, HUD_FLOAT_CENTER);
            LeftArrowButton->render(SDL_FLIP_HORIZONTAL);
            RightArrowButton->render();
            hud.drawTTFText(win.AvenuePixel, std::to_string(state.currentHighscore()), 30, 0, 400, {255, 255, 255}, HUD_FLOAT_CENTER);
            break;
        }
        default: {
            background.render();
            renderCoins();
            renderObstacles();
            if (state.speedBoostIsEnabled()) {
                speedBoostEffect.render(0, SDL_FLIP_VERTICAL);
            }
            player.render(carTexture);
            renderBullets();
            if (state.hasBoss()) {
                renderAIBoss();
            }
            // LEFT HUD
            hud.drawHearts(heartSymbolTexture, 30, 30, 2.3f);
            // RIGHT HUD
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
            if (state.currentState() == GSTATE_PAUSING) {
                hud.renderPauseScreen();
                hud.renderStore(storeItemsId, storeOption, &storeTimer);
            }
            if (state.currentState() == GSTATE_GAMEOVER) {
                hud.renderGameOverScreen();
                homeButton->render();
            }
            if (state.currentState() == GSTATE_TRANSITION) {
                hud.renderTransitionScreen(TRANSITION_COUNTDOWN * 1000 - state.transitionTimer->elapsedTime());
            }
        }
    }
    win.presentRender();
}

void update() {
    //Update
    switch (state.currentState()) {
        case GSTATE_STARTMENU:
            background.update(frameTimer.elapsedTime() / 1000.f);
            logoTitle.animate();
            break;

        case GSTATE_PLAYING: {
            if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(bgMusic, -1);
            }
            background.update(frameTimer.elapsedTime() / 1000.f);
            updateBgVelocity();
            updateObstacles();
            updateCoins();
            updateBullets();
            if (state.hasBoss()) {
                updateAIBoss();
            }
            if (state.speedBoostIsEnabled()) {
                speedBoostEffect.animate();
                speedBoostEffect.mRect.w = player.getRect().w;
                speedBoostEffect.mRect.h = player.getRect().h * 0.6;
                speedBoostEffect.setPos(
                    player.getPosX() + player.getRect().w / 2 - speedBoostEffect.mRect.w / 2,
                    player.getPosY() + player.getRect().h - 10
                );
            }
            updateActiveAbilities();
            useAbilities();
            state.updateScore(state.currentScore() + int(background.getVelY() / 60));
            break;
        }

        case GSTATE_TRANSITION: {
            if (state.transitionTimer->elapsedTime() > TRANSITION_COUNTDOWN * 1000) {
                state.updateState(GSTATE_PLAYING);
                state.transitionTimer->reset();
            }
            break;
        }

        case GSTATE_PAUSING:
            break;
        case GSTATE_GAMEOVER:
            if (Mix_PlayingMusic() != 0) {
                Mix_FadeOutMusic(1000);
            }
            break;
    }

    if (state.currentState() == GSTATE_PLAYING || state.currentState() == GSTATE_PAUSING) {
        if (storeTimer.elapsedTime() >= STORE_DURATION * 1000) {
            for (int i = 0; i < NUMBER_OF_ABILITY_TIER; i++) {
                storeItemsId[i] = rand() % (int) abils[i].size();
                storeOption[i].reset();
            }
            storeTimer.start();
        }
    }

    frameTimer.start();
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
            if (O.getPosY() > player.getPosY()) continue;

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
        checkCollisionsWithObstacles();
    }
    checkCollisionsWithBullets();
    manageObstaclesMovement();
    generateObstacles();
}

void checkCollisionsWithObstacles() {
    for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
        for (Obstacle& X : obstacles[i]) {
            if (!X.isCrashed() && checkCollision(player.getRect(), X.getRect())) {
                Mix_PlayChannel(-1, gruntSfx, 0);
                Mix_PlayChannel(-1, explodeSfx, 0);
                X.crash();
                X.setVelY(background.getVelY());
                state.updateLives(state.currentLives() - 1);
                hud.startBlinkingHearts();
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
                    Mix_PlayChannel(-1, explodeSfx, 0);
                    X.crash();
                    X.setVelY(background.getVelY());
                    bulletHitsObstacle = true;
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
            if (rand() % 10 == 0)
                Mix_PlayChannel(-1, honkSfx, 0);
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
                Mix_PlayChannel(-1, coinCollectSfx, 0);
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
        SDL_Point nearestObstacle = getNearestObstacle(player.getRect().x, player.getRect().y);
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
                Mix_PlayChannel(-1, explodeSfx, 0);
                X.crash();
                X.setVelY(background.getVelY());
            }
        }
    }
    if (player.isVisible() && !player.isGotHitByBossUltimate() && checkCollision(boss->getUltRect(), player.getRect())) {
        Mix_PlayChannel(-1, gruntSfx, 0);
        state.updateLives(state.currentLives() - 1);
        hud.startBlinkingHearts();
        player.getsHitByBossUltimate(true);
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
                                state.updateLives(state.currentLives() + 1);
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
