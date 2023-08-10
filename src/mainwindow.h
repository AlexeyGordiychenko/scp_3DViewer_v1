#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <s21_glwidget.h>

#include <QFileDialog>
#include <QMainWindow>

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

 private:
  Ui::MainWindow *ui;

 private slots:
  void s21_openFile();
  void s21_renderFile();
};
#endif  // MAINWINDOW_H
