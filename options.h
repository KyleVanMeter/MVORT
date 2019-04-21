#ifndef OPTIONS_H
#define OPTIONS_H

#include "camera.h"
#include "hitable.h"
#include <memory>
#include <vector>

struct Options {
  std::string filename;
  int sampleRate;
  int xRes, yRes;

  Eigen::Vector3f camPos;
  Eigen::Vector3f camTarget;
  Eigen::Vector3f camRoll;

  Hitable *world;
  Camera *cam;
  float aperature, focal, time0, time1;
  int vFOV;
};

#endif // OPTIONS_H
