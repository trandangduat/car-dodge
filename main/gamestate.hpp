#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "timer.hpp"
#include<vector>

#define GSTATE_STARTMENU 0
#define GSTATE_PLAYING   1
#define GSTATE_PAUSING   2
#define GSTATE_GAMEOVER  3
#define GSTATE_TRANSITION 4 // countdown screen ater starting game / unpausing
#define TRANSITION_COUNTDOWN 3

#define DIFFICULITY_EASY 0
#define DIFFICULITY_MEDIUM 1
#define DIFFICULITY_HARD 2
#define DIFFICULITY_ASIAN 3
#define NUMBER_OF_DIFFICULTIES 4

class GameState {
private:
    int gstate;
    int score, lives, coins, stage, bullets;
    int max_lives;
    std::vector<long long> highScores;
    int difficulty;
    bool bossSpawned;
    bool magnetEnabled;
    bool speedBoostEnabled;

public:
    Timer* transitionTimer = nullptr;
    GameState();
    void reset();
    void updateDifficulty (int _level);
    void updateState (int _state);
    void updateScore (long long _score);
    void updateLives (int _lives);
    void updateCoins (int _coins);
    void updateStage (int _stage);
    void updateMagnet (bool state);
    void updateBullets (int _bullets);
    void updateSpeedBoost (bool state);
    bool magnetIsEnabled();
    bool speedBoostIsEnabled();
    int currentState();
    long long currentScore();
    int currentLives();
    int maxLives();
    int currentStage();
    int currentCoins();
    int currentBullets();
    int currentDifficulty();
    bool hasBoss();
    long long currentHighscore();
};

#endif // GAMESTATE_HPP
