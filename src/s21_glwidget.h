#ifndef S21_GLWIDGET_H
#define S21_GLWIDGET_H

// #define GL_SILENCE_DEPRECATION
// #include <GL/gl.h>
// #include <GL/glut.h>

#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>
#include <iostream>

extern "C" {
#include "s21_viewer.h"
}

enum s21_projectionType { PARALLEL, CENTRAL };
enum s21_polygonType { SOLID, DASHED };
enum s21_verticeType { NONE, CIRCLE, SQUARE };

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  explicit GLWidget(QWidget *parent = Q_NULLPTR);
  uint32_t numVertices = 0, numEdges = 0;
  bool isParsed = false, fileChanged = true;
  void setFilename(char *filename);
  void setProjectionType(int idx);
  int parseFile();
  void matrix_reset_to_start();
  void scale(double scale);
  void move(double x, double y, double z);
  void rotate(double angle_x, double angle_y, double angle_z);
  void clearTransformations();
  double bg_red = 0, bg_green = 0, bg_blue = 0;
  double pol_red = 0, pol_green = 0, pol_blue = 0;
  double ver_red = 0, ver_green = 0, ver_blue = 0;
  double edges_thickness = 1, vertice_size = 1;
  int vertice_type = 0, projectionType = 0, edges_type = 0;

 private:
  ~GLWidget() override;
  char *filename;
  int sizeH = 0, sizeW = 0;
  double centerX = 0, centerY = 0, centerZ = 0, sizeCoefficient = 0, xRot = 0,
         yRot = 0, zRot = 0, xTrans = 0, yTrans = 0, zoom = 1;
  s21_obj *data = NULL;
  s21_matrix_t *matrix_start = NULL;
  QPointF lastMousePos;
  void setDimentionalValues();
  void countVerticesEdges();
  void translateToFromOrigin(int k);

 protected:
  // OPENGL
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void setVertices(uint32_t i);
};

#endif
