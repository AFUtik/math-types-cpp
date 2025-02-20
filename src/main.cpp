#include <emmintrin.h> 
#include <immintrin.h>
#include <iostream>

#include "perfomance/timer.hpp"
#include "math_types.hpp"

#include "sse_calc.hpp"


void multiply_matrix_scalar_n(double* matrix, double scalar, size_t size) {
    for (int i = 0; i < 4; i++) {  // Обрабатываем блоками по 4 элемента
        for (int j = 0; j < 4; j++) {  // Обрабатываем блоками по 4 элемента
            matrix[i*4+j] += scalar;  // Записываем обратно
        }
    }
}

void multiply_matrix_scalar_sse(double* matrix, double scalar, size_t size) {
    __m128d scalar_vec = _mm_set1_pd(scalar);  // Загружаем скаляр в SSE-регистр
    size_t i = 0;
    for (; i + 4 <= size; i += 4) {  // Обрабатываем блоками по 4 элемента
        __m128d mat_vec = _mm_loadu_pd(&matrix[i]);  // Загружаем 4 float
        __m128d result =  _mm_add_pd(mat_vec, scalar_vec);  // Умножаем
        _mm_storeu_pd(&matrix[i], result);  // Записываем обратно
    }
}

int main() {
    PerfomanceTimer timer;
    //const int N = 4, M = 4;  // Размерность матрицы
    //double matrix[N * M] = {
    //    1, 2, 3, 4,
    //    5, 6, 7, 8,
    //    9, 10, 11, 12,
    //    13, 14, 15, 16
    //};

    //double scalar = 1.2f;
    //timer.startTimer();
    //for(int i = 0; i < 2; i++) multiply_matrix_scalar_n(matrix, scalar, N * M);
    //std::cout << "normal mul: " << timer.getTime() << std::endl;

    //double sse_matrix[N * M] = {
    //    1, 2, 3, 4,
    //    5, 6, 7, 8,
    //    9, 10, 11, 12,
    //    13, 14, 15, 16
    //};    
    //timer.startTimer();
    //for(int i = 0; i < 2; i++) multiply_matrix_scalar_sse(sse_matrix, scalar, N * M);
    ////std::cout << "sse mul: " << timer.getTime() << std::endl;

    //timer.startTimer();
    //Vector3f vector(3.0f, 2.0f, 5.0f);
    //for(int i = 0; i < 0; i++) {
    //}
    //std::cout << "iterator: ";
    //timer.printTime();

    //timer.startTimer();
    //Vector3f vector2(3.0f, 2.0f, 5.0f);
    //for(int i = 0; i < 0; i++) {
    //}
    //std::cout << "index: ";
    //timer.printTime();


    Vector3f vector2(3.0f, 2.0f, 5.0f);

    return 0;
}
