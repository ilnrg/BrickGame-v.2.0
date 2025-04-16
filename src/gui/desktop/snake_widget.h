/** @file
 * @brief Заголовочный файл, определяющий функции отрисовки виджета Змейки
 */
#ifndef SNAKE_WIDGET_H_
#define SNAKE_WIDGET_H_

#include "game_widget.h"
#include "../../brick_game/snake/snake_controller.h"

namespace Ui {
class SnakeWidget;
}

/** @class SnakeWidget
 * @brief Класс qt-представления игры Змейка
 * @param GameWidget Родительский виджет
 */
class SnakeWidget : public GameWidget {
  Q_OBJECT

 public:
  explicit SnakeWidget(QWidget *parent = nullptr);
  ~SnakeWidget();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

 private:
  Ui::SnakeWidget *ui;
  s21::SnakeModel snake_model_;
  s21::SnakeController snake_controller_;

 private slots:
  void on_helpButton_clicked();
  void on_pauseButton_clicked();
};

#endif  // SNAKE_WIDGET_H_
