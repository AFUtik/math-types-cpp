# Math Types Library
###### The Library only works with standard C++ 17
###### __README has RU translation.__

## Russian README
Данный проект представляет собой легкую библиотеку, содержащую математические функции и собственные математические примитивы для простых вычислений.

### Установка 
Библиотека содержит только заголовочные файлы. Чтобы внедрить её, вам необходимо скачать папку проекта и перенести include/mtp в вашу директорию.
Для оптимизации вычислений рекомендуется использовать флаг -O3 для компиляторов GCC/Clang или /O2 для MSVC.

### Использование
На данный момент доступны следующие примитивы: vector, matrix. Все тонкости работы с векторами описаны далее. Также всё это работает с другими контейнерами библиотеки.

Важно отметить, что при сравнении двух векторов или матриц возвращается битовая маска (bitmask), где 1 означает true, а 0 — false.
Если маска отрицательная, то bitmask = 0. Если маска полностью положительная, то возвращается число 0b1111 (количество единиц зависит от размера вектора).
Следует учитывать, что битовая маска не может быть больше 32 бит из-за типа integer.
 
По умолчанию epsilon = 1e-6. Если вам нужно сравнить два вектора с плавающей точкой и задать собственную точность,
вы можете указать количество знаков после запятой последним параметром шаблона.
Пример: vector<float, 4, 2>.
 
Вектор имеет публичные поля x, y, z, w, а также r, g, b, a для представления цвета. Если вектор имеет размерность 2, то два последних поля (z и w) будут ссылатся на первый элемент, то есть x.
#### vector example
```cpp
#include "mtp/mtp.hpp"

int main()
{
  mtp::vector4f vec1(1.0f); // Заполняет вектор единицами.
  mtp::vector4f vec2(2.0f, 3.0f, 1.0f, 5.0f);

  mtp::vector4f result = vec1 + vec2; // Суммирует два вектора
  vec1 += vec2 // Суммирует значение в vec1

  /* Статические методы */
  mtp::vector4f normalized_vec = mtp::st_vector::normalize(vec1); // Нормализирует вектор
  mtp::vector4<int> = mtp::st_vector::cast<int>(vec1); // Переводит из типа float в int

  /* Методы экземпляра */
  vec1.normalize(); // Нормализует вектор, но результат сохраняется в самом векторе //

  return 0;
}
```
#
#### matrix example
```cpp
#include "mtp/mtp.hpp"

int main()
{
  mtp::matrix4f mat1(1.0f); // Заполняет матрицу единицами.
  mtp::matrix4f mat2(2.0f);

  mtp::matrix4f result = mat1 + mat2; // Суммирует две матрицы
  mat1 += mat2 // Суммирует матрицу в mat1

  return 0;
}
```
