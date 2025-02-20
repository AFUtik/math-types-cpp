#include "timer.hpp"
#include <iostream>

PerfomanceTimer::PerfomanceTimer() {
    startTimer();
}
void PerfomanceTimer::startTimer() {
    start = std::chrono::high_resolution_clock::now();
}

void PerfomanceTimer::printTime() {
    std::cout << getTime() << std::endl;
}

double PerfomanceTimer::getTime() {
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}