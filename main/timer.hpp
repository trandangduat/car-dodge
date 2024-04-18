#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
private:
    int startTime;
    int lastPause;
    int lastUnpause;
    int freezeTime;
    bool isPausing;
public:
    Timer();
    void start();
    void pause();
    void unpause();
    void reset();
    bool isPaused();
    int elapsedTime(); // return in miliseconds
};

#endif // TIMER_HPP
