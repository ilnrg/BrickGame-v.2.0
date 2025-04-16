/** @file
 * @brief Файл, устанавливающий точку входа в программу и запускающий главное
 * окно
 */
#include <QApplication>

#include "mainwindow.h"

/**
 * @brief Начало программы.
 * @details Эта функция является точкой входа в приложение. Она создает
 * экземпляр приложения, создает главное окно и отображает его
 */
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow window;
  window.show();
  return a.exec();
}
