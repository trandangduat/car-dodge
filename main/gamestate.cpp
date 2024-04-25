#include "gamestate.hpp"
#include "params.hpp"

#include <iostream>
#include <fstream>

#define HIGHSCORE_DATA_FILE "game_data/highscore.txt"

std::vector<long long> getHighScoreFromDataFile (std::string path) {
    std::vector<long long> scores(NUMBER_OF_DIFFICULTIES, 0LL);
    std::ifstream file(path);
    if (!file.is_open()) {
        std::clog << "Failed to open: " << path << '\n';
        return {0};
    }
    int i = 0;
    std::string s;
    while (file >> s) {
        for (const char& c : s) {
            scores[i] = scores[i] * 10 + int(c - '0');
        }
        i++;
    }
    file.close();
    return scores;
}

void updateHighScoreToDataFile (std::string path, const std::vector<long long> &highScores) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::clog << "Failed to open: " << path << '\n';
        return;
    }
    // Clear the file by setting the position indicator to the beginning
    file.seekp(0, std::ios::beg);
    for (int i = 0; i < NUMBER_OF_DIFFICULTIES; i++) {
        file << highScores[i] << '\n';
    }
    file.close();
}

GameState::GameState() {
    this->gstate = GSTATE_STARTMENU;
    this->transitionTimer = new Timer;
    this->difficulty = DIFFICULITY_EASY;
    reset();
}

void GameState::reset() {
    this->score = 0;
    this->max_lives = NUMBER_OF_LIVES;
    this->coins = 0;
    this->stage = 1;
    this->bullets = 0;
    this->bossSpawned = 1;
    this->magnetEnabled = 0;
    this->speedBoostEnabled = 0;
    BOSS_COOLDOWN = 5;
    this->highScores = getHighScoreFromDataFile(HIGHSCORE_DATA_FILE);
    updateDifficulty(this->difficulty);
}

void GameState::updateDifficulty (int _level) {
    this->difficulty = _level;
    switch (this->difficulty) {
        case DIFFICULITY_EASY: {
            this->bullets = 20;
            this->bossSpawned = 0;
            this->stage = 1;
            break;
        }
        case DIFFICULITY_MEDIUM: {
            this->bullets = 10;
            BOSS_COOLDOWN = 10;
            this->stage = 1;
            break;
        }
        case DIFFICULITY_HARD: {
            this->bullets = 5;
            this->max_lives = 3;
            this->stage = 5;
            BOSS_COOLDOWN = 5;
            break;
        }
        case DIFFICULITY_ASIAN:  {
            this->bullets = 0;
            this->max_lives = 2;
            this->stage = 10;
            BOSS_COOLDOWN = 3;
            break;
        }
    }
    this->lives = this->max_lives;
}

void GameState::updateState (int _state) {
    this->gstate = _state;
}

void GameState::updateScore (long long _score) {
    this->score = _score;
    if (this->gstate == GSTATE_GAMEOVER) {
        // Only update highscore when game is over
        if (this->highScores[this->difficulty] < _score) {
            this->highScores[this->difficulty] = _score;
            updateHighScoreToDataFile(HIGHSCORE_DATA_FILE, this->highScores);
        }
        return;
    }
}

void GameState::updateLives (int _lives) {
    if (this->gstate == GSTATE_GAMEOVER) return;
    this->lives = _lives;
    if (this->lives > this->max_lives) {
        this->lives = this->max_lives;
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
int GameState::maxLives() { return this->max_lives; }
int GameState::currentCoins() { return this->coins; }
int GameState::currentStage() { return this->stage; }
int GameState::currentBullets() { return this->bullets; }
int GameState::currentDifficulty() { return this->difficulty; }
bool GameState::hasBoss() { return this->bossSpawned; }
long long GameState::currentHighscore() { return this->highScores[this->difficulty]; }

