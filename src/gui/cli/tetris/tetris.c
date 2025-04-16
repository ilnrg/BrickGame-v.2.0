/** @file
 * @brief Файл, запускающий основной игровой цикл Тетриса
 */
#include "tetris.h"

/**
 * @brief Управление игровым циклом
 * @details Инициализирует начальное состояние игры и входит в цикл обработки
 * ввода и обновления состояния игры. Если игра не может быть инициализирована,
 * выводит сообщение об ошибке. Внутри цикла обрабатывается ввод пользователя,
 * обновляется состояние игры, отрисовываются основные игровые элементы (поле,
 * фигуры и статистика) и проверяется условие паузы или завершения игры. После
 * завершения цикла, освобождает ресурсы, связанные с состоянием игры
 */
void tetrisCycle() {
  timeout(50);
  TetrisInfo_t *game_state = getTetrisInfo_t();
  if (game_state == NULL) {
    printStatusScreen(kError);
  } else {
    clearScreen();
    game_state->set_time = setTime();
    while (game_state->game_status != kGameOver &&
           game_state->game_status != kWin) {
      getUserInput();
      GameInfo_t stats = updateCurrentState();
      if (stats.pause) {
        printStatusScreen(kPause);
        if (game_state->action == Terminate) {
          game_state->game_status = kGameOver;
        }
        continue;
      }
      Figure_t *figure = &game_state->figure;
      clearScreen();
      drawBordersAndStats(stats.level, stats.score, stats.high_score);
      updateField(&stats, figure, MOVING_CELL);
      drawObjects(&stats);
      updateField(&stats, figure, EMPTY_CELL);
      drawNextFigure(&game_state->next_figure);
      game_state->game_info = stats;
      tetrisMechanics(game_state);
    }
    printStatusScreen(game_state->game_status);
  }
  removeGameInfo_t();
}

/**
 * @brief Отображает предпросмотр следующей фигуры
 * @details Эта функция отображает предпросмотр следующей фигуры в специальном
 * окне на экране. Фигура состоит из символов '#'
 * @param figure Указатель на структуру Figure_t
 */
void drawNextFigure(Figure_t *figure) {
  printRectangle(13, 20, HEIGHT + 3, HEIGHT + 12);
  mvprintw(14, HEIGHT + 6, "NEXT");
  for (int y = 0; y < figure->height; y++) {
    for (int x = 0; x < figure->width; x++) {
      if (figure->f[y][x] == 1) {
        mvaddch(16 + y, HEIGHT + 6 + x, '#');
      } else {
        mvaddch(16 + y, HEIGHT + 6 + x, ' ');
      }
    }
  }
}