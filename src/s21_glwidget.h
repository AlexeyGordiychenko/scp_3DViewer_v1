#ifndef S21_GLWIDGET_H
#define S21_GLWIDGET_H

//#define GL_SILENCE_DEPRECATION
//#include <GL/gl.h>
//#include <GL/glut.h>

#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>
#include <iostream>

extern "C" {
#include "s21_viewer.h"
}

enum s21_projectionType { PARALLEL, CENTRAL };

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  explicit GLWidget(QWidget *parent = Q_NULLPTR);
  char *filename;
  void setFilename(char *filename);
  void setProjectionType(int idx);
  void parseFile();

 private:
  ~GLWidget() override;
  s21_obj *data = NULL;
  int sizeH = 0, sizeW = 0, projectionType = 0;
  double centerX, centerY, centerZ, sizeCoefficient;
  void setDimentionalValues();

 protected:
  // OPENGL
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
};

#endif
