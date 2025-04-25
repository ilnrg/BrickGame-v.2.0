/** @file
 * @brief Файл, содержащий функции для отрисовки игры Тетрис в виджете
 */
#include "tetris_widget.h"

#include "ui_tetris_widget.h"

/**
 * @brief Конструктор класса TetrisWidget
 * @details Инициализирует компоненты пользовательского интерфейса,
 * устанавливает свойства окна и определяет время обновления экрана. Настраивает
 * политики фокусировки для кнопок. Виджет имеет фиксированный размер и
 * называется "Tetris". Конструктор также устанавливает атрибут
 * Qt::WA_DeleteOnClose, чтобы виджет был удален при закрытии, и таймер
 * обновления экрана, который вызывается каждые 10 миллисекунд
 * @param parent Родительский виджет
 */
TetrisWidget::TetrisWidget(QWidget *parent)
    : GameWidget(parent), ui(new Ui::TetrisWidget) {
  ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_DeleteOnClose, true);
  resize(435, 480);
  setFixedSize(435, 480);
  setWindowTitle("Tetris");
  ui->helpButton->setFocusPolicy(Qt::NoFocus);
  ui->pauseButton->setFocusPolicy(Qt::NoFocus);
  ui->closeButton->setFocusPolicy(Qt::NoFocus);
  connect(timer, &QTimer::timeout, this, &TetrisWidget::updateScreen);
  timer->start(10);
}

/**
 * @brief Деструктор класса SnakeWidget
 * @details Удаляет объекты UI и очищает память, выделенную для игры
 */
TetrisWidget::~TetrisWidget() {
  delete ui;
  removeGameInfo_t();
}

/**
 * @brief Обработчик события перерисовки виджета
 * @details Перерисовывает виджет в зависимости от текущего состояния игры.
 * Если игра приостановлена, то отображается соответствующий экран.
 * Если игра продолжается, то перерисовывается поле игры, отображаются текущая
 * фигура и следующая фигура в отдельном окне, обновляются статистика и счет, а
 * также вызывается механика игры. Если игра проиграна или выиграна, то
 * отображается соответствующий экран.
 * @param event Событие QPaintEvent, указывающее, что необходимо перерисовать
 * виджет
 */
void TetrisWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  GameInfo_t stats = updateCurrentState();
  TetrisInfo_t *game_state = getTetrisInfo_t();
  Figure_t *figure = &game_state->figure;
  ui->level->display(stats.level);
  ui->score->display(stats.score);
  ui->high_score->display(stats.high_score);
  updateField(&stats, figure, MOVING_CELL);
  drawCell(&painter, stats.field, 1, 21, 1, 11, 0, 0);
  updateField(&stats, figure, EMPTY_CELL);
  painter.setPen(QColor(0, 59, 0));
  painter.setBrush(QColor(0, 59, 0));
  painter.drawRect(240, 200, 178, 220);
  if (stats.pause) {
    pauseScreen(&painter);
  } else {
    if (game_state->game_status != kGameOver &&
        game_state->game_status != kWin) {
      timer->start(10);
      game_state->game_info = stats;
      drawCell(&painter, game_state->next_figure.f, 0,
               game_state->next_figure.height, 0, game_state->next_figure.width,
               13, 15);
      tetrisMechanics(game_state);
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
 * управления (стрелки Left, Right, Up, Down), то соответствующее
 * направление отправляется в обработчик команд. Если нажата клавиша p или P, то
 * игра приостанавливается. Если нажата клавиша q, Q или Escape, то игра
 * завершается
 * @param event Событие QKeyEvent, указывающее, какая клавиша была нажата
 */
void TetrisWidget::keyPressEvent(QKeyEvent *event) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  bool hold = false;
  int key = event->key();
  switch (key) {
  case Qt::Key_Space:
  case Qt::Key_Up:
    game_state->action = Action;
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
    game_state->action = Start;
    break;
  default:
    game_state->action = Up;
    break;
  }
  userInput(game_state->action, hold);
  updateScreen();
}

/**
 * @brief Обрабатывает событие нажатия кнопки HELP
 * @details Отображает окно сообщения с инструкциями по управлению игрой,
 * включая управление движением, приостановку / возобновление и другие игровые
 * механики. Если игра еще не приостановлена, программа приостанавливает ее
 * перед отображением справочной информации, а затем возобновляет игру при
 * закрытии окна справки
 */
void TetrisWidget::on_helpButton_clicked() {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  int on_pause = game_state->game_info.pause;
  if (!on_pause) {
    on_pauseButton_clicked();
  }
  QString help_info =
      "Arrange falling blocks to complete horizontal lines and prevent the "
      "stack from reaching the top!\n\n"
      "Movement (use arrow keys):\n"
      "- Left/Right Arrow Keys: Move the figure left or right\n"
      "- Up Arrow / Spacebar: Rotate the figure 90° clockwise\n"
      "- Down Arrow: Instantly drop the figure to the bottom\n\n"
      "Pause/Resume: Press P\n"
      "Quit: Press Q or Esc to exit\n\n"
      "Gameplay Mechanics:\n"
      "- Scoring: Earn points by clearing lines\n"
      "· 1 line - 100 points\n"
      "· 2 lines - 300 points\n"
      "· 3 lines - 700 points\n"
      "· 4 lines - 1500 points\n"
      "- Levels & Speed: Every 600 points increase your level by 1. Each level "
      "raises the falling speed of figures\n"
      "- Game Over: If a new figure cannot spawn due to lack of space\n\n"
      "Click OK to return to the game";
  QMessageBox::information(this, "Quick Guide", help_info);
  if (!on_pause) {
    userInput(Pause, false);
  }
  update();
}

/**
 * @brief Обрабатывает событие нажатия кнопки PAUSE
 * @details Приостанавливает игру, отправляя статус Pause в контроллер игры.
 * Затем контроллер отправляет действие паузы в paintEvent и интерфейс
 * обновляется для отображения нового состояния.
 */
void TetrisWidget::on_pauseButton_clicked() {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->action = Pause;
  userInput(Pause, false);
  update();
}
