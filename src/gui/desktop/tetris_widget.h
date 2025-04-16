/** @file
 * @brief Заголовочный файл, определяющий функции отрисовки виджета Тетриса
 */
#ifndef TETRIS_WIDGET_H_
#define TETRIS_WIDGET_H_

#include "game_widget.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "../../brick_game/tetris/tetris_backend.h"
#ifdef __cplusplus
}
#endif

namespace Ui {
class TetrisWidget;
}

/** @class TetrisWidget
 * @brief Класс qt-представления игры Тетрис
 * @param GameWidget Родительский виджет
 */
class TetrisWidget : public GameWidget {
  Q_OBJECT

 public:
  explicit TetrisWidget(QWidget *parent = nullptr);
  ~TetrisWidget();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 private:
  Ui::TetrisWidget *ui;

 private slots:
  void on_helpButton_clicked();
  void on_pauseButton_clicked();
};

#endif  // TETRIS_WIDGET_H
