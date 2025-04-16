/** @file
 * @brief Заголовочный файл, определяющий функции фронтенда (qt-версии)
 */
#ifndef GAME_WIDGET_H_
#define GAME_WIDGET_H_

#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>
#include <QThread>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/common/common_specification.h"

/** @class GameWidget
 * @brief Класс qt-представления игры
 * @details Родительский класс для виджетов игры
 * @param parent Родительский виджет
 */
class GameWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GameWidget(QWidget *parent = nullptr);
  ~GameWidget();

 signals:
  void widgetClosed();

 protected:
  QTimer *timer;

  virtual void paintEvent(QPaintEvent *event) override;
  virtual void keyPressEvent(QKeyEvent *event) override;

  void updateScreen();
  void pauseScreen(QPainter *painter);
  void gameoverScreen(QPainter *painter);
  void winScreen(QPainter *painter);

  /**
   * @brief Рисует клетки на поле
   * @details Рисует ячейку на экране, учитывая матрицу поля (или следующей
   * фигуры в Тетрисе), ее координаты, смещение ячейки и размер ячейки в
   * пикселях
   * @param painter Указатель на объект, который используется для рисования
   * @param matrix Матрица игры
   * @param y Начальная координата по оси Y
   * @param max_y Максимальная координата по оси Y
   * @param x Начальная координата по оси X
   * @param max_x Максимальная координата по оси X
   * @param offset_y Смещение клетки по оси Y
   * @param offset_x Смещение клетки по оси X
   */
  template <typename MatrixType>
  void drawCell(QPainter *painter, MatrixType &matrix, int y, int max_y, int x,
                int max_x, int offset_y, int offset_x) {
    for (int i = y; i < max_y; ++i) {
      for (int j = x; j < max_x; ++j) {
        if (matrix[i][j] == MOVING_CELL || matrix[i][j] == STATIC_CELL) {
          painter->setBrush(QColor(0, 143, 17));
        } else {
          painter->setBrush(QColor(0, 59, 0));
        }
        painter->setPen(QColor(0, 59, 0));
        painter->drawRect(20 * (offset_x + j), 20 * (offset_y + i), 20, 20);
      }
    }
  }

 protected slots:
  void on_closeButton_clicked();
};

#endif  // GAME_WIDGET_H_
