#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class GameState {
private:
    bool gameOver;
    bool paused;
    int score, lives, coins, stage, bullets;
    bool magnetEnabled;
    bool speedBoostEnabled;

public:
    GameState();
    void reset();
    void pause();
    void unpause();
    void endGame();
    void updateScore (int _score);
    void updateLives (int _lives);
    void updateCoins (int _coins);
    void updateStage (int _stage);
    void updateMagnet (bool state);
    void updateBullets (int _bullets);
    void updateSpeedBoost (bool state);
    bool isPausing();
    bool isGameOver();
    bool magnetIsEnabled();
    bool speedBoostIsEnabled();
    int currentScore();
    int remainLives();
    int currentStage();
    int currentCoins();
    int currentBullets();
};

#endif // GAMESTATE_HPP
