#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->openFile, SIGNAL(clicked()), this, SLOT(s21_openFile()));
  connect(ui->renderFile, SIGNAL(clicked()), this, SLOT(s21_renderFile()));
  connect(ui->projectionType, SIGNAL(currentIndexChanged(int)), this,
          SLOT(s21_projectionTypeChange(int)));
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
}

void MainWindow::s21_projectionTypeChange(int idx) {
  ui->openGLWidget->setProjectionType(idx);
  ui->openGLWidget->update();
}
