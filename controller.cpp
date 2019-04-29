#include "controller.h"
#include "render.h"
#include "worker.h"

#include <QColor>
#include <QDebug>

Controller::Controller(std::vector<std::unique_ptr<Hitable>> sceneDescription,
                       const Options *opt) {
  qDebug() << "in controller\n";
  qDebug() << "opt in controller: " << opt->xRes
           << "\n controller scene size: " << sceneDescription.size();
  Render *render = new Render(*opt);
  render->setScene(std::move(sceneDescription));
  render->moveToThread(&_worker_thread);
  connect(&_worker_thread, &QThread::finished, render, &QObject::deleteLater);

  connect(this, &Controller::initializedWorkerRequested, render, &Render::init);
  connect(render, &Render::initialized, this, [=](bool success) {
    if (success) {
      qDebug() << "Worker thread initialized.";
    } else {
      qDebug() << "Worker thread failed to initialize.";
    }
  });

  connect(this, &Controller::startWorkerRequested, render, &Render::start);
  connect(render, &Render::started, this, [=](bool success) {
    if (success) {
      qDebug() << "Worker thread started\nStarting timer.";
      _timer.start();
    } else {
      qDebug() << "Worker thread is not stopped.";
    }
  });

  connect(this, &Controller::stopWorkerRequested, render, &Render::stop);
  connect(render, &Render::stopped, this, [=](bool success) {
    if (success) {
      qDebug() << "Worker thread stopped.\nStopping timer";
      _timer.stop();
    } else {
      qDebug() << "Worker thread is not stopped.";
    }
  });

  _timer.setTimerType(Qt::PreciseTimer);
  _timer.setSingleShot(true);
  //_timer.setInterval(100);
  connect(&_timer, &QTimer::timeout, render, &Render::generateData);
  connect(render, &Render::dataGenerated, this, &Controller::onDataGenerated);

  _worker_thread.start();
}

Controller::~Controller() {
  _worker_thread.quit();
  _worker_thread.wait();
}

void Controller::start() {
  emit initializedWorkerRequested();
  emit startWorkerRequested();
}

void Controller::stop() { emit stopWorkerRequested(); }

void Controller::onDataGenerated(const QImage &data) { emit passData(data); }
// void Controller::onDataGenerated(const QRgb &data, int x, int y) {
//  emit passData(data, x, y);
//}
