#include "appmain.h"
#include "options.h"
#include <QDebug>
#include <QDialogButtonBox>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGuiApplication>
#include <QPushButton>
#include <QProgressBar>

#include <iostream>

AppMain::AppMain(std::vector<std::unique_ptr<Hitable>> sceneDescription,
                 Options *opt, QWidget *parent = nullptr)
    : _controller(std::move(sceneDescription), opt) {
  qDebug() << "in AppMain\n opt res: " << opt->xRes << "x" << opt->yRes
           << "\n appmain scene size: " << sceneDescription.size() << "\n";
  // resize(opt->xRes, opt->yRes);

  auto button_box = new QDialogButtonBox;
  _start_btn = new QPushButton(tr("Start"));
  _stop_btn = new QPushButton(tr("Stop"));
  button_box->addButton(_start_btn, QDialogButtonBox::ActionRole);
  button_box->addButton(_stop_btn, QDialogButtonBox::RejectRole);

  connect(_start_btn, &QAbstractButton::clicked, this, &AppMain::onHandleStart);
  connect(_stop_btn, &QAbstractButton::clicked, this, &AppMain::onHandleStop);

  connect(&_controller, &Controller::passData, this, &AppMain::onDataGet);
  //QGridLayout *main_layout = nullptr;
  _view = new QGraphicsView();
  //_image = new QImage(_view->viewport()->width(), _view->viewport()->height(),
  // QImage::Format_ARGB32);
  _image = new QImage(opt->xRes, opt->yRes, QImage::Format_ARGB32);
  _image->fill(QColor(255, 255, 255));
  _scene = new QGraphicsScene;

  _refreshTimer.setTimerType(Qt::PreciseTimer);
  _refreshTimer.setSingleShot(false);
  _refreshTimer.setInterval(25);
  connect(&_refreshTimer, &QTimer::timeout, this, &AppMain::doRepaint);
  connect(_stop_btn, &QAbstractButton::clicked, &_refreshTimer, &QTimer::stop);
  _progress = new QProgressBar();
  _progress->setMaximum(opt->yRes);
  _progress->setMinimum(0);

  main_layout = new QGridLayout(this);
  main_layout->addWidget(button_box, 1, 0, 1, 2);
  main_layout->addWidget(_view, 2, 0, 50, -1);

  setWindowTitle(QGuiApplication::applicationDisplayName());

}

void AppMain::onHandleStart() {
  main_layout->addWidget(_progress);
  qDebug() << "Start clicked";
  _controller.start();
}

void AppMain::onHandleStop() {
  _progress->hide();
  main_layout->removeWidget(_progress);
  qDebug() << "Stop clicked";
  _controller.stop();
}

void AppMain::doRepaint() { repaint(); }

void AppMain::onDataGet(const QImage &newImage) {
  _progress->setValue(_progress->value()+1);
  *_image = newImage;
  _scene->clear();
  _scene->addPixmap(QPixmap::fromImage(*_image));
  if (_image->isNull()) {
    qDebug() << "_image is null!";
  }

  if (newImage.isNull()) {
    qDebug() << "newImage is null!";
  }
  _view->setScene(_scene);
}
