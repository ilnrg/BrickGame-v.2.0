/** @file
 * @brief Файл, содержащий функции для отрисовки десктопного интерфейса
 */
#include "game_widget.h"

/**
 * @brief Конструктор класса GameWidget
 * @details Этот класс является базовым для всех игровых виджетов. Он содержит
 * объект timer, который используется для обновления состояния игры и
 * перерисовки игровой сцены.
 * Конструктор также устанавливает политику фокусировки в Qt::StrongFocus, что
 * означает, что он будет получать все ключевые события, и устанавливает атрибут
 * Qt::WA_DeleteOnClose, который означает, что виджет будет удален при закрытии
 * @param parent Родительский виджет
 */
GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), timer(new QTimer(this)) {
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_DeleteOnClose, true);
}

/**
 * @brief Деструктор класса GameWidget
 * @details Этот деструктор удаляет объект timer, используемый для обновления
 * состояния игры и перерисовки игровой сцены
 */
GameWidget::~GameWidget() { delete timer; }

/**
 * @brief Отрисовывает игровой экран
 * @details Этот метод вызывается Qt, когда виджету необходимо перерисовать
 * себя. Он использует объект QPainter, чтобы отрисовать игровой экран
 * @param event Событие QPaintEvent, указывающее, что необходимо перерисовать
 * виджет
 */
void GameWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  winScreen(&painter);
}

/**
 * @brief Обработчик события нажатия клавиши
 * @details Он вызывает базовый обработчик QWidgets, а затем вызывает
 * updateScreen(), чтобы перерисовать игровой экран
 * @param event событие QKeyEvent, которое содержит информацию о нажатой
 * клавише
 */
void GameWidget::keyPressEvent(QKeyEvent *event) {
  QWidget::keyPressEvent(event);
  updateScreen();
}

/**
 * @brief Обновляет игровой экран
 * @details Этот метод вызывается для обновления состояния игры и перерисовки
 * игровой сцены. Он вызывает repaint(), чтобы перерисовать виджет
 */
void GameWidget::updateScreen() { repaint(); }

/**
 * @brief Отображает сообщение о паузе
 * @details Останавливает игровой таймер и с помощью QPainter рисует
 * полупрозрачный черный прямоугольник на игровом экране, отображает текст
 * "PAUSE" и инструкции по возобновлению игры
 * @param painter Указатель на объект QPainter
 */
void GameWidget::pauseScreen(QPainter *painter) {
  timer->stop();
  painter->setBrush(QColor(0, 0, 0, 127));
  painter->setPen(Qt::NoPen);
  painter->drawRect(rect());
  painter->setPen(QColor(255, 255, 255));
  painter->setFont(QFont("Arial", 20, QFont::Bold));
  painter->drawText(85, 200, "PAUSE");
  painter->setFont(QFont("Arial", 14, QFont::Bold));
  painter->drawText(57, 240, "Press P to resume");
}

/**
 * @brief Отображает сообщение о проигрыше
 * @details Этот метод вызывается, когда игра окончена (проигрыш или желание
 * игрока остановить игру). Он рисует полупрозрачный черный прямоугольник на
 * игровом экране, отображает текст "GAME OVER(" и инструкции по выходу из игры
 * @param painter Указатель на объект QPainter
 */
void GameWidget::gameoverScreen(QPainter *painter) {
  painter->setBrush(QColor(0, 0, 0, 127));
  painter->setPen(Qt::NoPen);
  painter->drawRect(rect());
  painter->setPen(QColor(255, 255, 255));
  painter->setFont(QFont("Arial", 20, QFont::Bold));
  painter->drawText(55, 200, "GAME OVER(");
  painter->setFont(QFont("Arial", 14, QFont::Bold));
  painter->drawText(35, 220, "GOOD LUCK NEXT TIME!");
  painter->setFont(QFont("Arial", 14, QFont::Bold));
  painter->drawText(30, 260, "Click QUIT to exit the game");
}

/**
 * @brief Отображает сообщение о победе
 * @details Этот метод вызывается, когда игра окончена и игрок выиграл. Он
 * рисует полупрозрачный черный прямоугольник на игровом экране, отображает
 * текст "YOU COMPLETED THE GAME!" и "WELL DONE!" и инструкции по выходу из
 * игры
 * @param painter Указатель на объект QPainter
 */
void GameWidget::winScreen(QPainter *painter) {
  painter->setBrush(QColor(0, 0, 0, 127));
  painter->setPen(Qt::NoPen);
  painter->drawRect(rect());
  painter->setPen(QColor(255, 255, 255));
  painter->setFont(QFont("Arial", 20, QFont::Bold));
  painter->drawText(30, 200, "YOU COMPLETED");
  painter->setFont(QFont("Arial", 20, QFont::Bold));
  painter->drawText(65, 220, "THE GAME!");
  painter->setFont(QFont("Arial", 20, QFont::Bold));
  painter->drawText(55, 240, "WELL DONE!");
  painter->setFont(QFont("Arial", 14, QFont::Bold));
  painter->drawText(30, 280, "Click QUIT to exit the game");
}

/**
 * @brief Обработчик события нажатия кнопки закрытия
 * @details Когда пользователь нажимает кнопку закрытия, он вызывает
 * сигнал widgetClosed() и закрывает виджет
 */
void GameWidget::on_closeButton_clicked() {
  emit widgetClosed();
  this->close();
}
