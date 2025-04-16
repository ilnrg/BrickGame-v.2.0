/** @file
 * @brief Заголовочный файл, определяющий параметры для старта консольной версии
 * игры
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_GUI_CLI_BRICK_GAME_H_
#define CPP3_BRICK_GAME_V2_0_1_GUI_CLI_BRICK_GAME_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "common/common_cli.h"
#include "tetris/tetris.h"
#ifdef __cplusplus
}
#endif
#include "../../brick_game/snake/snake_controller.h"
#include "../../brick_game/snake/snake_model.h"
#include "snake/snake.h"

#define START 0
#define STOP 1
#define TETRIS 2
#define SNAKE 3

// ENTRY POINT
void brickGame();
void printStartScreen();
void drawDPad();

// GAME SELECTION
int getStatus();

#endif // CPP3_BRICK_GAME_V2_0_1_GUI_CLI_BRICK_GAME_H_