/** @file
 * @brief Файл, устанавливающий точку входа в программу и инициализирующий
 * начальные состояния
 */
#include "mainwindow.h"

#include "ui_mainwindow.h"

/**
 * @brief Конструктор класса MainWindow.
 * @details Инициализирует главное окно с фиксированным размером 400x400
 * пикселей и задает заголовку окна значение "BrickGame". Настраивает
 * пользовательский интерфейс
 * @param parent Родительский виджет
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(400, 400);
  setWindowTitle("BrickGame");
}

MainWindow::~MainWindow() { delete ui; }

/**
 * @brief Слот, вызываемый при клике на кнопку "Snake".
 * @details Скрывает главное окно, создает объект SnakeWidget, настраивает
 * сигнал-слот, где сигналом является закрытие SnakeWidget, а слот - отображение
 * главного окна, а затем отображает SnakeWidget
 */
void MainWindow::on_snakeButton_clicked() {
  hide();
  snake_widget = new SnakeWidget;
  connect(snake_widget, &SnakeWidget::widgetClosed, this, &MainWindow::show);
  snake_widget->show();
}

/**
 * @brief Слот, вызываемый при клике на кнопку "Tetris".
 * @details Скрывает главное окно, создает объект TetrisWidget, настраивает
 * сигнал-слот, где сигналом является закрытие TetrisWidget, а слот -
 * отображение главного окна, а затем отображает TetrisWidget
 */
void MainWindow::on_tetrisButton_clicked() {
  hide();
  tetris_widget = new TetrisWidget;
  connect(tetris_widget, &TetrisWidget::widgetClosed, this, &MainWindow::show);
  tetris_widget->show();
}

/**
 * @brief Слот, вызываемый при клике на кнопку "Close".
 * @details Спрашивает у пользователя, действительно ли он хочет закрыть
 * приложение. Если пользователь подтверждает, то закрывает главное окно
 */
void MainWindow::on_closeButton_clicked() {
  QMessageBox::StandardButton reply =
      QMessageBox::question(this, "Quit", "Do you really want to quit?",
                            QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    close();
  }
}
