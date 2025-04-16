/** @file
 * @brief Заголовочный файл, определяющий параметры для старта qt-версии игры
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "snake_widget.h"
#include "tetris_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_snakeButton_clicked();
  void on_tetrisButton_clicked();
  void on_closeButton_clicked();

 private:
  Ui::MainWindow *ui;
  SnakeWidget *snake_widget;
  TetrisWidget *tetris_widget;
};
#endif  // MAINWINDOW_H
