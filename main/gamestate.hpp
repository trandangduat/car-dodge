#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class GameState {
private:
    int score, lives, stage;

public:
    GameState();
    void reset();
    void updateScore (int _score);
    void updateLives (int _lives);
    void updateStage (int _stage);
    int currentScore();
    int remainLives();
    int currentStage();
};

#endif // GAMESTATE_HPP
