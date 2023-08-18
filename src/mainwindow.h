#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <scp_glwidget.h>

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>

#include "scp_gifcreator.h"

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
  QSettings *settings;

 private slots:
  void scp_openFile();
  void scp_renderFile();
  void scp_projectionTypeChange(int idx);
  void scp_takeScreenshot();
  void scp_getGIF();
  void scp_affine();
  void scp_reset();
  void scp_reset_params();
  void scp_setBackgroundColor();
  void scp_setPolygonColor();
  void scp_solidPolygonType();
  void scp_dashedPolygonType();
  void scp_setPolygonThickness(int value);
  void scp_setNoneVertice();
  void scp_setCircleVertice();
  void scp_setSquareVertice();
  void scp_setVerticeSize(int value);
  void scp_setVerticeColor();
  void scp_saveSettings();
  void scp_loadSettings();
  void scp_setValuesOnButtons();
};
#endif  // MAINWINDOW_H
