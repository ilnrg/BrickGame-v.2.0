/** @file
 * @brief Файл, устанавливающий точку входа в программу и инициализирующий
 * начальные состояния
 */
#include "brick_game.h"

/**
 * @brief Начало программы
 * @details Определяет точку входа в программу, инициализирует генератор
 * случайных чисел (для фигур в Тетрисе и яблока в Змейке), устанавливает
 * настройки терминала и запускает меню выбора игры
 *
 * @return 0 в случае успеха
 */
int main() {
  srand(time(NULL));
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  brickGame();
  getch();
  endwin();
  return 0;
}

/**
 * @brief Входная точка игры
 * @details В зависимости от выбора игрока запускает игровой цикл,
 * соответствующий выбранной игре, или выходит из игры
 */
void brickGame() {
  int status = START;
  while (status != STOP) {
    printStartScreen();
    status = getStatus();
    if (status == TETRIS) {
      tetrisCycle();
    }
    if (status == SNAKE) {
      s21::SnakeModel snake_model;
      s21::SnakeController snake_controller(&snake_model);
      s21::SnakeView snake_view(&snake_controller);
      snake_view.snakeCycle();
    }
    if (status == STOP) {
      printBye();
      break;
    }
  }
}

/**
 * @brief Функция, выводящая стартовый экран
 * @details Функция, которая рисует на экране стартовое меню, состоящее из
 * рамки, надписей с именами игр и возможных опций
 */
void printStartScreen() {
  clearScreen();
  printRectangle(0, HEIGHT + 1, 0, HEIGHT + 1);
  printRectangle(1, HEIGHT / 2 - 1, 1, HEIGHT);
  mvprintw(2, 6, "WELCOME TO");
  mvprintw(3, 6, "BRICK GAME");
  mvaddch(5, 9, '[');
  mvaddch(5, 10, ']');
  mvaddch(5, 11, '[');
  mvaddch(5, 12, ']');
  mvaddch(6, 9, '[');
  mvaddch(6, 10, ']');
  mvaddch(6, 11, '[');
  mvaddch(6, 12, ']');
  printRectangle(HEIGHT / 2, HEIGHT / 2 + 3, 0, HEIGHT + 1);
  mvaddch(HEIGHT / 2, 0, ACS_LTEE);
  mvaddch(HEIGHT / 2 + 3, 0, ACS_LTEE);
  mvaddch(HEIGHT / 2, HEIGHT + 1, ACS_RTEE);
  mvaddch(HEIGHT / 2 + 3, HEIGHT + 1, ACS_RTEE);
  drawDPad();
  printRectangle(0, HEIGHT / 2, HEIGHT + 2, HEIGHT * 2 + 3);
  mvprintw(1, HEIGHT + 3, "CHOOSE THE GAME:");
  mvprintw(3, HEIGHT + 3, "Press T for TETRIS");
  mvprintw(4, HEIGHT + 3, "Press S for SNAKE");
  mvprintw(6, HEIGHT + 3, "Press Q to quit");
  mvprintw(HEIGHT, HEIGHT + 3, "powered by yajirobh");
  refresh();
}

/**
 * @brief Рисует DPad на экране
 * @details Эта функция отображает на экране DPad (набор кнопок,
 * расположенных в форме ромба). Панель отображается в нижней части
 * экрана и состоит из четырех кнопок: вверх, вниз, влево и вправо. Кнопки
 * отображаются с использованием ACS-символов
 */
void drawDPad() {
  mvaddch(HEIGHT / 2 + 4, 4, ACS_ULCORNER);
  mvaddch(HEIGHT / 2 + 4, 5, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 4, 6, ACS_URCORNER);
  mvaddch(HEIGHT / 2 + 5, 4, ACS_LRCORNER);
  mvaddch(HEIGHT / 2 + 5, 3, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 5, 2, ACS_ULCORNER);
  mvaddch(HEIGHT / 2 + 6, 2, ACS_LLCORNER);
  mvaddch(HEIGHT / 2 + 6, 3, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 6, 4, ACS_URCORNER);
  mvaddch(HEIGHT / 2 + 7, 4, ACS_LLCORNER);
  mvaddch(HEIGHT / 2 + 7, 5, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 7, 6, ACS_LRCORNER);
  mvaddch(HEIGHT / 2 + 6, 6, ACS_ULCORNER);
  mvaddch(HEIGHT / 2 + 6, 7, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 6, 8, ACS_LRCORNER);
  mvaddch(HEIGHT / 2 + 5, 8, ACS_URCORNER);
  mvaddch(HEIGHT / 2 + 5, 7, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 5, 6, ACS_LLCORNER);

  mvaddch(HEIGHT / 2 + 5, HEIGHT - 6, ACS_ULCORNER);
  mvaddch(HEIGHT / 2 + 5, HEIGHT - 5, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 5, HEIGHT - 4, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 5, HEIGHT - 3, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 5, HEIGHT - 2, ACS_URCORNER);
  mvaddch(HEIGHT / 2 + 6, HEIGHT - 6, ACS_VLINE);
  mvaddch(HEIGHT / 2 + 7, HEIGHT - 6, ACS_LLCORNER);
  mvaddch(HEIGHT / 2 + 7, HEIGHT - 5, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 7, HEIGHT - 4, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 7, HEIGHT - 3, ACS_HLINE);
  mvaddch(HEIGHT / 2 + 7, HEIGHT - 2, ACS_LRCORNER);
  mvaddch(HEIGHT / 2 + 6, HEIGHT - 2, ACS_VLINE);
}

/**
 * @brief Определение начала или завершения игры
 * @details Функция ожидает команду пользователя:
 * - если пользователь нажал t или T, запускается Тетрис;
 * - если пользователь нажал s или S, запускается Змейка;
 * - если пользователь нажал ESCAPE, 'q' или 'Q', игра завершается;
 * и возвращает сигнал, соответствующий выбору игрока
 *
 * @return TETRIS если пользователь нажал t или T, SNAKE если пользователь
 * нажал s или S и STOP если пользователь нажал ESCAPE или 'q' или 'Q'
 */

int getStatus() {
  int status = START;
  int c = getch();
  switch (c) {
  case 't':
  case 'T':
    status = TETRIS;
    break;
  case 's':
  case 'S':
    status = SNAKE;
    break;
  case ESCAPE:
  case 'q':
  case 'Q':
    status = STOP;
    break;
  }
  return status;
}