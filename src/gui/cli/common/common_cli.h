/** @file
 * @brief Заголовочный файл, определяющий функции фронтенда (консольной версии)
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_GUI_CLI_COMMON_COMMON_CLI_H_
#define CPP3_BRICK_GAME_V2_0_1_GUI_CLI_COMMON_COMMON_CLI_H_

#include "../../../brick_game/common/common_specification.h"

// GAME ELEMENTS DRAWING FUNCS
void drawBordersAndStats(int level, int score, int high_score);
void printRectangle(int top_y, int bottom_y, int left_x, int right_x);
void drawObjects(GameInfo_t *stats);
void clearScreen();

// GAME STATE DRAWING FUNCS
void printStatusScreen(GameStatus_t game_status);
void printMallocError();
void printPause();
void printBye();
void printWin();
void printGameOver();

#endif // CPP3_BRICK_GAME_V2_0_1_GUI_CLI_COMMON_COMMON_CLI_H_