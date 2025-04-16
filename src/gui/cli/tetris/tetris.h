/** @file
 * @brief Заголовочный файл, определяющий параметры для старта Тетриса
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_GUI_CLI_TETRIS_TETRIS_H_
#define CPP3_BRICK_GAME_V2_0_1_GUI_CLI_TETRIS_TETRIS_H_

#include "../../../brick_game/tetris/tetris_backend.h"
#include "../common/common_cli.h"

// MAIN GAME CYCLE
void tetrisCycle();

// GAME ELEMENTS DRAWING FUNCS
void drawNextFigure(Figure_t *figure);

#endif // CPP3_BRICK_GAME_V2_0_1_GUI_CLI_TETRIS_TETRIS_H_