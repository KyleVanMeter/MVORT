#include "worker.h"

#include <QDebug>
#include <QtCore>
#include <QColor>

Worker::Worker() { qDebug() << "Worker created"; }

Worker::~Worker() { qDebug() << "Worker destroyed"; }

void Worker::init() {
  bool status = true;

  emit initialized(status);
}

void Worker::start() {
  bool status = true;

  emit started(status);
}

void Worker::stop() {
  bool status = true;

  emit stopped(status);
}

void Worker::generateData() {
  int a = QRandomGenerator::global()->bounded(255);
  int b = QRandomGenerator::global()->bounded(255);
  int c = QRandomGenerator::global()->bounded(255);
  QRgb result = qRgb(a, b, c);

  emit dataGenerated(result, 0, 0);
}
