#ifndef APPMAIN_H
#define APPMAIN_H

#include <QColor>
#include <QDialog>
#include <QGraphicsView>
#include <QObject>
#include <QProgressBar>
#include <QGridLayout>

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
  void doRepaint();
  // void onDataGet(const QRgb &newColor, int x, int y);
  void onDataGet(const QImage &newImage);

private:
  QGridLayout *main_layout = nullptr;
  QProgressBar *_progress = nullptr;
  QPushButton *_start_btn = nullptr;
  QPushButton *_stop_btn = nullptr;
  QGraphicsView *_view = nullptr;
  QGraphicsScene *_scene = nullptr;
  Controller _controller;
  QImage *_image = nullptr;

  QTimer _refreshTimer;

  int _x;
  int _y;
};

#endif // APPMAIN_H
