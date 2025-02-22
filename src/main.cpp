#include "utils/timer.hpp"

#include "../include/mtp.hpp"

#include <iostream>

#define ITERATIONS (1<<20)


int main() {
    PerfomanceTimer timer;

    mtp::vector2<float> vec_vec;

    timer.startTimer();

    mtp::vector2<float> mtp_vec;
    for(int i =0; i < ITERATIONS; i++) mtp_vec = vec_vec + vec_vec;

    std::cout << "library speed: ";
    timer.printTime();
    
    return 0;
}
