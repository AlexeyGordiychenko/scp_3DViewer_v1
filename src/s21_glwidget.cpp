#include "s21_glwidget.h"

GLWidget::~GLWidget() { s21_free_obj_struct(this->data); }

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void GLWidget::setFilename(char *filename) { this->filename = filename; }

void GLWidget::setViewBoxCoef() {
  uint32_t rows = this->data->matrix_3d->rows;
  uint32_t cols = this->data->matrix_3d->cols;
  double **matrix = this->data->matrix_3d->matrix;
  GLdouble viewBoxCoef = matrix[0][0];
  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t j = 0; j < cols; j++) {
      if (abs(matrix[i][j]) > viewBoxCoef) viewBoxCoef = abs(matrix[i][j]);
    }
  }
  this->viewBoxCoef = viewBoxCoef;
}

void GLWidget::parseFile() {
  s21_obj *data = (s21_obj *)malloc(sizeof(s21_obj));
  s21_parse_obj_file(this->filename, data);
  this->data = data;
  this->test = true;
  this->setViewBoxCoef();

  update();
}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // temporary for a cube, replace with min/max x,y,z
  glOrtho(-1, 3, -1, 3, -3, 3);
  //  glFrustum(-1, 3, -1, 3, -3, 3);
  //  glTranslatef(0, 0, -1);
  //  glRotatef(30, 1, 0, 0);

  //  glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();
  if (this->data != nullptr) {
    for (uint32_t i = 0; i < this->data->polygons_count; i++) {
      glBegin(GL_LINE_LOOP);
      glColor3f(0.0f, 1.0f, 0.0f);
      for (uint32_t j = 0; j < this->data->polygons[i].count; j++) {
        uint32_t vertex_index = this->data->polygons[i].vertexes[j];
        glVertex3d(this->data->matrix_3d->matrix[vertex_index - 1][0],
                   this->data->matrix_3d->matrix[vertex_index - 1][1],
                   this->data->matrix_3d->matrix[vertex_index - 1][2]);
      }
      glEnd();
    }
    glFlush();
  }
}
