/** @file
 * @brief Заголовочный файл, определяющий функции контроллера игры Змейка
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_SNAKE_SNAKE_CONTROLLER_H_
#define CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_SNAKE_SNAKE_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "../common/common_back.h"
#ifdef __cplusplus
}
#endif
#include "snake_model.h"

namespace s21 {
/** @class SnakeController
 * @brief Контроллер игры, передающий сигналы между классами SnakeView и
 * SnakeModel
 * @param snake_model Указатель на модель игры
 */
class SnakeController {
public:
  // CONSTRUCTOR & DESTRUCTOR
  SnakeController(SnakeModel *snake_model);
  ~SnakeController();

  // USER'S COMMAND HANDLERS
  void getUserInput();
  void setUserAction(int key, UserAction_t *action, bool *hold);
  void userInput(UserAction_t action, bool hold);

  // GETTERS
  SnakeModel *getModel() const;
  GameInfo_t updateCurrentState();

private:
  /** @brief Указатель на модель игры */
  SnakeModel *snake_model_;
};

} // namespace s21

#endif // CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_SNAKE_SNAKE_CONTROLLER_H_