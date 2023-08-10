#ifndef S21_GLWIDGET_H
#define S21_GLWIDGET_H

#define GL_SILENCE_DEPRECATION
#include <GL/gl.h>
#include <GL/glut.h>

#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>
#include <iostream>

extern "C" {
#include "s21_viewer.h"
}

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  explicit GLWidget(QWidget *parent = Q_NULLPTR);
  char *filename;
  void setFilename(char *filename);
  void parseFile();

 private:
  ~GLWidget() override;
  s21_obj *data = NULL;
  GLdouble viewBoxCoef = 0;
  bool test = false;
  void setViewBoxCoef();

 protected:
  // OPENGL
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
};

#endif
