#include "perfomance/timer.hpp"

#include "math_types.hpp"
#include "glm/glm.hpp"

#include <iostream>

#define SIZE (1<<24)

int main() {
    PerfomanceTimer timer;

    alignas(16) float vec[4] = {2, 3, 4, 5};
    alignas(16) float vec2[4]= {2, 3, 4, 5};

    glm::vec4 glm_vec = {2.0f, 3.0f, 4.0f, 5.0f};
    glm::vec4 glm_vec2= {2.0f, 3.0f, 4.0f, 5.0f};

    Vector4f vec_vec (2.0f, 3.0f, 4.0f, 5.0f);
    Vector4f vec_vec2(2.0f, 3.0f, 4.0f, 5.0f);
    F4 f1 = sse::load(vec);
    F4 f2 = sse::load(vec2);
    
    float data[4];
    glm::vec4 glm_data;
    Vector4f sse_data;

    timer.startTimer();
    for(int i = 0; i < SIZE; i++) for(int j = 0; j < 4; j++) data[j] = vec[j] * vec2[j];
    std::cout << "COMMON OPERATIONS: ";
    timer.printTime();

    timer.startTimer();
    for(int i = 0; i < SIZE; i++) glm_data = glm_vec * glm_vec2;
    std::cout << "GLM: ";
    timer.printTime();

    F4 res;
    timer.startTimer();
    for(int i = 0; i < SIZE; i++) sse_data = vec_vec * vec_vec2;

    std::cout << "SSE: ";
    timer.printTime();


    return 0;
}
