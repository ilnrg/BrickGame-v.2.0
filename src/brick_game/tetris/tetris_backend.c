/** @file
 * @brief Файл, содержащий функции для работы с игровой информацией и механикой
 * игры Тетрис
 */
#include "tetris_backend.h"

/**
 * @brief Получает указатель на структуру TetrisInfo_t
 * @details Функция возвращает указатель на структуру TetrisInfo_t,
 * содержащую состояние игры. В случае, если состояние игры ещё не
 * инициализировано, функция вызывает функцию createInfo_t для его
 * инициализации
 * @return Указатель на инициализированную структуру или NULL, если
 * инициализация не удалась
 */
TetrisInfo_t *getTetrisInfo_t() {
  static TetrisInfo_t game_state;
  int status = START;
  if (game_state.game_info.field == NULL) {
    status = createInfo_t(&game_state);
  }
  return status == START ? &game_state : NULL;
}

/**
 * @brief Инициализирует структуру TetrisInfo_t
 * @details Функция инициализирует структуру, содержащую состояние игры.
 * Устанавливает начальные значения для полей структуры, заполняет матрицы field
 * и next и заполняет массив figures порядковыми номерами фигур
 * @param game_state Указатель на структуру TetrisInfo_t
 * @return START, если инициализация прошла успешно, и STOP в противном случае
 */
int createInfo_t(TetrisInfo_t *game_state) {
  int status = START;
  GameInfo_t *stats = &game_state->game_info;
  status = createMatrix(HEIGHT + 1, WIDTH * 2 + 1, &stats->field);
  if (status != STOP) {
    status = createMatrix(4, 4, &stats->next);
    if (status != STOP) {
      game_state->action = Start;
      game_state->game_status = kStart;
      stats->score = 0;
      stats->high_score = getHighScore();
      stats->level = 1;
      stats->speed = START_SPEED;
      stats->pause = 0;
      figureOrdering();
      corrSpawn(game_state->figures, FIGURES_COUNT);
      initNextFigure(&game_state->game_info);
      spawnFigure(game_state);
    }
  }
  return status;
}

/**
 * @brief Cоздает двумерную матрицу
 * @details Функция выделяет память для двумерной матрицы заданного размера
 * @param height Размер матрицы по вертикали
 * @param width Размер матрицы по горизонтали
 * @param matrix Указатель на указатель на матрицу
 * @return START, если создание прошло успешно, и STOP в противном случае
 */
int createMatrix(int height, int width, int ***matrix) {
  int status = START;
  *matrix = (int **)malloc(height * sizeof(int *));
  if (*matrix != NULL) {
    for (int i = 0; i < height && !status; i++) {
      (*matrix)[i] = (int *)malloc((width) * sizeof(int));
      if ((*matrix)[i] == NULL) {
        status = STOP;
      }
      memset((*matrix)[i], 0, width * sizeof(int));
    }
  } else {
    status = STOP;
  }
  return status;
}

/**
 * @brief Очищает структуру GameInfo_t
 * @details Функция вызывается при завершении игры, очищает структуру GameInfo_t
 * и освобождает занятую память. Для сохранения рекорда вызывается функция
 * setHighScore
 */
void removeGameInfo_t() {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  GameInfo_t *stats = &game_state->game_info;
  if (stats != NULL) {
    removeMatrix(stats->field, HEIGHT + 1);
    stats->field = NULL;
    removeMatrix(stats->next, 4);
    stats->next = NULL;
    setHighScore(stats->high_score);
  }
}

/**
 * @brief Очищает матрицу
 * @details Функция освобождает память, выделенную для матриц
 * field и next. Вспомогательная функция для removeGameInfo_t
 * @param matrix Указатель на указатель на матрицу
 * @param height Размер матрицы
 */
void removeMatrix(int **matrix, int height) {
  if (matrix != NULL) {
    for (int i = 0; i < height; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

/**
 * @brief Определяет порядок появления фигур
 * @details Функция определяет порядок появления фигур, которые будут
 * использоваться в игре, заполняя массив figures порядковыми номерами фигур
 */
void figureOrdering() {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->curr_figure = FIGURES_COUNT;
  for (int i = 0; i < FIGURES_COUNT; i++) {
    game_state->figures[i] = i;
  }
}

/**
 * @brief Перемешивает очерёдность фигур
 * @details Функция перемешивает массив порядковых номеров figures
 * для того чтобы фигуры появлялись в случайном порядке
 * @param figures Массив, содержащий порядковые номера фигур
 * @param count Количество фигур
 */
void corrSpawn(int *figures, int count) {
  for (int i = count - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = figures[i];
    figures[i] = figures[j];
    figures[j] = temp;
  }
}

/**
 * @brief Размещает фигуру на поле
 * @details Функция размещает фигуру на поле по заданным координатам и размерам
 * и инициализирует следующую фигуру
 * @param game_state Указатель на структуру TetrisInfo_t
 */
void spawnFigure(TetrisInfo_t *game_state) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      game_state->figure.f[y][x] = game_state->game_info.next[y][x];
    }
  }
  game_state->figure.width = game_state->next_figure.width;
  game_state->figure.height = game_state->next_figure.height;
  game_state->figure.x = (WIDTH - game_state->figure.width) / 2 + 1;
  game_state->figure.y = 1;
  initNextFigure(&game_state->game_info);
}

/**
 * @brief Инициализирует следующую фигуру
 * @details Функция определяет фигуру, которая появится на поле следующей, и
 * заполняет матрицу next для предпросмотра в специальном окне. В случае если
 * все фигуры использованы, вызывается функция corrSpawn для нового определения
 * очерёдности
 * @param stats Указатель на структуру GameInfo_t
 */
void initNextFigure(GameInfo_t *stats) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  if (game_state->curr_figure >= FIGURES_COUNT) {
    corrSpawn(game_state->figures, FIGURES_COUNT);
    game_state->curr_figure = 0;
  }
  int next_type = game_state->figures[game_state->curr_figure];
  game_state->curr_figure++;
  initFigure(&game_state->next_figure, next_type);
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      stats->next[y][x] = game_state->next_figure.f[y][x];
    }
  }
}

/**
 * @brief Выбирает фигуру
 * @details Функция бирает фигуру по её номеру и заполняет матрицу
 * @param figure Указатель на структуру Figure_t
 * @param type Номер фигуры
 */
void initFigure(Figure_t *figure, int type) {
  figure->width = 4;
  figure->height = 2;
  switch (type) {
  case 0:
    lineFigure(figure);
    break;
  case 1:
    squareFigure(figure);
    break;
  case 2:
    zFigure(figure);
    break;
  case 3:
    sFigure(figure);
    break;
  case 4:
    tFigure(figure);
    break;
  case 5:
    lFigure(figure);
    break;
  case 6:
    jFigure(figure);
    break;
  default:
    break;
  }
}

/**
 * @brief Копирует фигуру из одной матрицы в другую
 * @details Функция копирует фигуру из одной матрицы (src) в другую (dst)
 * @param src Исходная матрица
 * @param dst Матрица, в которую происходит копирование
 * @param height Высота матрицы
 * @param width Ширина матрицы
 */
void copyFigure(int src[4][4], int dst[4][4], int height, int width) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      dst[y][x] = src[y][x];
    }
  }
}

/**
 * @brief Инициализирует фигуру "линия"
 * @details Эта функция инициализирует фигуру "линия". Готовая фигура копируется
 * в массив figure для отображения на поле
 * @param figure Указатель на инициализируемую структуру Figure_t
 */
void lineFigure(Figure_t *figure) {
  int f[4][4] = {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copyFigure(f, figure->f, figure->height, figure->width);
}

/**
 * @brief Инициализирует фигуру "квадрат"
 * @details Эта функция инициализирует фигуру "линия". Готовая фигура копируется
 * в массив figure для отображения на поле
 * @param figure Указатель на инициализируемую структуру Figure_t
 */
void squareFigure(Figure_t *figure) {
  int f[4][4] = {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copyFigure(f, figure->f, figure->height, figure->width);
}

/**
 * @brief Инициализирует фигуру "Z"
 * @details Эта функция инициализирует фигуру "линия". Готовая фигура копируется
 * в массив figure для отображения на поле
 * @param figure Указатель на инициализируемую структуру Figure_t
 */
void zFigure(Figure_t *figure) {
  int f[4][4] = {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copyFigure(f, figure->f, figure->height, figure->width);
}

/**
 * @brief Инициализирует фигуру "S"
 * @details Эта функция инициализирует фигуру "линия". Готовая фигура копируется
 * в массив figure для отображения на поле
 * @param figure Указатель на инициализируемую структуру Figure_t
 */
void sFigure(Figure_t *figure) {
  int f[4][4] = {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copyFigure(f, figure->f, figure->height, figure->width);
}

/**
 * @brief Инициализирует фигуру "T"
 * @details Эта функция инициализирует фигуру "линия". Готовая фигура копируется
 * в массив figure для отображения на поле
 * @param figure Указатель на инициализируемую структуру Figure_t
 */
void tFigure(Figure_t *figure) {
  int f[4][4] = {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copyFigure(f, figure->f, figure->height, figure->width);
}

/**
 * @brief Инициализирует фигуру "L"
 * @details Эта функция инициализирует фигуру "линия". Готовая фигура копируется
 * в массив figure для отображения на поле
 * @param figure Указатель на инициализируемую структуру Figure_t
 */
void lFigure(Figure_t *figure) {
  int f[4][4] = {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copyFigure(f, figure->f, figure->height, figure->width);
}

/**
 * @brief Инициализирует фигуру "J"
 * @details Эта функция инициализирует фигуру "линия". Готовая фигура копируется
 * в массив figure для отображения на поле
 * @param figure Указатель на инициализируемую структуру Figure_t
 */
void jFigure(Figure_t *figure) {
  int f[4][4] = {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copyFigure(f, figure->f, figure->height, figure->width);
}

/**
 * @brief Возвращает информацию о текущем состоянии игры
 * @details Эта функция возвращает структуру GameInfo_t, содержащую информацию о
 * текущем состоянии игры. Эта функция должна вызываться из интерфейса с
 * определенной периодичностью для поддержания интерфейса в актуальном
 * состоянии
 * @return Структура GameInfo_t, содержащая информацию о текущем состоянии игры
 */

GameInfo_t updateCurrentState() {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  GameInfo_t *stats = &game_state->game_info;
  return *stats;
}

/**
 * @brief Главная логика игры
 * @details Функция tetrisMechanics управляет ходом игры. Она проверяет,
 * достигнут ли максимальный счет, и если да, то меняет статус игры на "win".
 * Если счет меньше максимального, то функция continueOrNot вызывается
 * для продолжения игры
 * @param game_state Указатель на структуру TetrisInfo_t, содержащую
 * информацию о текущем состоянии игры
 */
void tetrisMechanics(TetrisInfo_t *game_state) {
  if (game_state->game_info.score >= TETRIS_MAX_SCORE) {
    game_state->game_status = kWin;
  } else {
    continueOrNot(game_state->action, game_state);
  }
}

/**
 * @brief Проверяет, необходимо ли продолжать игру
 * @details Если состояние action равно Terminate, то меняет статус игры на
 * "game over". Иначе, функция, используя функцию checkCollision (проверка на
 * столкновение), проверяет, можно ли сдвинуть фигуру вниз. Если фигура не может
 * быть сдвинута вниз, то функция updateField сохраняет упавшую фигуру и
 * обновляет игровое поле, функция removeLine проверяет, есть ли полные линии,
 * которые необходимо удалить, и затем вызывается функция spawnFigure для вызова
 * следующей фигуры. Если новая фигура не может быть сдвинута вниз (нет места на
 * поле), то статус игры меняется на "game over"
 * @param state Команда пользователя
 * @param game_state Указатель на структуру TetrisInfo_t, содержащую
 * информацию о текущем состоянии игры
 */
void continueOrNot(UserAction_t state, TetrisInfo_t *game_state) {
  if (state == Terminate) {
    game_state->game_status = kGameOver;
  } else {
    GameInfo_t *stats = &game_state->game_info;
    Figure_t *figure = &game_state->figure;
    long long curr_time = setTime();
    if (curr_time - game_state->set_time >= stats->speed) {
      game_state->set_time = curr_time;
      if (!checkCollision(stats, figure, 0, 1)) {
        figure->y++;
      } else {
        updateField(stats, figure, STATIC_CELL);
        removeLine(game_state);
        spawnFigure(game_state);
        if (checkCollision(stats, figure, 0, 0)) {
          game_state->game_status = kGameOver;
        }
      }
    }
  }
}

/**
 * @brief Обновляет игровое поле в соответствии с текущим положением фигуры
 * @details Эта функция выполняет итерацию по размерам указанной фигуры и
 * обновляет матрицу игрового поля в соответствии с текущим положением фигуры с
 * заданным типом ячейки. Обновление происходит только в том случае, если
 * сегмент фигуры помечен как MOVING_CELL и вычисленная позиция находится в
 * пределах поля
 * @param stats Указатель на структуру GameInfo_t, содержащую поле, которое
 * необходимо обновить
 * @param figure Указатель на структуру фигуры, представляющую текущую фигуру на
 * поле
 * @param cell_type Тип ячейки, размещаемой на поле в месте расположения фигуры,
 * который определяет ее состояние (MOVING_CELL - падающая фигура, STATIC_CELL -
 * застывшая фигура, EMPTY_CELL - пустая ячейка)
 */

void updateField(GameInfo_t *stats, Figure_t *figure, int cell_type) {
  for (int y = 0; y < figure->height; y++) {
    for (int x = 0; x < figure->width; x++) {
      if (figure->f[y][x] == MOVING_CELL) {
        if (figure->x + x >= 1 && figure->x + x <= WIDTH &&
            figure->y + y >= 1 && figure->y + y <= HEIGHT) {
          stats->field[figure->y + y][figure->x + x] = cell_type;
        }
      }
    }
  }
}

/**
 * @brief Проверяет, столкнулась ли фигура с другими фигурами или с границами
 * игрового поля
 * @details Функция проверяет, не столкнулась ли фигура с другими фигурами или с
 * границами поля. Функция возвращает true, если столкновение произошло, и
 * false, если столкновения не произошло
 * @param stats Текущее состояние игры
 * @param figure Фигура, которую нужно проверить
 * @param offset_x Смещение фигуры по горизонтали
 * @param offset_y Смещение фигуры по вертикали
 * @return true, если столкновение произошло, и false, если
 * столкновения не произошло
 */
bool checkCollision(GameInfo_t *stats, Figure_t *figure, int offset_x,
                    int offset_y) {
  bool is_collision = false;
  for (int y = 0; y < figure->height && !is_collision; y++) {
    for (int x = 0; x < figure->width && !is_collision; x++) {
      if (figure->f[y][x] == MOVING_CELL) {
        int next_y = figure->y + y + offset_y;
        int next_x = figure->x + x + offset_x;
        if (next_x < 1 || next_x > WIDTH || next_y < 1 || next_y > HEIGHT) {
          is_collision = true;
        } else if (stats->field[next_y][next_x] == STATIC_CELL) {
          is_collision = true;
        }
      }
    }
  }
  return is_collision;
}

/**
 * @brief Функция, которая получает команду от пользователя
 * @details Функция получает команду от пользователя, используя функцию
 * getch. Затем при помощи функции setUserAction определяется, какая команда
 * была получена и присваивается значение структуре UserAction_t. Далее функция
 * вызывается userInput, чтобы обработать полученную команду
 */
void getUserInput() {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  int key = getch();
  bool hold = false;
  setUserAction(key, &game_state->action);
  userInput(game_state->action, hold);
}

/**
 * @brief Определяет, какая команда была получена от пользователя
 * @details Функция использует switch, чтобы определить, какая команда была
 * получена от пользователя. Если пользователь нажал Enter, то игра будет
 * начата. Если пользователь нажал пробел или вверх, то фигура будет повернута.
 * Если пользователь нажал влево, то фигура будет сдвинута влево. Если
 * пользователь нажал вправо, то фигура будет сдвинута вправо. Если пользователь
 * нажал вниз, то фигура будет сдвинута вниз. Если пользователь нажал 'p' или
 * 'P', то игра будет приостановлена. Если пользователь нажал 'q', 'Q', или
 * ESCAPE, то игра будет остановлена.
 * @param key Команда, полученная от пользователя
 * @param state Структура, которая хранит команду, полученную от пользователя
 */
void setUserAction(int key, UserAction_t *state) {
  switch (key) {
  case ENTER_KEY:
    *state = Start;
    break;
  case SPACE_KEY:
  case KEY_UP:
    *state = Action;
    break;
  case KEY_LEFT:
    *state = Left;
    break;
  case KEY_RIGHT:
    *state = Right;
    break;
  case KEY_DOWN:
    *state = Down;
    break;
  case 'p':
  case 'P':
    *state = Pause;
    break;
  case 'q':
  case 'Q':
  case ESCAPE:
    *state = Terminate;
    break;
  default:
    *state = Up;
    break;
  }
}

/**
 * @brief Обработка ввода пользователя
 * @details Эта функция обрабатывает ввод пользователя. Она принимает команду
 * пользователя и определяет, какое действие необходимо выполнить. Если получена
 * команда Action, то фигура будет повернута. Если получена команда Pause, то
 * игра будет приостановлена. Если получены команды Left, Right или Down, то
 * фигура будет сдвинута в соответствующем направлении. Если получены команды
 * Start, Terminate, или Up, то ничего не произойдёт. После этого функция
 * обновляет статистику игры
 * @param action Обрабатываемая команда
 * @param hold Не используется
 */
void userInput(UserAction_t action, bool hold) {
  if (hold) {
    hold = false;
  }
  TetrisInfo_t *game_state = getTetrisInfo_t();
  GameInfo_t stats = updateCurrentState();
  switch (action) {
  case Action:
    rotateFigure(game_state);
    break;
  case Pause:
    stats.pause = !stats.pause;
    break;
  case Left:
    if (!stats.pause) {
      moveLeft(game_state);
    }
    break;
  case Right:
    if (!stats.pause) {
      moveRight(game_state);
    }
    break;
  case Down:
    if (!stats.pause) {
      moveDown(game_state);
    }
    break;
  case Start:
  case Terminate:
  case Up:
  default:
    break;
  }
  game_state->game_info = stats;
}

/**
 * @brief Функция, поворачивающая фигуру
 * @details Функция поворачивает фигуру на 90 градусов, путём перестановки
 * элементов фигуры в обратном порядке. Если после поворота фигура не поместится
 * на поле, то она останется на прежнем месте
 * @param game_state Информация о состоянии игры
 */
void rotateFigure(TetrisInfo_t *game_state) {
  Figure_t *figure = &game_state->figure;
  Figure_t temp_figure = *figure;
  int temp_matrix[4][4] = {0};
  int new_width = figure->height;
  int new_height = figure->width;
  for (int y = 0; y < figure->height; y++) {
    for (int x = 0; x < figure->width; x++) {
      temp_matrix[x][figure->height - 1 - y] = figure->f[y][x];
    }
  }
  temp_figure.width = new_width;
  temp_figure.height = new_height;
  memcpy(temp_figure.f, temp_matrix, sizeof(temp_matrix));
  if (!checkCollision(&game_state->game_info, &temp_figure, 0, 0)) {
    *figure = temp_figure;
  }
}

/**
 * @brief Перемещает фигуру влево
 * @details Функция перемещает фигуру влево, если это возможно, т.е.
 * до тех пор, пока не возникнет столкновение с левой границей поля или
 * с уже упавшими фигурами. Функция уменьшает X-координату фигуры на величину
 * смещения
 * @param game_state Информация о состоянии игры
 */
void moveLeft(TetrisInfo_t *game_state) {
  GameInfo_t *stats = &game_state->game_info;
  Figure_t *figure = &game_state->figure;
  if (!checkCollision(stats, figure, -1, 0)) {
    figure->x--;
  }
}

/**
 * @brief Перемещает фигуру влево
 * @details Функция перемещает фигуру влево, если это возможно, т.е.
 * до тех пор, пока не возникнет столкновение с правой границей поля или
 * с уже упавшими фигурами. Функция увеличивает X-координату фигуры на величину
 * смещения
 * @param game_state Информация о состоянии игры
 */
void moveRight(TetrisInfo_t *game_state) {
  GameInfo_t *stats = &game_state->game_info;
  Figure_t *figure = &game_state->figure;
  if (!checkCollision(stats, figure, 1, 0)) {
    figure->x++;
  }
}

/**
 * @brief Мгновенное падение фигуры
 * @details Функция перемещает фигуру вниз до ближайшей нижней границы, если это
 * возможно, т.е. до тех пор, пока не возникнет столкновение с нижней границей
 * поля или с уже упавшими фигурами. Функция увеличивает Y-координату фигуры на
 * величину смещения
 * @param game_state Информация о состоянии игры
 */
void moveDown(TetrisInfo_t *game_state) {
  GameInfo_t *stats = &game_state->game_info;
  Figure_t *figure = &game_state->figure;
  while (!checkCollision(stats, figure, 0, 1)) {
    figure->y++;
  }
}

/**
 * @brief Удаляет заполненные линии
 * @details Функция проходит по игровому полю и проверяет, присутствуют ли
 * заполненные линии и удаляет их, если они присутствуют. Незаполненные линии
 * смещаются вниз на количество очищенных линий. Если линии удалены, то функция
 * обновляет статистику игры, вызывая функцию updateScore
 * @param game_state Информация о состоянии игры
 */
void removeLine(TetrisInfo_t *game_state) {
  GameInfo_t *stats = &game_state->game_info;
  int how_much = 0;
  for (int y = HEIGHT; y > 0; y--) {
    if (checkLine(stats, y)) {
      how_much++;
      for (int i = y; i > 1; i--) {
        for (int x = 1; x <= WIDTH; x++) {
          stats->field[i][x] = stats->field[i - 1][x];
        }
      }
      for (int x = 1; x <= WIDTH; x++) {
        stats->field[1][x] = EMPTY_CELL;
      }
      y++;
    }
  }
  if (how_much > 0) {
    updateScore(stats, how_much);
  }
}

/**
 * @brief Проверяет, является ли линия заполненной
 * @details Функция проходит по заданной горизонтальной линии и проверяет,
 * заполнена ли она. Если линия заполнена, то функция возвращает true, иначе -
 * false
 * @param stats Информация о состоянии игры
 * @param y Номер линии, которую нужно проверить
 * @return true, если линия заполнена, false - если нет
 */
bool checkLine(GameInfo_t *stats, int y) {
  bool filled_line = true;
  for (int x = 1; x <= WIDTH && filled_line != 0; x++) {
    if (stats->field[y][x] == EMPTY_CELL) {
      filled_line = false;
    }
  }
  return filled_line;
}

/**
 * @brief Обновляет статистику игры
 * @details Функция обновляет статистику игры, если была удалена хотя бы одна
 * линия. Функция увеличивает счет игры на соответствующее количество очков,
 * обновляет рекорд, если счет игры стал больше рекорда, и обновляет уровень
 * игры, если счет игры достиг 600 очков. Затем вызывается функция setSpeed
 * для увеличения скорости падения фигур
 * @param stats Информация о состоянии игры
 * @param how_much Количество линий, которые были удалены
 */
void updateScore(GameInfo_t *stats, int how_much) {
  if (how_much == 1) {
    stats->score += 100;
  } else if (how_much == 2) {
    stats->score += 300;
  } else if (how_much == 3) {
    stats->score += 700;
  } else if (how_much == 4) {
    stats->score += 1500;
  }
  if (stats->score > stats->high_score) {
    stats->high_score = stats->score;
  }
  stats->level = 1 + stats->score / 600;
  setSpeed(&stats->level, &stats->speed);
}

/**
 * @brief Получает high_score из файла
 * @details Функция читает максимальное количество очков после последней сессии
 * игры из файла tetrisHS.txt и возвращает его. Если файл не существует, то
 * функция возвращает 0
 * @return Рекорд, если файл существует, 0 - если файла не существует
 */
int getHighScore() {
  int high_score = 0;
  FILE *file = fopen("./tetrisHS.txt", "r");
  if (file != NULL) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}

/**
 * @brief Записывает high_score в файл
 * @details Функция записывает рекорд в файл tetrisHS.txt. Если файл не
 * существует, то он будет создан
 * @param high_score Рекорд, который будет записан
 */
void setHighScore(int high_score) {
  FILE *file = fopen("./tetrisHS.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}