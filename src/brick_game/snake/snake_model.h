/** @file
 * @brief Заголовочный файл, определяющий функции бэкенда игры Змейка
 */
#ifndef CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_SNAKE_SNAKE_MODEL_H_
#define CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_SNAKE_SNAKE_MODEL_H_

#include <cstring>
#include <fstream>
#include <vector>

#define SNAKE_MAX_SCORE 196

namespace s21 {
/** @class Snake
 * @brief Класс, содержащий информацию о змее и методы для работы с ней
 * @param start_x Начальное положение на оси X
 * @param start_y Начальное положение на оси Y
 */
class Snake {
public:
  /**
   * @brief Структура, хранящая информацию о направлении движения змейки
   */
  typedef enum { kUp, kDown, kLeft, kRight } Direction;

  // CONSTRUCTOR & DESTRUCTOR
  Snake(int start_x, int start_y);
  ~Snake() = default;

  // GETTERS & SETTER
  const std::vector<std::pair<int, int>> &getSnakeBody() const;
  Direction getDirection() const;
  void setDirection(Direction new_dir);

  // MOVING FUNCS
  std::pair<int, int> nextStep() const;
  void move();
  void grow();

private:
  std::vector<std::pair<int, int>> snake_body_;
  Direction direction_;
};

/** @class Apple
 * @brief Класс, содержащий информацию о яблоки и методы для работы с ним
 */
class Apple {
public:
  // CONSTRUCTOR & DESTRUCTOR
  Apple();
  ~Apple();

  // GETTERS
  int getAppleX() const;
  int getAppleY() const;

  // SPAWN FUNCS
  void spawnApple(const std::vector<std::pair<int, int>> &snake_body);
  bool checkApplesPosition(const std::vector<std::pair<int, int>> &snake_body);

private:
  int apple_x_;
  int apple_y_;
};

/** @class SnakeModel
 * @brief Класс модели игры
 */
class SnakeModel {
public:
  /**
   * @brief Основная игровая структура
   */
  typedef struct {
    UserAction_t action;
    GameInfo_t game_info;
    GameStatus_t game_status;
    long long set_time;
    int current_speed;
  } SnakeInfo_t;

  // CONSTRUCTOR & DESTRUCTOR
  SnakeModel();
  ~SnakeModel();

  // GAME INFO GETTERS
  SnakeInfo_t *getSnakeInfo_t();
  GameInfo_t *getGameInfo_t();
  Snake &getSnake();

  // GAME LOGIC
  void snakeMechanics(GameStatus_t &game_status);
  void updateField(GameInfo_t &stats);

  // SPEED BOOSTER
  void boostOrNot(int key, bool *hold);
  void speedBoost(bool hold);

  // HIGH SCORE SETTER & GETTER
  int getHighScore();
  void setHighScore(int high_score);

private:
  GameInfo_t game_info;
  SnakeInfo_t game_state;
  Snake snake_;
  Apple apple_;

  // GAME LOGIC HELEPRS
  void fillField(GameInfo_t &stats, int x, int y);
  void continueOrNot(UserAction_t state, SnakeInfo_t *game_state);
  bool checkCollision();

  // SCORE & LEVEL UPDATE FUNC
  void updateScore(GameInfo_t *stats);
};

} // namespace s21

#endif // CPP3_BRICK_GAME_V2_0_1_BRICK_GAME_SNAKE_SNAKE_MODEL_H_