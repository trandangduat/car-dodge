#include "gamestate.hpp"
#include "params.hpp"

#include <iostream>
#include <fstream>

#define HIGHSCORE_DATA_FILE "game_data/highscore.txt"

long long getHighScoreFromDataFile (std::string path) {
    long long score = 0;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::clog << "Failed to open: " << path << '\n';
        return -1;
    }
    std::string s;
    file >> s;
    for (const char& c : s) {
        score = score * 10 + int(c - '0');
    }
    file.close();
    return score;
}

void updateHighScoreToDataFile (std::string path, long long score) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::clog << "Failed to open: " << path << '\n';
        return;
    }
    // Clear the file by setting the position indicator to the beginning
    file.seekp(0, std::ios::beg);
    file << score;
    file.close();
}

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
    this->highScore = getHighScoreFromDataFile(HIGHSCORE_DATA_FILE);
}

void GameState::updateState (int _state) {
    this->gstate = _state;
}

void GameState::updateScore (long long _score) {
    this->score = _score;
    if (this->gstate == GSTATE_GAMEOVER) {
        // Only update highscore when game is over
        if (this->highScore < _score) {
            this->highScore = _score;
            updateHighScoreToDataFile(HIGHSCORE_DATA_FILE, _score);
        }
        return;
    }
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
long long GameState::currentScore() { return this->score; }
int GameState::currentLives() { return this->lives; }
int GameState::currentCoins() { return this->coins; }
int GameState::currentStage() { return this->stage; }
int GameState::currentBullets() { return this->bullets; }
long long GameState::currentHighscore() { return this->highScore; }

