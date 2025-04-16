/** @file
 * @brief Файл, содержащий функции для работы с игровой информацией и механикой
 * игры Змейка
 */
#include "snake_controller.h"

namespace s21 {
/**
 * @brief Конструктор кдасса Snake
 * @details Змейка создается длиной 4 ячейки и начинает двигаться вниз по оси Y
 * @param start_x Начальное положение на оси X
 * @param start_y Начальное положение на оси Y
 */
Snake::Snake(int start_x, int start_y) : direction_(kDown) {
  for (int i = 0; i < 4; ++i) {
    snake_body_.push_back({start_x, start_y + i});
  }
}

/**
 * @brief Геттер для вектора змейки
 * @details Возвращает вектор змейки для дальнейшего использования
 * @return Вектор змейки
 */
const std::vector<std::pair<int, int>> &Snake::getSnakeBody() const {
  return snake_body_;
}

/**
 * @brief Геттер направления движения змейки
 * @details Возвращает текущее направление движения змейки
 * @return Направление движения змейки
 */
Snake::Direction Snake::getDirection() const { return direction_; }

/**
 * @brief Сеттер направления движения змейки
 * @details Изменяет текущее направление движения змейки на заданное новое
 * направление, если оно не прямо противоположно текущему направлению. Это
 * предотвращает переворачивание змейки на 180 градусов
 * @param new_dir Новое направление движения
 */
void Snake::setDirection(Direction new_dir) {
  if ((direction_ == kUp && new_dir != kDown) ||
      (direction_ == kDown && new_dir != kUp) ||
      (direction_ == kLeft && new_dir != kRight) ||
      (direction_ == kRight && new_dir != kLeft)) {
    direction_ = new_dir;
  }
}

/**
 * @brief Расчёт следующих координат змейки
 * @details Определяет новые координаты головы змеи, изменяя ее текущее
 * положение в соответствии с направлением движения. Возвращает новое положение
 * в виде пары целых чисел, представляющих координаты X и Y
 * @return Новое положение змейки
 */
std::pair<int, int> Snake::nextStep() const {
  auto head = snake_body_.back();
  int new_x = head.first;
  int new_y = head.second;
  switch (direction_) {
  case kUp:
    new_y--;
    break;
  case kDown:
    new_y++;
    break;
  case kLeft:
    new_x -= 2;
    break;
  case kRight:
    new_x += 2;
    break;
  }
  return {new_x, new_y};
}

/**
 * @brief Движение змейки
 * @details Эта функция перемещает змею по полю, добавляя к голове одну клетку и
 * удаляя одну клетку из хвоста
 */
void Snake::move() {
  grow();
  snake_body_.erase(snake_body_.begin());
}

/**
 * @brief Добавление новой клетки к змее
 * @details Добавляет к змее новую клетку, расположенную на следующей позиции
 * относительно текущей головы
 */
void Snake::grow() {
  auto new_head = nextStep();
  snake_body_.push_back(new_head);
}

/**
 * @brief Конструктор класса Apple
 * @details Инициализирует переменные apple_x_ и apple_y_ нулями (начальное
 * положение яблока до старта игры)
 */
Apple::Apple() : apple_x_(0), apple_y_(0) {}

/**
 * @brief Деструктор класса Apple
 */
Apple::~Apple() {}

/**
 * @brief Геттер для X-координаты яблока
 * @details Возвращает X-координату яблока на поле
 * @return X-координата яблока
 */
int Apple::getAppleX() const { return apple_x_; }

/**
 * @brief Геттер для Y-координаты яблока
 * @details Возвращает Y-координату яблока на поле
 * @return Y-координата яблока
 */
int Apple::getAppleY() const { return apple_y_; }

/**
 * @brief Генерация новой позиции яблока
 * @details Генерирует новую позицию яблока, проверяя не пересекаются ли
 * координаты яблока с координатами тела змейки
 * @param snake_body Вектор змейки
 */
void Apple::spawnApple(const std::vector<std::pair<int, int>> &snake_body) {
  bool is_collision = true;
  while (is_collision) {
    apple_x_ = (rand() % WIDTH) * 2 + 1;
    apple_y_ = rand() % (HEIGHT - 2) + 1;
    is_collision = checkApplesPosition(snake_body);
  }
}

/**
 * @brief Проверка, не попадает ли яблоко на тело змейки
 * @details Проверяет, не находится ли яблоко на теле змейки, сравнивая
 * координаты яблока со всеми координатами тела змейки
 * @param snake_body Вектор змейки
 * @return true, если яблоко находится на теле змейки, иначе false
 */
bool Apple::checkApplesPosition(
    const std::vector<std::pair<int, int>> &snake_body) {
  bool there_is_body = false;
  for (const auto &body : snake_body) {
    int snake_x = body.first;
    int snake_y = body.second;
    if ((apple_x_ == snake_x || apple_x_ == snake_x + 1) &&
        apple_y_ == snake_y) {
      there_is_body = true;
      break;
    }
  }
  return there_is_body;
}

/**
 * @brief Конструктор класса SnakeModel
 * @details Инициализирует змейку, поле, яблоко, счет, уровень и другие
 * переменные игры. Создает двумерный массив field для хранения состояния поля,
 * инициализирует его значения нулями. Создает змейку в заданных координатах,
 * инициализирует переменные score, high_score, level, pause, speed, set_time и
 * current_speed и генерирует начальную позицию яблока
 */
SnakeModel::SnakeModel() : snake_(7, 1) {
  int status = START;
  game_info.field = new int *[HEIGHT + 1];
  if (game_info.field != nullptr) {
    for (int i = 0; i < HEIGHT + 1 && status != STOP; ++i) {
      game_info.field[i] = new int[WIDTH * 2 + 1]{};
      if (game_info.field[i] == nullptr) {
        status = STOP;
      }
    }
    if (status != STOP) {
      game_state.action = Start;
      game_state.game_status = kStart;
      game_info.score = 0;
      game_info.high_score = getHighScore();
      game_info.level = 1;
      game_info.pause = 0;
      game_info.speed = START_SPEED;
      game_state.game_info = game_info;
      game_state.current_speed = START_SPEED;
      game_state.set_time = 0;
      apple_.spawnApple(snake_.getSnakeBody());
    }
  }
}

/**
 * @brief Деструктор класса SnakeModel
 * @details Освобождает память, выделенную под двумерный массив field и
 * сохраняет текущий рекорд
 */
SnakeModel::~SnakeModel() {
  if (game_info.field != nullptr) {
    for (int i = 0; i < HEIGHT + 1; ++i) {
      delete[] game_info.field[i];
    }
    delete[] game_info.field;
  }
  setHighScore(game_info.high_score);
}

/**
 * @brief Геттер структуры SnakeInfo_t
 * @details Функция возвращает указатель на структуру SnakeInfo_t, которая
 * содержит основную игровую информацию
 * @return Указатель на структуру SnakeInfo_t
 */
SnakeModel::SnakeInfo_t *SnakeModel::getSnakeInfo_t() { return &game_state; }

/**
 * @brief Геттер структуры GameInfo_t
 * @details Функция возвращает указатель на структуру GameInfo_t, которая
 * содержит информацию о текущем состоянии игры
 * @return Указатель на структуру GameInfo_t
 */
GameInfo_t *SnakeModel::getGameInfo_t() { return &game_info; }

/**
 * @brief Геттер змейки
 * @details Функция возвращает ссылку на класс Snake
 * @return Ссылка на змейку
 */
Snake &SnakeModel::getSnake() { return snake_; }

/**
 * @brief Основная логика игры
 * @details Функция snakeMechanics управляет ходом игры. Она проверяет,
 * достигнут ли максимальный счет, и если да, то меняет статус игры на "win".
 * Если счет меньше максимального, то функция continueOrNot вызывается
 * для продолжения игры
 * @param game_status Указатель на GameStatus_t, хранящую статус игры
 */
void SnakeModel::snakeMechanics(GameStatus_t &game_status) {
  if (game_info.score == SNAKE_MAX_SCORE) {
    game_status = kWin;
  } else {
    continueOrNot(game_state.action, &game_state);
  }
}

/**
 * @brief Обновляет игровое поле
 * @details Функция обновляет игровое поле, очищая его от предыдущих значений, и
 * заполняя его новыми значениями змейки и яблока. Она используется для
 * обновления интерфейса игры.
 * @param stats Указатель на структуру GameInfo_t, содержащую поле, которое
 * необходимо обновить
 */
void SnakeModel::updateField(GameInfo_t &stats) {
  for (int y = 0; y <= HEIGHT; ++y) {
    for (int x = 0; x <= WIDTH * 2; ++x) {
      stats.field[y][x] = EMPTY_CELL;
    }
  }
  for (const auto &body : snake_.getSnakeBody()) {
    int snake_x = body.first;
    int snake_y = body.second;
    fillField(stats, snake_x, snake_y);
  }
  int apple_x = apple_.getAppleX();
  int apple_y = apple_.getAppleY();
  fillField(stats, apple_x, apple_y);
}

/**
 * @brief Помещает обекты на поле
 * @details Функция помещает змейку и яблоку на поле, т.е. заносит их координаты
 * в матрицу field
 * @param stats Указатель на структуру GameInfo_t, содержащую поле, которое
 * необходимо обновить
 * @param x X-координата
 * @param y Y-координата
 */
void SnakeModel::fillField(GameInfo_t &stats, int x, int y) {
  int cell_x = (x + 1) / 2;
  if (y >= 1 && y <= HEIGHT && cell_x >= 1 && cell_x <= WIDTH) {
    stats.field[y][cell_x] = MOVING_CELL;
  }
}

/**
 * @brief Проверяет, необходимо ли продолжать игру
 * @details Если состояние action равно Terminate, то меняет статус игры на
 * "game over". Иначе, функция, используя функцию checkCollision (проверка на
 * столкновение), проверяет, может ли змея двигаться. Если движение невозможно
 * (столкновение с границами или телом), то статус игры меняется на "game over"
 * @param state Команда пользователя
 * @param game_state Указатель на структуру SnakeInfo_t, содержащую
 * информацию о текущем состоянии игры
 */
void SnakeModel::continueOrNot(UserAction_t state, SnakeInfo_t *game_state) {
  if (state == Terminate) {
    game_state->game_status = kGameOver;
  } else {
    GameInfo_t *stats = &game_state->game_info;
    long long curr_time = setTime();
    if (curr_time - game_state->set_time >= stats->speed) {
      game_state->set_time = curr_time;
      const auto &body = snake_.getSnakeBody();
      auto head = body.back();
      if (head.first == apple_.getAppleX() &&
          head.second == apple_.getAppleY()) {
        updateScore(&game_info);
        apple_.spawnApple(body);
      } else {
        if (!checkCollision()) {
          snake_.move();
        } else {
          game_state->game_status = kGameOver;
        }
      }
    }
  }
}

/**
 * @brief Проверка столкновений
 * @details Функция проверяет, может ли змея двигаться. Если движение невозможно
 * (столкновение с границами или телом), то функция возвращает true
 * @return true, если столкновение происходит, иначе false
 */
bool SnakeModel::checkCollision() {
  bool is_collision = false;
  auto next_step = snake_.nextStep();
  int next_x = next_step.first;
  int next_y = next_step.second;
  if (next_x < 1 || next_x > WIDTH * 2 || next_y < 1 || next_y > HEIGHT) {
    is_collision = true;
  }
  if (!is_collision) {
    const auto &body = snake_.getSnakeBody();
    for (size_t i = 0; i < body.size() - 1 && !is_collision; i++) {
      if (body[i].first == next_x && body[i].second == next_y) {
        is_collision = true;
      }
    }
  }
  return is_collision;
}

/**
 * @brief Функция, отвечающая за ускорение змейки
 * @details Если пользователь нажал клавишу, соответствующую направлению
 * движения змейки, то функция возвращает true, иначе false
 * @param key Клавиша, нажатая пользователем
 * @param hold Указатель на переменную, в которую будет записано
 * значение true, если пользователь нажал клавишу, соответствующую
 * направлению движения змейки, иначе false
 */
void SnakeModel::boostOrNot(int key, bool *hold) {
  Snake::Direction curr_dir = getSnake().getDirection();
  if ((curr_dir == Snake::kUp && key == KEY_UP) ||
      (curr_dir == Snake::kDown && key == KEY_DOWN) ||
      (curr_dir == Snake::kLeft && key == KEY_LEFT) ||
      (curr_dir == Snake::kRight && key == KEY_RIGHT)) {
    *hold = true;
  } else {
    *hold = false;
  }
}

/**
 * @brief Ускоряет змейку
 * @details Если hold равно true, то функция ускоряет змейку, уменьшая
 * значение speed. Если hold равно false, то функция возвращает speed
 * к значению current_speed
 * @param hold true, если ускорение змейки включено, иначе false
 */
void SnakeModel::speedBoost(bool hold) {
  if (hold) {
    int boost = 5;
    if (game_info.level > 5) {
      boost = 3;
    }
    game_info.speed /= boost;
  } else {
    game_info.speed = game_state.current_speed;
  }
}

/**
 * @brief Обновляет статистику игры
 * @details Функция обновляет статистику игры, если съедено яблоко. Функция
 * увеличивает счет игры на 1, обновляет рекорд, если счет игры стал больше
 * рекорда, и обновляет уровень игры, если счет игры достиг 5 очков. Затем
 * вызывается функция setSpeed для увеличения скорости падения фигур
 * @param stats Информация о состоянии игры
 */
void SnakeModel::updateScore(GameInfo_t *stats) {
  snake_.grow();
  stats->score++;
  if (stats->score > stats->high_score) {
    stats->high_score = stats->score;
  }
  stats->level = 1 + stats->score / 5;
  setSpeed(&stats->level, &stats->speed);
  game_state.current_speed = stats->speed;
}

/**
 * @brief Получает high_score из файла
 * @details Функция читает максимальное количество очков после последней сессии
 * игры из файла snakeHS.txt и возвращает его. Если файл не существует, то
 * функция возвращает 0
 * @return Рекорд, если файл существует, 0 - если файла не существует
 */
int SnakeModel::getHighScore() {
  int high_score = 0;
  std::ifstream file("./snakeHS.txt");
  if (file.is_open()) {
    file >> high_score;
    file.close();
  }
  return high_score;
}

/**
 * @brief Записывает high_score в файл
 * @details Функция записывает рекорд в файл snakeHS.txt. Если файл не
 * существует, то он будет создан
 * @param high_score Рекорд, который будет записан
 */
void SnakeModel::setHighScore(int high_score) {
  std::ofstream file("./snakeHS.txt");
  if (file.is_open()) {
    file << high_score;
    file.close();
  }
}
} // namespace s21