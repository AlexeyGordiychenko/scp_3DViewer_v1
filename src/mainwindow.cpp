#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->openFile, SIGNAL(clicked()), this, SLOT(s21_openFile()));
  connect(ui->renderFile, SIGNAL(clicked()), this, SLOT(s21_renderFile()));
  connect(ui->projectionType, SIGNAL(currentIndexChanged(int)), this,
          SLOT(s21_projectionTypeChange(int)));
  connect(ui->getScreenshot, SIGNAL(clicked()), this,
          SLOT(s21_takeScreenshot()));
  connect(ui->getGIF, SIGNAL(clicked()), this, SLOT(s21_getGIF()));
  connect(ui->apply_params, SIGNAL(clicked()), this, SLOT(s21_affine()));
  connect(ui->reset_model, SIGNAL(clicked()), this, SLOT(s21_reset()));
  connect(ui->reset_params, SIGNAL(clicked()), this, SLOT(s21_reset_params()));
  connect(ui->setBgColor, SIGNAL(clicked()), this,
          SLOT(s21_setBackgroundColor()));
  connect(ui->setPolygonColor, SIGNAL(clicked()), this,
          SLOT(s21_setPolygonColor()));
  connect(ui->solidPolygonType, SIGNAL(clicked()), this,
          SLOT(s21_solidPolygonType()));
  connect(ui->dashedPolygonType, SIGNAL(clicked()), this,
          SLOT(s21_dashedPolygonType()));
  connect(ui->polygonThickness, SIGNAL(valueChanged(int)), this,
          SLOT(s21_setPolygonThickness(int)));
  connect(ui->noneVertice, SIGNAL(clicked()), this, SLOT(s21_setNoneVertice()));
  connect(ui->circleVertice, SIGNAL(clicked()), this,
          SLOT(s21_setCircleVertice()));
  connect(ui->squareVertice, SIGNAL(clicked()), this,
          SLOT(s21_setSquareVertice()));
  connect(ui->sizeVertice, SIGNAL(valueChanged(int)), this,
          SLOT(s21_setVerticeSize(int)));
  connect(ui->setVerticeColor, SIGNAL(clicked()), this,
          SLOT(s21_setVerticeColor()));

  ui->projectionType->addItem("Parallel", PARALLEL);
  ui->projectionType->addItem("Central", CENTRAL);

  ui->filePath->setReadOnly(true);

  settings = new QSettings("21school", "3DViewer_v1.0", this);
  s21_loadSettings();
  s21_setValuesOnButtons();
}

MainWindow::~MainWindow() {
  s21_saveSettings();
  delete settings;
  delete ui;
}

void MainWindow::s21_openFile() {
  QString QString_filename = QFileDialog::getOpenFileName(
      this, tr("Open .obj file:"), "~/", tr("Obj Files (*.obj)"));
  ui->filePath->setText(QString_filename);
  ui->openGLWidget->fileChanged = true;
}

void MainWindow::s21_reset() {
  if (ui->openGLWidget->isParsed && !ui->openGLWidget->fileChanged) {
    ui->openGLWidget->clearTransformations();
    ui->openGLWidget->matrix_reset_to_start();
    ui->openGLWidget->update();
  }
}

void MainWindow::s21_renderFile() {
  if (ui->openGLWidget->fileChanged) {
    std::string std_filename = ui->filePath->text().toStdString();
    ui->openGLWidget->setFilename((char *)std_filename.c_str());
    int res = ui->openGLWidget->parseFile();
    if (res == S21_OK) {
      ui->numVertices->setText(QString::number(ui->openGLWidget->numVertices));
      ui->numEdges->setText(QString::number(ui->openGLWidget->numEdges));
    } else {
      QMessageBox messageBoxImage;
      if (res == S21_MEM) {
        messageBoxImage.information(0, "",
                                    "Couldn't render the file, out of memory.");
      } else if (res == S21_ERR) {
        messageBoxImage.information(0, "",
                                    "Corrupted file or incorrect format.");
      }
    }
    ui->openGLWidget->fileChanged = false;
  } else {
    ui->openGLWidget->clearTransformations();
    ui->openGLWidget->update();
  }
}

void MainWindow::s21_projectionTypeChange(int idx) {
  ui->openGLWidget->setProjectionType(idx);
  ui->openGLWidget->update();
}

void MainWindow::s21_takeScreenshot() {
  const QString suffixJpeg = ".jpeg", suffixBmp = ".bmp",
                filterJpeg = "JPEG Image (*." + suffixJpeg + ")",
                filterBmp = "Bitmap Image (*." + suffixBmp + ")";
  QFileInfo fileInfo(ui->filePath->text());
  QFileDialog saveImageDialog(this);
  QString saveFilename =
      fileInfo.baseName() + " render " +
      QDateTime::currentDateTime().toString("yyyy-MM-dd hh.mm.ss") + suffixJpeg;
  QString selectedFilter;
  QString image_name = saveImageDialog.getSaveFileName(
      this, "Screenshot saving", saveFilename, filterJpeg + ";;" + filterBmp,
      &selectedFilter);
  if (image_name.length() > 0) {
    if (!image_name.endsWith(suffixJpeg, Qt::CaseInsensitive) &&
        !image_name.endsWith(suffixBmp, Qt::CaseInsensitive)) {
      if (selectedFilter == filterJpeg) {
        image_name += suffixJpeg;
      } else {
        image_name += suffixBmp;
      }
    }
    QImage img = ui->openGLWidget->grabFramebuffer();
    img.save(image_name);
    QMessageBox messageBoxImage;
    messageBoxImage.information(0, "", "Screenshot saved successfully.");
  }
}

void MainWindow::s21_getGIF() {
  const QString gifExt = ".gif";
  QFileInfo fileInfo(ui->filePath->text());
  QFileDialog saveGifDialog(this);
  QString saveFilename =
      fileInfo.baseName() + " anim " +
      QDateTime::currentDateTime().toString("yyyy-MM-dd hh.mm.ss") + gifExt;
  QString gifName =
      saveGifDialog.getSaveFileName(this, "GIF saving", saveFilename, gifExt);
  if (gifName.length() > 0) {
    GifCreator *gifCreator = new GifCreator(ui->openGLWidget, gifName);
    gifCreator->createGif();
  }
}

void MainWindow::s21_reset_params() {
  ui->scale_on_k->setValue(1);
  ui->move_on_x->setValue(0);
  ui->move_on_y->setValue(0);
  ui->move_on_z->setValue(0);
  ui->rotate_x->setValue(0);
  ui->rotate_y->setValue(0);
  ui->rotate_z->setValue(0);
}

void MainWindow::s21_affine() {
  if (ui->openGLWidget->isParsed && !ui->openGLWidget->fileChanged) {
    double move_x = (ui->move_on_x->value());
    double move_y = (ui->move_on_y->value());
    double move_z = (ui->move_on_z->value());
    double scale_k = (ui->scale_on_k->value());
    double rotate_x = (ui->rotate_x->value());
    double rotate_y = (ui->rotate_y->value());
    double rotate_z = (ui->rotate_z->value());
    if (scale_k == 0) scale_k = 1;
    ui->openGLWidget->clearTransformations();
    ui->openGLWidget->matrix_reset_to_start();
    ui->openGLWidget->scale(scale_k);
    ui->openGLWidget->move(move_x, move_y, move_z);
    ui->openGLWidget->rotate((rotate_x)*M_PI / 180, (rotate_y)*M_PI / 180,
                             (rotate_z)*M_PI / 180);
    ui->openGLWidget->update();
  }
}

void MainWindow::s21_setBackgroundColor() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    ui->openGLWidget->bg_red = color.redF();
    ui->openGLWidget->bg_green = color.greenF();
    ui->openGLWidget->bg_blue = color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", color.red(),
            color.green(), color.blue());
    ui->setBgColor->setStyleSheet(rgba_color);
    ui->openGLWidget->update();
  }
}

void MainWindow::s21_setPolygonColor() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    ui->openGLWidget->pol_red = color.redF();
    ui->openGLWidget->pol_green = color.greenF();
    ui->openGLWidget->pol_blue = color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", color.red(),
            color.green(), color.blue());
    ui->setPolygonColor->setStyleSheet(rgba_color);
    ui->openGLWidget->update();
  }
}

void MainWindow::s21_solidPolygonType() {
  ui->openGLWidget->edges_type = SOLID;
  ui->openGLWidget->update();
}

void MainWindow::s21_dashedPolygonType() {
  ui->openGLWidget->edges_type = DASHED;
  ui->openGLWidget->update();
}

void MainWindow::s21_setPolygonThickness(int value) {
  ui->openGLWidget->edges_thickness = value / 10;
  ui->openGLWidget->update();
}

void MainWindow::s21_setNoneVertice() {
  ui->openGLWidget->vertice_type = NONE;
  ui->openGLWidget->update();
}

void MainWindow::s21_setCircleVertice() {
  ui->openGLWidget->vertice_type = CIRCLE;
  ui->openGLWidget->update();
}

void MainWindow::s21_setSquareVertice() {
  ui->openGLWidget->vertice_type = SQUARE;
  ui->openGLWidget->update();
}

void MainWindow::s21_setVerticeSize(int value) {
  ui->openGLWidget->vertice_size = value / 5;
  ui->openGLWidget->update();
}

void MainWindow::s21_setVerticeColor() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    ui->openGLWidget->ver_red = color.redF();
    ui->openGLWidget->ver_green = color.greenF();
    ui->openGLWidget->ver_blue = color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", color.red(),
            color.green(), color.blue());
    ui->setVerticeColor->setStyleSheet(rgba_color);
    ui->openGLWidget->update();
  }
}

void MainWindow::s21_saveSettings() {
  settings->setValue("bg_red", ui->openGLWidget->bg_red);
  settings->setValue("bg_green", ui->openGLWidget->bg_green);
  settings->setValue("bg_blue", ui->openGLWidget->bg_blue);
  settings->setValue("pol_red", ui->openGLWidget->pol_red);
  settings->setValue("pol_green", ui->openGLWidget->pol_green);
  settings->setValue("pol_blue", ui->openGLWidget->pol_blue);
  settings->setValue("edges_type", ui->openGLWidget->edges_type);
  settings->setValue("edges_thickness", ui->openGLWidget->edges_thickness);
  settings->setValue("vertice_type", ui->openGLWidget->vertice_type);
  settings->setValue("vertice_size", ui->openGLWidget->vertice_size);
  settings->setValue("ver_red", ui->openGLWidget->ver_red);
  settings->setValue("ver_green", ui->openGLWidget->ver_green);
  settings->setValue("ver_blue", ui->openGLWidget->ver_blue);
  settings->setValue("projectionType", ui->openGLWidget->projectionType);
}

void MainWindow::s21_loadSettings() {
  ui->openGLWidget->bg_red = settings->value("bg_red").toDouble();
  ui->openGLWidget->bg_green = settings->value("bg_green").toDouble();
  ui->openGLWidget->bg_blue = settings->value("bg_blue").toDouble();
  ui->openGLWidget->pol_red = settings->value("pol_red").toDouble();
  if (settings->contains("pol_green")) {
    ui->openGLWidget->pol_green = settings->value("pol_green").toDouble();
  } else {
    ui->openGLWidget->pol_green = 1;
  }
  ui->openGLWidget->pol_blue = settings->value("pol_blue").toDouble();
  ui->openGLWidget->edges_type = settings->value("edges_type").toInt();
  ui->openGLWidget->edges_thickness =
      settings->value("edges_thickness").toDouble();
  ui->openGLWidget->vertice_type = settings->value("vertice_type").toInt();
  ui->openGLWidget->vertice_size = settings->value("vertice_size").toInt();
  ui->openGLWidget->ver_red = settings->value("ver_red").toDouble();
  ui->openGLWidget->ver_green = settings->value("ver_green").toDouble();
  if (settings->contains("ver_blue")) {
    ui->openGLWidget->ver_blue = settings->value("ver_blue").toDouble();
  } else {
    ui->openGLWidget->ver_blue = 1;
  }
  ui->openGLWidget->projectionType = settings->value("projectionType").toInt();
  ui->openGLWidget->update();
}

void MainWindow::s21_setValuesOnButtons() {
  char bg_color[40], pol_color[40], ver_color[40];
  sprintf(bg_color, "background-color: rgb(%d,%d,%d)",
          (int)(ui->openGLWidget->bg_red * 255),
          (int)(ui->openGLWidget->bg_green * 255),
          (int)(ui->openGLWidget->bg_blue * 255));
  ui->setBgColor->setStyleSheet(bg_color);
  sprintf(pol_color, "background-color: rgb(%d,%d,%d)",
          (int)(ui->openGLWidget->pol_red * 255),
          (int)(ui->openGLWidget->pol_green * 255),
          (int)(ui->openGLWidget->pol_blue * 255));
  ui->setPolygonColor->setStyleSheet(pol_color);
  sprintf(ver_color, "background-color: rgb(%d,%d,%d)",
          (int)(ui->openGLWidget->ver_red * 255),
          (int)(ui->openGLWidget->ver_green * 255),
          (int)(ui->openGLWidget->ver_blue * 255));
  ui->setVerticeColor->setStyleSheet(ver_color);
  if (ui->openGLWidget->edges_type == SOLID) {
    ui->solidPolygonType->setChecked(true);
  } else {
    ui->dashedPolygonType->setChecked(true);
  }
  if (ui->openGLWidget->vertice_type == NONE) {
    ui->noneVertice->setChecked(true);
  } else if (ui->openGLWidget->vertice_type == CIRCLE) {
    ui->circleVertice->setChecked(true);
  } else {
    ui->squareVertice->setChecked(true);
  }
  ui->polygonThickness->setValue(settings->value("edges_thickness").toInt() *
                                 10);
  ui->sizeVertice->setValue(settings->value("vertice_size").toInt() * 5);
  if (ui->openGLWidget->projectionType == PARALLEL) {
    ui->projectionType->setCurrentIndex(0);
  } else {
    ui->projectionType->setCurrentIndex(1);
  }
}
