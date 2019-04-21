#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "options.h"
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QColor>

class Controller : public QObject {
  Q_OBJECT
public:
  Controller(std::vector<std::unique_ptr<Hitable>> sceneDescription, const Options *opt);
  ~Controller();

signals:

public slots:
  void start();
  void stop();

signals:
  void initializedWorkerRequested();
  void startWorkerRequested();
  void stopWorkerRequested();
  void passData(const QRgb &data, int x, int y);

private slots:
  void onDataGenerated(const QRgb &data, int x, int y);

private:
  QThread _worker_thread;
  QTimer _timer;
  Options *_opt;
};

#endif // CONTROLLER_H
