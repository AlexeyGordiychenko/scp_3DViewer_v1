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
  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(s21_affine()));

  ui->projectionType->addItem("Parallel", PARALLEL);
  ui->projectionType->addItem("Central", CENTRAL);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::s21_openFile() {
  QString QString_filename = QFileDialog::getOpenFileName(
      this, tr("Open .obj file:"), "~/", tr("Obj Files (*.obj)"));
  ui->filePath->setText(QString_filename);
}

void MainWindow::s21_renderFile() {
  std::string std_filename = ui->filePath->text().toStdString();
  ui->openGLWidget->setFilename((char *)std_filename.c_str());
  ui->openGLWidget->parseFile();
  ui->numVertices->setText(QString::number(ui->openGLWidget->numVertices));
  ui->numEdges->setText(QString::number(ui->openGLWidget->numEdges));
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

void MainWindow::scale(double k) {
  s21_scale(ui->openGLWidget->data->matrix_3d, k);
}

void MainWindow::move(double x, double y, double z) {
  s21_xyz_movement(ui->openGLWidget->data->matrix_3d, x, y, z);
}

void MainWindow::rotate(double angle_x, double angle_y, double angle_z) {
  s21_rotation_by_ox(ui->openGLWidget->data->matrix_3d, angle_x);
  s21_rotation_by_oy(ui->openGLWidget->data->matrix_3d, angle_y);
  s21_rotation_by_oz(ui->openGLWidget->data->matrix_3d, angle_z);
}

void MainWindow::s21_affine() {
  double move_x = (ui->move_on_x->value());
  double move_y = (ui->move_on_y->value());
  double move_z = (ui->move_on_z->value());
  double scale_k = (ui->scale_on_k->value());
  double rotate_x = (ui->rotate_x->value());
  double rotate_y = (ui->rotate_y->value());
  double rotate_z = (ui->rotate_z->value());
  if (scale_k == 0) scale_k = 1;
  if (scale_k_old == 0) scale_k_old = 1;
  scale(scale_k / scale_k_old);
  move(move_x - move_x_old, move_y - move_y_old, move_z - move_z_old);
  rotate((rotate_x - rotate_x_old) * M_PI / 180,
         (rotate_y - rotate_y_old) * M_PI / 180,
         (rotate_z - rotate_z_old) * M_PI / 180);
  ui->openGLWidget->update();
  scale_k_old = scale_k;
  move_x_old = move_x;
  move_y_old = move_y;
  move_z_old = move_z;
  rotate_x_old = rotate_x;
  rotate_y_old = rotate_y;
  rotate_z_old = rotate_z;
}
