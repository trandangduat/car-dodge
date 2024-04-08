#include "gamestate.hpp"
#include "params.hpp"

GameState::GameState() {
    reset();
}

void GameState::reset() {
    this->score = 0;
    this->lives = NUMBER_OF_LIVES;
    this->coins = 0;
    this->stage = 1;
    this->paused = 0;
    this->gameOver = 0;
    this->bullets = 0;
}

void GameState::pause() {
    this->paused = 1;
}

void GameState::unpause() {
    this->paused = 0;
}

void GameState::endGame() {
    this->gameOver = 1;
}

void GameState::updateScore (int _score) {
    if (gameOver) return;
    this->score = _score;
}

void GameState::updateLives (int _lives) {
    if (gameOver) return;
    this->lives = _lives;
    if (this->lives > NUMBER_OF_LIVES) {
        this->lives = NUMBER_OF_LIVES;
    }
    if (this->lives == 0) {
        this->endGame();
    }
}

void GameState::updateStage (int _stage) {
    if (gameOver) return;
    this->stage = _stage;
}

void GameState::updateCoins (int _coins) {
    if (gameOver) return;
    this->coins = _coins;
}

void GameState::updateBullets (int _bullets) {
    this->bullets = _bullets;
}

bool GameState::isPausing() { return this->paused; }
bool GameState::isGameOver() { return this->gameOver; }

int GameState::currentScore() { return this->score; }
int GameState::remainLives() { return this->lives; }
int GameState::currentCoins() { return this->coins; }
int GameState::currentStage() { return this->stage; }
int GameState::currentBullets() { return this->bullets; }
