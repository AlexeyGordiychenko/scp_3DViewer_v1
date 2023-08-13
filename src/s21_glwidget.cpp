#include "s21_glwidget.h"

GLWidget::~GLWidget() {
  s21_free_obj_struct(this->data);
  free(this->data);
}

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  this->data = (s21_obj *)calloc(1, sizeof(s21_obj));
}

void GLWidget::setFilename(char *filename) { this->filename = filename; }

void GLWidget::setProjectionType(int projectionType) {
  this->projectionType = projectionType;
}

void GLWidget::scale(double k) { s21_scale(this->data->matrix_3d, k); }

void GLWidget::move(double x, double y, double z) {
  s21_xyz_movement(this->data->matrix_3d, x, y, z);
}

void GLWidget::rotate(double angle_x, double angle_y, double angle_z) {
  printf("%lf %lf %lf\n", angle_x, angle_y, angle_z);
  s21_rotation_by_ox(this->data->matrix_3d, angle_x);
  s21_rotation_by_oy(this->data->matrix_3d, angle_y);
  s21_rotation_by_oz(this->data->matrix_3d, angle_z);
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

void GLWidget::countVerticesEdges() {
  if (this->data != nullptr) {
    this->numVertices = this->data->matrix_3d->rows;
    this->numEdges = 0;
    for (uint32_t i = 0; i < data->polygons_count; i++) {
      this->numEdges += data->polygons[i].count;
    }
  }
}

void GLWidget::clearTransformations() {
  this->xRot = 0, this->yRot = 0, this->zRot = 0, this->xTrans = 0,
  this->yTrans = 0, this->zoom = 1;
}

int GLWidget::parseFile() {
  if (this->fileChanged && this->isParsed) s21_free_obj_struct(this->data);
  this->isParsed = false;
  this->clearTransformations();
  int res = s21_parse_obj_file(this->filename, this->data);
  if (res == S21_OK) {
    this->isParsed = true;
    this->setDimentionalValues();
    this->countVerticesEdges();
    update();
  }
  return res;
}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h) {
  this->sizeH = h;
  this->sizeW = w;
  glViewport(0, 0, w, h);
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  static double aspect_ratio =
      static_cast<double>(this->sizeW) / static_cast<double>(this->sizeH);

  if (this->isParsed) {
    if (this->projectionType == PARALLEL) {
      glOrtho(-1.5 * aspect_ratio, 1.5 * aspect_ratio, -1.5, 1.5, -2, 1000);
    } else {
      glFrustum(-1 * aspect_ratio, 1 * aspect_ratio, -1, 1, 1, 99999);
      glTranslatef(0, 0, -2.5);
    }
    glScalef(this->zoom, this->zoom, this->zoom);
    glTranslatef(this->xTrans, this->yTrans, 0.0);
    glRotatef(this->xRot, 1.0, 0.0, 0.0);
    glRotatef(this->yRot, 0.0, 1.0, 0.0);
    glRotatef(this->zRot, 0.0, 0.0, 1.0);

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
      GLfloat(event->position().x() - this->lastMousePos.x()) / this->sizeW;
  GLfloat dy =
      GLfloat(event->position().y() - this->lastMousePos.y()) / this->sizeH;

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
