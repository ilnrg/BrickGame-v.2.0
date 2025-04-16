/** @file
 * @brief Файл, содержащий функции для отрисовки консольного интерфейса
 */
#include "common_cli.h"

/**
 * @brief Рисует элементы интерфейса
 * @details Функция рисует границы игрового поля, полей уровня, текущих и
 * макисмальных очков и размещает в них информацию, получаемую из логики игры в
 * реальном времени
 * @param level Текущий уровень
 * @param score Текущий счет
 * @param high_score Максимальный счет, набранный игроком
 */
void drawBordersAndStats(int level, int score, int high_score) {
  printRectangle(0, HEIGHT + 1, 0, WIDTH * 2 + 1);
  printRectangle(0, HEIGHT + 1, WIDTH * 2 + 2, HEIGHT + 13);

  printRectangle(1, 4, WIDTH * 2 + 3, HEIGHT + 12);
  printRectangle(5, 8, WIDTH * 2 + 3, HEIGHT + 12);
  printRectangle(9, 12, WIDTH * 2 + 3, HEIGHT + 12);

  mvprintw(2, WIDTH * 2 + 5, "LEVEL");
  mvprintw(3, WIDTH * 2 + 7, "%d", level);

  mvprintw(6, WIDTH * 2 + 5, "SCORE");
  if (score < 100) {
    mvprintw(7, WIDTH * 2 + 7, "%d", score);
  } else if (score >= 100) {
    mvprintw(7, WIDTH * 2 + 6, "%d", score);
  }

  mvprintw(10, WIDTH * 2 + 5, "RECORD");
  if (high_score < 100) {
    mvprintw(11, WIDTH * 2 + 7, "%d", high_score);
  } else if (high_score >= 100) {
    mvprintw(11, WIDTH * 2 + 6, "%d", high_score);
  }
}

/**
 * @brief Рисует прямоугольник на экране
 * @details Эта функция рисует прямоугольник на экране, используя символы из
 * библиотеки ncurses. Прямоугольник размещается на заданных координатах
 * (top_y, bottom_y, left_x, right_x)
 * @param top_y Верхняя координата по оси Y
 * @param bottom_y Нижняя координата по оси Y
 * @param left_x Левая координата по оси X
 * @param right_x Правая координата по оси X
 *
 */
void printRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);
  int x = left_x + 1;
  for (; x < right_x; x++) {
    mvaddch(top_y, x, ACS_HLINE);
    mvaddch(bottom_y, x, ACS_HLINE);
  }
  mvaddch(top_y, x, ACS_URCORNER);
  mvaddch(bottom_y, x, ACS_LRCORNER);
  for (int y = top_y + 1; y < bottom_y; y++) {
    mvaddch(y, left_x, ACS_VLINE);
    mvaddch(y, right_x, ACS_VLINE);
  }
  mvaddch(bottom_y, left_x, ACS_LLCORNER);
}

/**
 * @brief Рисует объекты на поле
 * @details Функция рисует объекты (фигуры в Тетрисе и змея и яблоко в Змейке)
 * заданного размера и в заданных координатах. При отрисовке проверяется, чтобы
 * фигура не выходила за пределы поля. Фигура состоит из символов '[]'
 * @param stats Указатель на структуру GameInfo_t, содержащую информацию о
 * положении объектов на поле
 */
void drawObjects(GameInfo_t *stats) {
  for (int y = 1; y <= HEIGHT; y++) {
    for (int x = 1; x <= WIDTH; x++) {
      if (stats->field[y][x] == MOVING_CELL ||
          stats->field[y][x] == STATIC_CELL) {
        if (x > 0 && x <= WIDTH * 2 && y > 0 && y <= HEIGHT) {
          mvaddch(y, x * 2 - 1, '[');
          mvaddch(y, x * 2, ']');
        }
      }
    }
  }
}

/**
 * @brief Очищает экран
 * @details Функция стирает все ранее отображенные на экране символы перед
 * перерисовкой нового состояния игры. Аналогична системной функции clear()
 */
void clearScreen() {
  for (int y = 0; y <= HEIGHT + 1; y++) {
    for (int x = 0; x <= HEIGHT * 2 + 3; x++) {
      mvaddch(y, x, ' ');
    }
  }
}

/**
 * @brief Отображает экран, соответствующий состоянию игры
 * @details В зависимости от предоставленного статуса игры эта функция вызывает
 * соответствующую функцию отображения экрана для отображения на консоли паузы,
 * выхода, выигрыша, окончания игры или сообщения об ошибке
 * @param game_status Структура типа GameStatus_t, хранящая текущее состояние
 * игры
 */
void printStatusScreen(GameStatus_t game_status) {
  if (game_status == kPause) {
    printPause();
  }
  if (game_status == kExit) {
    printBye();
  }
  if (game_status == kWin) {
    printWin();
  }
  if (game_status == kGameOver) {
    printGameOver();
  }
  if (game_status == kError) {
    printMallocError();
  }
}

/**
 * @brief Отображает сообщение об ошибке в выделении памяти
 * @details Очищает экран и отображает сообщение об ошибке в выделении памяти.
 * Сообщение выводится с задержкой в секунду перед выходом из игры
 */
void printMallocError() {
  clearScreen();
  printRectangle(0, HEIGHT + 1, 0, HEIGHT + 1);
  mvprintw(HEIGHT / 2 - 1, 8, "ERROR!");
  mvprintw(HEIGHT / 2, 2, "COULDN'T ALLOCATE");
  mvprintw(HEIGHT / 2 + 1, 8, "MEMORY");
  refresh();
  sleep(1);
}

/**
 * @brief Отображает сообщение о паузе
 * @details Очищает экран и отображает сообщение о паузе на экране. Предлагает
 * пользователю продолжить игру или выйти
 */
void printPause() {
  clearScreen();
  printRectangle(0, HEIGHT + 1, 0, HEIGHT + 1);
  mvprintw(HEIGHT / 2, 5, "GAME PAUSED");
  mvprintw(HEIGHT - 3, 2, "Press P to resume");
  mvprintw(HEIGHT - 2, WIDTH * 2 / 4, "or Q to quit");
  refresh();
}

/**
 * @brief Отображает сообщение о завершении игры
 * @details Очищает экран и отображает сообщение о завершении игры на экране.
 * Сообщение выводится с задержкой в секунду перед выходом из игры
 */
void printBye() {
  clearScreen();
  printRectangle(0, HEIGHT + 1, 0, HEIGHT + 1);
  mvprintw(HEIGHT / 2, 2, "SEE YOU NEXT TIME!");
  refresh();
  sleep(1);
}

/**
 * @brief Отображает сообщение о победе
 * @details Очищает экран и отображает сообщение о победе в игре. Сообщение
 * выводится с задержкой в секунду перед выходом из игры
 */
void printWin() {
  clearScreen();
  printRectangle(0, HEIGHT + 1, 0, HEIGHT + 1);
  mvprintw(HEIGHT / 2, WIDTH * 2 / 4 + 1, "WELL DONE!");
  mvprintw(HEIGHT / 2 + 2, 4, "YOU COMPLETED");
  mvprintw(HEIGHT / 2 + 3, 6, "THE GAME!");
  refresh();
  sleep(1);
}

/**
 * @brief Отображает сообщение о проигрыше
 * @details Очищает экран и отображает сообщение о проигрыше. Сообщение
 * выводится с задержкой в секунду перед выходом из игры
 */
void printGameOver() {
  clearScreen();
  printRectangle(0, HEIGHT + 1, 0, HEIGHT + 1);
  mvprintw(HEIGHT / 2, WIDTH * 2 / 4, "GAME OVER(");
  mvprintw(HEIGHT / 2 + 1, 2, "SEE YOU NEXT TIME!");
  refresh();
  sleep(1);
}