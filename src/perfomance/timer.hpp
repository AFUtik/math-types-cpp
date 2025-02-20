#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class PerfomanceTimer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
public:
    PerfomanceTimer();

    void startTimer();
    void printTime();

    double getTime();
};

#endif