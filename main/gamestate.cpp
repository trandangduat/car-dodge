#include "gamestate.hpp"
#include "params.hpp"

GameState::GameState() {
    reset();
}

void GameState::reset() {
    this->score = 0;
    this->lives = NUMBER_OF_LIVES;
    this->stage = 1;
}

void GameState::updateScore (int _score) {
    this->score = _score;
}

void GameState::updateLives (int _lives) {
    this->lives = _lives;
}

void GameState::updateStage (int _stage) {
    this->stage = _stage;
}

int GameState::currentScore() {
    return this->score;
}

int GameState::remainLives() {
    return this->lives;
}

int GameState::currentStage() {
    return this->stage;
}
