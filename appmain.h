#ifndef APPMAIN_H
#define APPMAIN_H

#include <QColor>
#include <QDialog>
#include <QGraphicsView>
#include <QObject>

#include "controller.h"
#include "options.h"

class AppMain : public QDialog {
  Q_OBJECT
public:
  explicit AppMain(std::vector<std::unique_ptr<Hitable>> sceneDescription,
                   Options *opt, QWidget *parent);

private slots:
  void onHandleStart();
  void onHandleStop();
  void onDataGet(const QRgb &newColor, int x, int y);

private:
  QPushButton *_start_btn = nullptr;
  QPushButton *_stop_btn = nullptr;
  QGraphicsView *_view = nullptr;
  QGraphicsScene *_scene = nullptr;
  Controller _controller;
  QImage *_image = nullptr;

  int _x;
  int _y;
};

#endif // APPMAIN_H
