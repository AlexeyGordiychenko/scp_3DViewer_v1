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
  s21_obj *data = NULL;
  uint32_t numVertices = 0, numEdges = 0;
  void setFilename(char *filename);
  void setProjectionType(int idx);
  void parseFile();

 private:
  ~GLWidget() override;
  int sizeH = 0, sizeW = 0, projectionType = 0;
  double centerX = 0, centerY = 0, centerZ = 0, sizeCoefficient = 0, xRot = 0,
         yRot = 0, zRot = 0, xTrans = 0, yTrans = 0, zoom = 1;
  QPointF lastMousePos;
  void setDimentionalValues();
  void countVerticesEdges();

 protected:
  // OPENGL
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
};

#endif
