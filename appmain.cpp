#include "appmain.h"
#include "options.h"
#include <QDebug>
#include <QDialogButtonBox>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGuiApplication>
#include <QPushButton>

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
  QGridLayout *main_layout = nullptr;
  _view = new QGraphicsView();
  //_image = new QImage(_view->viewport()->width(), _view->viewport()->height(),
  // QImage::Format_ARGB32);
  _image = new QImage(opt->xRes, opt->yRes, QImage::Format_ARGB32);
  _image->fill(QColor(255, 255, 255));
  _scene = new QGraphicsScene;

  main_layout = new QGridLayout(this);
  main_layout->addWidget(button_box, 1, 0, 1, 2);
  main_layout->addWidget(_view, 2, 0, 50, 50);

  setWindowTitle(QGuiApplication::applicationDisplayName());
}

void AppMain::onHandleStart() {
  qDebug() << "Start clicked";
  _controller.start();
}

void AppMain::onHandleStop() {
  qDebug() << "Stop clicked";
  _controller.stop();
}

void AppMain::onDataGet(const QRgb &newColor, int x, int y) {
  _scene->clear();
  _scene->addPixmap(QPixmap::fromImage(*_image));
  _view->setScene(_scene);
  _image->setPixelColor(x, y, newColor);

  repaint();
}
