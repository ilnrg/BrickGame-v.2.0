/** @file
 * @brief Заголовочный файл, определяющий общие для двух игр функции логики
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_COMMON_COMMON_BACK_H_
#define CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_COMMON_COMMON_BACK_H_

#include "common_specification.h"

#define SECOND 1000

// TIMER
long long setTime();

// SPEED UPDATER
void setSpeed(int *level, int *speed);

#endif // CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_COMMON_COMMON_BACK_H_