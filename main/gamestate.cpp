#include "gamestate.hpp"
#include "params.hpp"

GameState::GameState() {
    this->gstate = GSTATE_STARTMENU;
    reset();
}

void GameState::reset() {
    this->score = 0;
    this->lives = NUMBER_OF_LIVES;
    this->coins = 0;
    this->stage = 1;
    this->bullets = 0;
    this->magnetEnabled = 0;
    this->speedBoostEnabled = 0;
}

void GameState::updateState (int _state) {
    this->gstate = _state;
}

void GameState::updateScore (int _score) {
    if (this->gstate == GSTATE_GAMEOVER) return;
    this->score = _score;
}

void GameState::updateLives (int _lives) {
    if (this->gstate == GSTATE_GAMEOVER) return;
    this->lives = _lives;
    if (this->lives > NUMBER_OF_LIVES) {
        this->lives = NUMBER_OF_LIVES;
    }
    if (this->lives == 0) {
        this->updateState(GSTATE_GAMEOVER);
    }
}

void GameState::updateStage (int _stage) {
    if (this->gstate == GSTATE_GAMEOVER) return;
    this->stage = _stage;
}

void GameState::updateCoins (int _coins) {
    if (this->gstate == GSTATE_GAMEOVER) return;
    this->coins = _coins;
}

void GameState::updateBullets (int _bullets) {
    this->bullets = _bullets;
}

void GameState::updateMagnet (bool state) {
    this->magnetEnabled = state;
}

void GameState::updateSpeedBoost (bool state) {
    this->speedBoostEnabled = state;
}

bool GameState::magnetIsEnabled() { return this->magnetEnabled; }
bool GameState::speedBoostIsEnabled() { return this->speedBoostEnabled; }

int GameState::currentState() { return this->gstate; }
int GameState::currentScore() { return this->score; }
int GameState::currentLives() { return this->lives; }
int GameState::currentCoins() { return this->coins; }
int GameState::currentStage() { return this->stage; }
int GameState::currentBullets() { return this->bullets; }
