#pragma once

#ifndef __RENDER__
#define __RENDER__

#include "QtGui/QImage"
#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "camera.h"
#include "hitable_list.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "options.h"

class Render : public QObject {
  Q_OBJECT
public:
  Render(const std::string &filename);
  Render(const Options &opt);

  /* --------------------------------------------------------------------------------------
     Public Render member functions
     --------------------------------------------------------------------------------------
   */
  void makeRender();
  QImage *getRender();

  /* --------------------------------------------------------------------------------------
     Public Scene member functions
     --------------------------------------------------------------------------------------
   */
  void setScene(std::vector<std::unique_ptr<Hitable>> sceneDescription);
  std::vector<Hitable> getScene();
  Hitable *randomScene();

  /* --------------------------------------------------------------------------------------
     Public Camera member functions
     --------------------------------------------------------------------------------------
   */
  void setCamera(Eigen::Vector3f lookfrom, Eigen::Vector3f lookat,
                 Eigen::Vector3f vup, float vfov, float aspect, float aperature,
                 float focus_dist, float t0, float t1);
  Camera *getCamera();

  Eigen::Vector3f getCameraTarget();
  void setCameraTarget(Eigen::Vector3f pos);

  Eigen::Vector3f getCameraPosition();
  void setCameraPosition(Eigen::Vector3f pos);

  float getCameraAperature();
  void setCameraAperature(float x);

  float getCameraFocalDist();
  void setCameraFocalDist(float x);

  int getCameraVFOV();
  void setCameraVFOV(int x);

  Eigen::Vector3f getCameraRoll();
  void setCameraRoll(Eigen::Vector3f roll);

  float getInitialTime();
  void setInitialTime(float x);

  float getEndTime();
  void setEndTime(float x);

  /* --------------------------------------------------------------------------------------
     Public Resolution member functions
     --------------------------------------------------------------------------------------
   */
  int getSampleRate();
  void setSampleRate(int k);

  std::pair<int, int> getResolution();
  void setResolution(std::pair<int, int> a);
  void setXResolution(int x);
  void setYResolution(int y);

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
  QImage *_Render();
  Eigen::Vector3f color(const Ray &r, Hitable *world, int depth);
  Eigen::Vector3f oldColor(const Ray &r, Hitable *world);

  Hitable *_world;
  Camera *_cam;
  float _aperature, _focal, _time0, _time1;
  int _sampleRate, _vFOV, _xRes, _yRes;
  Eigen::Vector3f _camPos, _camTarget, _camRoll;

  std::string _filename;
  std::string _format;
};

#endif
