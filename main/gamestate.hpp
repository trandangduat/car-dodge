#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "timer.hpp"

#define GSTATE_STARTMENU 1
#define GSTATE_PLAYING   2
#define GSTATE_PAUSING   3
#define GSTATE_GAMEOVER  4
#define GSTATE_TRANSITION 5 // countdown screen ater starting game / unpausing
#define TRANSITION_COUNTDOWN 3

class GameState {
private:
    int gstate;
    int score, lives, coins, stage, bullets;
    int highScore;
    bool magnetEnabled;
    bool speedBoostEnabled;

public:
    Timer* transitionTimer = nullptr;
    GameState();
    void reset();
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
    int currentStage();
    int currentCoins();
    int currentBullets();
    long long currentHighscore();
};

#endif // GAMESTATE_HPP
