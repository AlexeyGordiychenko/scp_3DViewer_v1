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
  double move_x_old = 0, move_y_old = 0, move_z_old = 0, scale_k_old = 1,
         rotate_x_old = 0, rotate_y_old = 0, rotate_z_old = 0;

 private slots:
  void s21_openFile();
  void s21_renderFile();
  void s21_projectionTypeChange(int idx);
  void s21_takeScreenshot();
  void s21_getGIF();
  void scale(double scale);
  void move(double x, double y, double z);
  void rotate(double angle_x, double angle_y, double angle_z);
  void s21_affine();
};
#endif  // MAINWINDOW_H
