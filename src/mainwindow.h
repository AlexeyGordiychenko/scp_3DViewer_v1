#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <s21_glwidget.h>

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>

#include "s21_gifcreator.h"

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
  GifCreator *gifCreator;
  bool check_render = false;

 private slots:
  void s21_openFile();
  void s21_renderFile();
  void s21_projectionTypeChange(int idx);
  void s21_takeScreenshot();
  void s21_getGIF();
  void s21_affine();
  void s21_reset();
};
#endif  // MAINWINDOW_H
