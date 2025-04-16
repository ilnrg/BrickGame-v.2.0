/** @file
 * @brief Заголовочный файл, определяющий параметры для старта Змейки
 */

#ifndef CPP3_BRICK_GAME_V2_0_1_GUI_CLI_SNAKE_SNAKE_H_
#define CPP3_BRICK_GAME_V2_0_1_GUI_CLI_SNAKE_SNAKE_H_

#include <iostream>
#ifdef __cplusplus
extern "C" {
#endif
#include "../common/common_cli.h"
#ifdef __cplusplus
}
#endif
#include "../../../brick_game/snake/snake_controller.h"

namespace s21 {
class SnakeController;

/** @class SnakeView
 * @brief Класс представления игры
 * @param snake_controller Контроллер игры, передающий сигналы
 * между классами SnakeView и SnakeModel
 */
class SnakeView {
public:
  // CONSTRUCTOR & DESTRUCTOR
  SnakeView(SnakeController *snake_controller);
  ~SnakeView();

  // MAIN GAME CYCLE
  void snakeCycle();

private:
  SnakeController *snake_controller_;
};

} // namespace s21

#endif // CPP3_BRICK_GAME_V2_0_1_GUI_CLI_SNAKE_SNAKE_H_