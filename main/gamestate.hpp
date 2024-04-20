#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#define GSTATE_STARTMENU 1
#define GSTATE_PLAYING   2
#define GSTATE_PAUSING   3
#define GSTATE_GAMEOVER  4

class GameState {
private:
    int gstate;
    int score, lives, coins, stage, bullets;
    bool magnetEnabled;
    bool speedBoostEnabled;

public:
    GameState();
    void reset();
    void updateState (int _state);
    void updateScore (int _score);
    void updateLives (int _lives);
    void updateCoins (int _coins);
    void updateStage (int _stage);
    void updateMagnet (bool state);
    void updateBullets (int _bullets);
    void updateSpeedBoost (bool state);
    bool magnetIsEnabled();
    bool speedBoostIsEnabled();
    int currentState();
    int currentScore();
    int currentLives();
    int currentStage();
    int currentCoins();
    int currentBullets();
};

#endif // GAMESTATE_HPP
