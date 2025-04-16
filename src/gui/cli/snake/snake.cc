/** @file
 * @brief Файл, запускающий основной игровой цикл Змейки
 */
#include "snake.h"

namespace s21 {
/**
 * @brief Конструктор класса SnakeView
 * @details Инициализирует представление игры
 * @param snake_controller Контроллер игры, передающий сигналы
 * между классами SnakeView и SnakeModel
 */
SnakeView::SnakeView(SnakeController *snake_controller)
    : snake_controller_{snake_controller} {}

SnakeView::~SnakeView() {}

/**
 * @brief Управление игровым циклом
 * @details Инициализирует начальное состояние игры и входит в цикл обработки
 * ввода и обновления состояния игры. Внутри цикла обрабатывается ввод
 * пользователя, обновляется состояние игры, отрисовываются основные игровые
 * элементы (поле, фигуры и статистика) и проверяется условие паузы или
 * завершения игры
 */
void SnakeView::snakeCycle() {
  timeout(50);
  clearScreen();
  SnakeModel::SnakeInfo_t *game_state =
      snake_controller_->getModel()->getSnakeInfo_t();
  game_state->set_time = setTime();
  while (game_state->game_status != kGameOver &&
         game_state->game_status != kWin) {
    snake_controller_->getUserInput();
    GameInfo_t stats = snake_controller_->updateCurrentState();
    if (stats.pause) {
      printStatusScreen(kPause);
      if (game_state->action == Terminate) {
        game_state->game_status = kGameOver;
      }
      continue;
    }
    clearScreen();
    drawBordersAndStats(stats.level, stats.score, stats.high_score);
    snake_controller_->getModel()->updateField(stats);
    drawObjects(&stats);
    game_state->game_info = stats;
    snake_controller_->getModel()->snakeMechanics(game_state->game_status);
  }
  printStatusScreen(game_state->game_status);
}
} // namespace s21