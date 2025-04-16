/** @file
 * @brief Заголовочный файл, определяющий параметры игры
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_COMMON_COMMON_SPECIFICATION_H_
#define CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_COMMON_COMMON_SPECIFICATION_H_

#include <ncurses.h>
#undef border
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 10
#define HEIGHT 20

#define START 0
#define STOP 1

#define ENTER_KEY 10
#define ESCAPE 27
#define SPACE_KEY 32
#define START_SPEED 1000

#define EMPTY_CELL 0
#define MOVING_CELL 1
#define STATIC_CELL 2

/**
 * @brief Структура, хранящая информацию о пользовательских командах
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * @brief Структура, хранящая информацию о текущем статусе игры
 */
typedef enum { kStart, kGameOver, kPause, kExit, kError, kWin } GameStatus_t;

/**
 * @brief Структура, хранящая информацию о состоянии игры и основных игровых
 * элементах
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#endif // CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_COMMON_COMMON_SPECIFICATION_H_