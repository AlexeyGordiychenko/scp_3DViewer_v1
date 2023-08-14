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
  connect(ui->reset_model, SIGNAL(clicked()), this, SLOT(s21_reset()));

  ui->projectionType->addItem("Parallel", PARALLEL);
  ui->projectionType->addItem("Central", CENTRAL);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::s21_openFile() {
  QString QString_filename = QFileDialog::getOpenFileName(
      this, tr("Open .obj file:"), "~/", tr("Obj Files (*.obj)"));
  ui->filePath->setText(QString_filename);
  ui->openGLWidget->fileChanged = true;
}

void MainWindow::s21_reset() {
  ui->openGLWidget->clearTransformations();
  ui->openGLWidget->reset();
  ui->openGLWidget->update();
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

void MainWindow::s21_affine() {
  double move_x = (ui->move_on_x->value());
  double move_y = (ui->move_on_y->value());
  double move_z = (ui->move_on_z->value());
  double scale_k = (ui->scale_on_k->value());
  double rotate_x = (ui->rotate_x->value());
  double rotate_y = (ui->rotate_y->value());
  double rotate_z = (ui->rotate_z->value());
  if (scale_k == 0) scale_k = 1;
  ui->openGLWidget->clearTransformations();
  ui->openGLWidget->scale(scale_k);
  ui->openGLWidget->move(move_x, move_y, move_z);
  ui->openGLWidget->rotate((rotate_x)*M_PI / 180, (rotate_y)*M_PI / 180,
                           (rotate_z)*M_PI / 180);
}
