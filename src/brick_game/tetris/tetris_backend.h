/** @file
 * @brief Заголовочный файл, определяющий функции бэкенда игры Тетрис
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_TETRIS_TETRIS_BACKEND_H_
#define CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_TETRIS_TETRIS_BACKEND_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common_back.h"

#define FIGURES_COUNT 7
#define TETRIS_MAX_SCORE 10000

/**
 * @brief Структура, хранящая информацию о текущей фигуре
 */
typedef struct {
  int x;
  int y;
  int width;
  int height;
  int f[4][4];
} Figure_t;

/**
 * @brief Основная игровая структура
 */
typedef struct {
  UserAction_t action;
  GameInfo_t game_info;
  GameStatus_t game_status;
  Figure_t figure;
  Figure_t next_figure;
  int figures[FIGURES_COUNT];
  int curr_figure;
  long long set_time;
} TetrisInfo_t;

// GAME ELEMENTS INITIALIZATION FUNCS
TetrisInfo_t *getTetrisInfo_t();
int createInfo_t(TetrisInfo_t *game_state);
int createMatrix(int height, int width, int ***matrix);

// GAME ELEMENTS REMOVAL FUNCS
void removeGameInfo_t();
void removeMatrix(int **matrix, int height);

// FIGURES INITIALIZATION FUNCS
void figureOrdering();
void corrSpawn(int *figures, int count);
void spawnFigure(TetrisInfo_t *game_state);
void initNextFigure(GameInfo_t *stats);
void initFigure(Figure_t *figure, int type);
void copyFigure(int src[4][4], int dst[4][4], int height, int width);
void lineFigure(Figure_t *figure);
void squareFigure(Figure_t *figure);
void zFigure(Figure_t *figure);
void sFigure(Figure_t *figure);
void tFigure(Figure_t *figure);
void lFigure(Figure_t *figure);
void jFigure(Figure_t *figure);

// GAME STATE GETTER
GameInfo_t updateCurrentState();

// GAME LOGIC
void tetrisMechanics(TetrisInfo_t *game_state);
void continueOrNot(UserAction_t state, TetrisInfo_t *game_state);
void updateField(GameInfo_t *stats, Figure_t *figure, int cell_type);
bool checkCollision(GameInfo_t *stats, Figure_t *figure, int offset_x,
                    int offset_y);

// USER'S COMMAND HANDLERS
void getUserInput();
void setUserAction(int key, UserAction_t *state);
void userInput(UserAction_t action, bool hold);
void rotateFigure(TetrisInfo_t *game_state);
void moveDown(TetrisInfo_t *game_state);
void moveLeft(TetrisInfo_t *game_state);
void moveRight(TetrisInfo_t *game_state);

// SCORE & LEVEL UPDATE FUNC
void removeLine(TetrisInfo_t *game_state);
bool checkLine(GameInfo_t *stats, int y);
void updateScore(GameInfo_t *stats, int how_much);

// HIGH SCORE SETTER & GETTER
int getHighScore();
void setHighScore(int high_score);

#endif // CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_TETRIS_TETRIS_BACKEND_H_