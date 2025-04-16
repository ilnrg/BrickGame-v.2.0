/** @file
 * @brief Файл, содержащий реализацию общих функций
 */
#include "common_back.h"

/**
 * @brief Получает текущее время в миллисекундах
 * @details Функция получает текущее время с помощью функции
 * gettimeofday и возвращает его в миллисекундах
 * @return Текущее время в миллисекундах
 */
long long setTime() {
  struct timeval timer;
  gettimeofday(&timer, NULL);
  return (timer.tv_sec * SECOND + timer.tv_usec / SECOND);
}

/**
 * @brief Функция увеличения скорости
 * @details Функция изменяет скорость в зависимости от уровня. Если
 * уровень меньше 10, то скорость уменьшается на 10% от начальной для каждого
 * уровня, начиная с 2 уровня. Если уровень равен 10, то скорость равна 5% от
 * начальной.
 * @param level Указатель на переменную, хранящую текущий уровень
 * @param speed Указатель на переменную, хранящую текущую скорость
 */
void setSpeed(int *level, int *speed) {
  if (*level < 10) {
    *speed = START_SPEED * (1 - (*level - 1) * 0.1);
  } else {
    *level = 10;
    *speed = START_SPEED * 0.05;
  }
}