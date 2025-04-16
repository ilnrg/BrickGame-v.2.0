/** @file
 * @brief Файл, содержащий функции для отрисовки игры Змейка в виджете
 */
#include "snake_widget.h"

#include "ui_snake_widget.h"

/**
 * @brief Конструктор класса SnakeWidget
 * @details Инициализирует компоненты пользовательского интерфейса,
 * устанавливает свойства окна и определяет время обновления экрана. Настраивает
 * модель и контроллер игры, а также политики фокусировки для кнопок. Виджет
 * имеет фиксированный размер и называется "Snake". Конструктор также
 * устанавливает атрибут Qt::WA_DeleteOnClose, чтобы виджет был удален при
 * закрытии, и таймер обновления экрана, который вызывается каждые 10
 * миллисекунд
 * @param parent Родительский виджет
 */
SnakeWidget::SnakeWidget(QWidget *parent)
    : GameWidget(parent), ui(new Ui::SnakeWidget), snake_model_(),
      snake_controller_(&snake_model_) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose, true);
  setFocusPolicy(Qt::StrongFocus);
  ui->helpButton->setFocusPolicy(Qt::NoFocus);
  ui->pauseButton->setFocusPolicy(Qt::NoFocus);
  ui->closeButton->setFocusPolicy(Qt::NoFocus);
  resize(435, 480);
  setFixedSize(435, 480);
  setWindowTitle("Snake");
  connect(timer, &QTimer::timeout, this, &SnakeWidget::updateScreen);
  timer->start(10);
}

/**
 * @brief Деструктор класса SnakeWidget
 * @details Удаляет объекты UI
 */
SnakeWidget::~SnakeWidget() { delete ui; }

/**
 * @brief Обработчик события перерисовки виджета
 * @details Перерисовывает виджет в зависимости от текущего состояния игры.
 * Если игра приостановлена, то отображается соответствующий экран.
 * Если игра продолжается, то перерисовывается поле игры, отображается змейка и
 * яблоко, обновляются статистика и счет, а также вызывается механика игры.
 * Если игра проиграна или выиграна, то отображается соответствующий экран.
 * @param event Событие QPaintEvent, указывающее, что необходимо перерисовать
 * виджет
 */
void SnakeWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  GameInfo_t stats = snake_controller_.updateCurrentState();
  s21::SnakeModel::SnakeInfo_t *game_state =
      snake_controller_.getModel()->getSnakeInfo_t();
  ui->level->display(stats.level);
  ui->score->display(stats.score);
  ui->high_score->display(stats.high_score);
  snake_controller_.getModel()->updateField(stats);
  drawCell(&painter, stats.field, 1, 21, 1, 11, 0, 0);
  if (stats.pause) {
    pauseScreen(&painter);
  } else {
    if (game_state->game_status != kGameOver &&
        game_state->game_status != kWin) {
      timer->start(10);
      game_state->game_info = stats;
      snake_controller_.getModel()->snakeMechanics(game_state->game_status);
    }
    if (game_state->game_status == kGameOver) {
      gameoverScreen(&painter);
    }
    if (game_state->game_status == kWin) {
      winScreen(&painter);
    }
  }
}

/**
 * @brief Обработчик события нажатия клавиши
 * @details Вызывается при нажатии клавиши. Если нажата одна из клавиш
 * управления змейкой (стрелки Left, Right, Up, Down), то соответствующее
 * направление отправляется в контроллер игры. Если нажата клавиша p или P, то
 * игра приостанавливается. Если нажата клавиша q, Q или Escape, то игра
 * завершается. Также проверяется зажата ли клавиша, соответствующая направлению
 * движения, для ускорения движения змейки
 * @param event Событие QKeyEvent, указывающее, какая клавиша была нажата
 */
void SnakeWidget::keyPressEvent(QKeyEvent *event) {
  s21::SnakeModel::SnakeInfo_t *game_state =
      snake_controller_.getModel()->getSnakeInfo_t();
  bool hold = false;
  int key = event->key();
  switch (key) {
  case Qt::Key_Space:
    game_state->action = Action;
    break;
  case Qt::Key_Up:
    game_state->action = Up;
    break;
  case Qt::Key_Left:
    game_state->action = Left;
    break;
  case Qt::Key_Right:
    game_state->action = Right;
    break;
  case Qt::Key_Down:
    game_state->action = Down;
    break;
  case 'p':
  case 'P':
    game_state->action = Pause;
    break;
  case 'q':
  case 'Q':
  case Qt::Key_Escape:
    game_state->action = Terminate;
    break;
  case Qt::Key_Enter:
  default:
    game_state->action = Start;
    break;
  }
  s21::Snake::Direction curr_dir =
      snake_controller_.getModel()->getSnake().getDirection();
  if ((curr_dir == s21::Snake::kUp && key == Qt::Key_Up) ||
      (curr_dir == s21::Snake::kDown && key == Qt::Key_Down) ||
      (curr_dir == s21::Snake::kLeft && key == Qt::Key_Left) ||
      (curr_dir == s21::Snake::kRight && key == Qt::Key_Right)) {
    hold = true;
  }
  snake_controller_.userInput(game_state->action, hold);
  updateScreen();
}

/**
 * @brief Обработчик события отпускания клавиши
 * @details Вызывается при отпускании клавиши, что означает необходимость отмены
 * ускорения. Если отпущена одна из клавиш управления змейкой (стрелки Left,
 * Right, Up, Down), то отправляется соответствующее направление с флагом hold
 * == false
 * @param event Событие QKeyEvent, указывающее, какая клавиша была отпущена
 */
void SnakeWidget::keyReleaseEvent(QKeyEvent *event) {
  int key = event->key();
  if (key == Qt::Key_Up || key == Qt::Key_Left || key == Qt::Key_Right ||
      key == Qt::Key_Down) {
    s21::SnakeModel::SnakeInfo_t *game_state =
        snake_controller_.getModel()->getSnakeInfo_t();
    snake_controller_.userInput(game_state->action, false);
  }
  QWidget::keyReleaseEvent(event);
}

/**
 * @brief Обрабатывает событие нажатия кнопки HELP
 * @details Отображает окно сообщения с инструкциями по управлению игрой,
 * включая управление движением, приостановку / возобновление и другие игровые
 * механики. Если игра еще не приостановлена, программа приостанавливает ее
 * перед отображением справочной информации, а затем возобновляет игру при
 * закрытии окна справки
 */
void SnakeWidget::on_helpButton_clicked() {
  int on_pause = snake_controller_.getModel()->getGameInfo_t()->pause;
  if (!on_pause) {
    on_pauseButton_clicked();
  }
  QString help_info =
      "Control the snake to eat food and grow while avoiding collisions with "
      "walls or itself!\n\n"
      "Movement (use arrow keys to steer):\n"
      "- Vertical Movement (Up/Down): Turn with Left/Right arrows\n"
      "- Horizontal Movement (Left/Right): Turn with Up/Down arrows\n"
      "Opposite direction keys (e.g., Left arrow while moving right) are "
      "disabled\n\n"
      "Accelerate: Press the arrow key matching your current direction (e.g., "
      "press Right arrow while moving right)\n\n"
      "Pause/Resume: Press P\n"
      "Quit: Press Q or Esc to exit\n\n"
      "Gameplay Mechanics:\n"
      "- Apple Consumption: Eating an apple grows the snake by 1 segment\n"
      "- Levels & Speed: Every 5 apples increase your level by 1. Each new "
      "level raises the snake’s movement speed\n\n"
      "Click OK to return to the game";
  QMessageBox::information(this, "Quick Guide", help_info);
  if (!on_pause) {
    snake_controller_.userInput(Pause, false);
  }
  update();
}

/**
 * @brief Обрабатывает событие нажатия кнопки PAUSE
 * @details Приостанавливает игру, отправляя статус Pause в контроллер игры.
 * Затем контроллер отправляет действие паузы в paintEvent и интерфейс
 * обновляется для отображения нового состояния.
 */
void SnakeWidget::on_pauseButton_clicked() {
  s21::SnakeModel::SnakeInfo_t *game_state =
      snake_controller_.getModel()->getSnakeInfo_t();
  game_state->action = Pause;
  snake_controller_.userInput(Pause, false);
  update();
}
