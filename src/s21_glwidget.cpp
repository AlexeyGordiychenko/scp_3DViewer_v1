#include "s21_glwidget.h"

GLWidget::~GLWidget() {
  s21_free_obj_struct(this->data);
  free(this->data);
}

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void GLWidget::setFilename(char *filename) { this->filename = filename; }

void GLWidget::setProjectionType(int projectionType) {
  this->projectionType = projectionType;
}

void GLWidget::setDimentionalValues() {
  double x_min = this->data->viewbox[0], x_max = this->data->viewbox[1],
         y_min = this->data->viewbox[2], y_max = this->data->viewbox[3],
         z_min = this->data->viewbox[4], z_max = this->data->viewbox[5];

  this->centerX = (x_max + x_min) / 2;
  this->centerY = (y_max + y_min) / 2;
  this->centerZ = (z_max + z_min) / 2;
  double maxd =
      std::max({fabs(x_max - x_min), fabs(y_max - y_min), fabs(z_max - z_min)});
  if (maxd != 0) {
    this->sizeCoefficient = 2 / maxd;
  } else {
    this->sizeCoefficient = 0;
  }
}

void GLWidget::parseFile() {
  if (this->data != nullptr) {
    s21_free_obj_struct(this->data);
  } else {
    this->data = (s21_obj *)malloc(sizeof(s21_obj));
  }
  s21_parse_obj_file(this->filename, this->data);
  this->setDimentionalValues();

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

  if (this->data != nullptr) {
    if (this->projectionType == PARALLEL) {
      glOrtho(-1.5, 1.5, -1.5, 1.5, -2, 1000);
    } else {
      glFrustum(-1, 1, -1, 1, 1, 99999);
      glTranslatef(0, 0, -2.5);
    }
    glScalef(this->zoom, this->zoom, this->zoom);
    glTranslatef(this->xTrans, this->yTrans, 0.0);
    glRotatef(this->xRot / 4.0, 1.0, 0.0, 0.0);
    glRotatef(this->yRot / 4.0, 0.0, 1.0, 0.0);
    glRotatef(this->zRot / 4.0, 0.0, 0.0, 1.0);

    for (uint32_t i = 0; i < this->data->polygons_count; i++) {
      glBegin(GL_LINE_LOOP);
      glColor3f(0.0f, 1.0f, 0.0f);
      for (uint32_t j = 0; j < this->data->polygons[i].count; j++) {
        uint32_t vertex_index = this->data->polygons[i].vertexes[j];
        glVertex3d((this->data->matrix_3d->matrix[vertex_index - 1][0] -
                    this->centerX) *
                       this->sizeCoefficient,
                   (this->data->matrix_3d->matrix[vertex_index - 1][1] -
                    this->centerY) *
                       this->sizeCoefficient,
                   (this->data->matrix_3d->matrix[vertex_index - 1][2] -
                    this->centerZ) *
                       this->sizeCoefficient);
      }
      glEnd();
    }
    glFlush();
  }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  this->lastMousePos = event->position();
}

void normalizeAngle(double &angle) {
  while (angle < 0) angle += 360 * 16;
  while (angle > 360) angle -= 360 * 16;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  GLfloat dx =
      GLfloat(event->position().x() - this->lastMousePos.x()) / width();
  GLfloat dy =
      GLfloat(event->position().y() - this->lastMousePos.y()) / height();

  if (event->buttons() & Qt::LeftButton) {
    this->xRot += 360 * dy;
    this->yRot += 360 * dx;
    normalizeAngle(this->xRot);
    normalizeAngle(this->yRot);
  } else if (event->buttons() & Qt::RightButton) {
    this->xRot += 360 * dy;
    this->zRot += 360 * dx;
    normalizeAngle(this->xRot);
    normalizeAngle(this->zRot);
  } else if (event->buttons() & Qt::MiddleButton) {
    this->xTrans += dx;
    this->yTrans -= dy;
  }

  this->lastMousePos = event->position();
  update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  QPoint angleDelta = event->angleDelta();

  if (!angleDelta.isNull()) {
    int delta = angleDelta.y();

    if (delta > 0) {
      this->zoom *= 1.1;
    } else {
      this->zoom /= 1.1;
    }
    update();
  }
}
