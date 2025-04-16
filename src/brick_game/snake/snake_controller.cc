/** @file
 * @brief Файл, содержащий функции для обработки сигналов от пользователя
 */
#include "snake_controller.h"

namespace s21 {
/**
 * @brief Конструктор класса SnakeController
 * @details Инициализирует контроллер игры, передавая указатель на модель игры
 * @param snake_model Указатель на модель игры
 */
SnakeController::SnakeController(SnakeModel *snake_model)
    : snake_model_{snake_model} {}

/**
 * @brief Деструктор класса SnakeController
 */
SnakeController::~SnakeController() {}

/**
 * @brief Функция, которая получает команду от пользователя
 * @details Функция получает команду от пользователя, используя функцию
 * getch. Затем при помощи функции setUserAction определяется, какая команда
 * была получена и присваивается значение структуре UserAction_t. Далее функция
 * вызывается userInput, чтобы обработать полученную команду
 */
void SnakeController::getUserInput() {
  SnakeModel::SnakeInfo_t *game_state = snake_model_->getSnakeInfo_t();
  int key = getch();
  bool hold = false;
  setUserAction(key, &game_state->action, &hold);
  userInput(game_state->action, hold);
}

/**
 * @brief Определяет, какая команда была получена от пользователя
 * @details Функция использует switch, чтобы определить, какая команда была
 * получена от пользователя, и обновляет указатель действия. Если клавиша
 * соответствует направлению движения, функция boostOrNot проверяет, следует ли
 * увеличивать скорость
 * @param key Команда, полученная от пользователя
 * @param action Структура, которая хранит команду, полученную от пользователя
 * @param hold Индикатор зажатия клавиши, соответствующей направлению движения
 */

void SnakeController::setUserAction(int key, UserAction_t *action, bool *hold) {
  switch (key) {
  case SPACE_KEY:
    *action = Action;
    break;
  case KEY_UP:
    *action = Up;
    break;
  case KEY_LEFT:
    *action = Left;
    break;
  case KEY_RIGHT:
    *action = Right;
    break;
  case KEY_DOWN:
    *action = Down;
    break;
  case 'p':
  case 'P':
    *action = Pause;
    break;
  case 'q':
  case 'Q':
  case ESCAPE:
    *action = Terminate;
    break;
  case ENTER_KEY:
  default:
    *action = Start;
    break;
  }
  snake_model_->boostOrNot(key, hold);
}

/**
 * @brief Обработка ввода пользователя
 * @details Эта функция обрабатывает ввод пользователя. Она принимает команду
 * пользователя и определяет, какое действие необходимо выполнить. Переменная
 * hold передаётся в функцию speedBoost для ускорения змейки. Если получена
 * команда Action или Pause, то игра будет приостановлена. Если получены команды
 * Left, Right, Up или Down, то змея будет сдвинута в соответствующем
 * направлении (если игра не приостановлена и поворот допустим). Если получены
 * команды Start или Terminate, то ничего не произойдёт. После этого функция
 * обновляет статистику игры
 * @param action Обрабатываемая команда
 * @param hold Индикатор зажатия клавиши, соответствующей направлению движения
 */
void SnakeController::userInput(UserAction_t action, bool hold) {
  snake_model_->speedBoost(hold);
  switch (action) {
  case Action:
  case Pause:
    snake_model_->getGameInfo_t()->pause =
        !snake_model_->getGameInfo_t()->pause;
    break;
  case Up:
    if (!snake_model_->getGameInfo_t()->pause) {
      snake_model_->getSnake().setDirection(Snake::kUp);
    }
    break;
  case Left:
    if (!snake_model_->getGameInfo_t()->pause) {
      snake_model_->getSnake().setDirection(Snake::kLeft);
    }
    break;
  case Right:
    if (!snake_model_->getGameInfo_t()->pause) {
      snake_model_->getSnake().setDirection(Snake::kRight);
    }
    break;
  case Down:
    if (!snake_model_->getGameInfo_t()->pause) {
      snake_model_->getSnake().setDirection(Snake::kDown);
    }
    break;
  case Start:
  case Terminate:
  default:
    break;
  }
}

/**
 * @brief Возвращает указатель на модель игры
 * @details Функция возвращает указатель на модель игры, с которой работает
 * контроллер
 * @return Указатель на модель игры
 */
SnakeModel *SnakeController::getModel() const { return snake_model_; }

/**
 * @brief Получает информацию о текущем состоянии игры
 * @details Функция возвращает структуру GameInfo_t, содержащую информацию о
 * текущем состоянии игры. Эта функция должна вызываться из интерфейса с
 * определенной периодичностью для поддержания интерфейса в актуальном
 * состоянии
 * @return Структура GameInfo_t, содержащая информацию о текущем состоянии игры
 */
GameInfo_t SnakeController::updateCurrentState() {
  GameInfo_t *stats = snake_model_->getGameInfo_t();
  return *stats;
}
} // namespace s21