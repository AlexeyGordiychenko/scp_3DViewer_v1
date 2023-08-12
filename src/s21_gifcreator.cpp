#include "s21_gifcreator.h"

GifCreator::~GifCreator() {}

GifCreator::GifCreator(QOpenGLWidget *widget, const QString &outputGifPath,
                       int fps, int duration_sec, QObject *parent)
    : QObject(parent),
      widget(widget),
      outputGifPath(outputGifPath),
      frameCount(0),
      frameMax(fps * duration_sec),
      frameDelay(1000 / fps) {
  connect(&timer, &QTimer::timeout, this, &GifCreator::captureFrame);
}

void GifCreator::createGif() {
  QImage firstFrame = widget->grabFramebuffer();
  gifAnim.GifBegin(&gifWriter, outputGifPath.toStdString().c_str(),
                   firstFrame.width(), firstFrame.height(), frameDelay);
  gifAnim.GifWriteFrame(&gifWriter, firstFrame.bits(), firstFrame.width(),
                        firstFrame.height(), frameDelay);
  frameCount++;
  timer.start(frameDelay);
}

void GifCreator::captureFrame() {
  if (frameCount >= frameMax) {
    endGif();
    return;
  }

  QImage frame = widget->grabFramebuffer();
  gifAnim.GifWriteFrame(&gifWriter, frame.bits(), frame.width(), frame.height(),
                        frameDelay / 10);
  frameCount++;
}

void GifCreator::endGif() {
  timer.stop();
  gifAnim.GifEnd(&gifWriter);
  delete this;
  QMessageBox messageBoxImage;
  messageBoxImage.information(0, "", "GIF animation saved successfully.");
}
