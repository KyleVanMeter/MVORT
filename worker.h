#ifndef WORKER_H
#define WORKER_H

#include <QColor>
#include <QObject>

class Worker : public QObject {
  Q_OBJECT
public:
  Worker();
  virtual ~Worker();

public slots:
  void init();
  void start();
  void stop();
  void generateData();

signals:
  void initialized(bool status);
  void started(bool status);
  void stopped(bool status);
  void dataGenerated(const QRgb &data, int x, int y);

private:
  QColor _data;
};

#endif // WORKER_H
