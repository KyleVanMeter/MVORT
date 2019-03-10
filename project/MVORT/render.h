#ifndef __RENDER__
#define __RENDER__

#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "ray.h"

#include "QtGui/QImage"
#include <cassert>
#include <string>
#include <vector>

class Render {
 public:
  Render(const std::string& filename) {
    assert(!filename.empty());

    _filename.assign(filename);

    // It should be noted here that the amount added to the delimiters is the
    // length of the delimiters themselves
    unsigned startDelim = _filename.find(".");
    unsigned endDelim   = _filename.find(" ");
    _format.assign(_filename.substr(startDelim + 1, endDelim - (startDelim + 1)));

    assert(_format == "png" || _format == "jpg" || _format == "ppm");

    _cam = new Camera(Vec3(0,0,0), Vec3(1,1,1), Vec3(0,1,0), 20, float(_xRes)/(float(_yRes)), 0.0, 0.0, 0.0, 0.0);
    _xRes = 200;
    _yRes = 200;
    _sampleRate = 0;
    _aperature = 0.0;
    _focal = 10.0;
    _vFOV = 20;
  }

  void makeRender();

  void setScene(Hitable *list[]);
  std::vector<Hitable> getScene();
  Hitable *randomScene();

  void setCamera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect,
                 float aperature, float focus_dist, float t0, float t1);
  Camera *getCamera() { return _cam; }

  Vec3 getCameraTarget() { return _camTarget; }
  void setCameraTarget(Vec3 pos) { _camTarget = pos; }

  Vec3 getCameraPosition() { return _camPos; }
  void setCameraPosition(Vec3 pos) { _camPos = pos; }

  float getCameraAperature() { return _aperature; }
  void setCameraAperature(float x) { _aperature = x; }

  float getCameraFocalDist() { return _focal; }
  void setCameraFocalDist(float x) { _focal = x; }

  int getCameraVFOV() { return _vFOV; }
  void setCameraVFOV(int x) { _vFOV = x; }

  int getSampleRate() { return _sampleRate; }
  void setSampleRate(int k) { _sampleRate = k; }

  std::pair<int, int> getResolution() { return std::make_pair(_xRes, _yRes); }
  void setXResolution(int x) { _xRes = x; }
  void setYResolution(int y) { _yRes = y; }

private:
  Vec3 color(const Ray &r, Hitable *world, int depth);

  Hitable *_world;
  Camera *_cam;
  float _aperature, _focal;
  int _sampleRate, _vFOV, _xRes, _yRes;
  Vec3 _camPos, _camTarget;

  std::string _filename;
  std::string _format;
};


Vec3 Render::color(const Ray &r, Hitable *world, int depth) {
  Hit_Record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    Ray scattered;
    Vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return (attenuation * color(scattered, world, depth + 1));
    }
    return Vec3(0, 0, 0);

  } else {
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return ((1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0));
  }
}

void Render::makeRender() {
  int nx = _xRes;
  int ny = _yRes;
  int pixel_count = nx * ny;

  std::cout << "Rendering image to " + _filename + " with " + std::to_string(nx) + "x" + std::to_string(ny) + " resolution.\n";
  QImage *image = new QImage(nx, ny, QImage::Format::Format_RGB32);
  QRgb pixel;
  QString qfilename = QString::fromStdString(_filename);
  const char * qformat = _format.c_str();

  float R = cos(M_PI/4);
  Hitable *list[5];
  list[0] =
      new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));
  list[1] =
      new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
  list[2] =
      new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.3));
  list[3] =
      new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectic(1.5));
  list[4] =
      new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectic(1.5));
  Hitable *world = new Hitable_List(list, 5);
  //world = random_scene();

  Camera cam(_camPos, _camTarget, Vec3(0,1,0), _vFOV, float(nx)/float(ny), _aperature, _focal, 0.0, 1.0);

  int count = 0;

  for(int j = ny-1; j >= 0; j--) {
    for(int i = 0; i < nx; i++) {
      Vec3 col(0,0,0);
      for(int k = 0; k < _sampleRate; k++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        Ray r = cam.get_ray(u, v);
        Vec3 p = r.point_at_parameter(2.0);
        col += color(r, world, 0);
      }

      count++;
      printf("\rRendering pixel %d out of %d", count, pixel_count);

      col /= float(_sampleRate);
      col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

      Vec3 rgb = 255.99 * col;
      pixel = qRgb(rgb.r(), rgb.g(), rgb.b());
      image->setPixel(i, -1 * (j-ny+1), pixel);
    }
  }

  image->save(qfilename, qformat, 100);

  std::cout << "\nDone.\n";
}

Hitable * Render::randomScene() {
  int n = 500;
  Hitable **list = new Hitable*[n+1];

  list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
  int i = 1;
  for(int a = -11; a < 11; a++) {
    for(int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      Vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
      if((center-Vec3(4,0.2,0)).length() > 0.9) {
        if(choose_mat < 0.8) {
          list[i++] = new Moving_Sphere(
              center, center + Vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2,
              new Lambertian(Vec3(drand48() * drand48(), drand48() * drand48(),
                                  drand48() * drand48())));
          // list[i++] = new Sphere(
          //     center, 0.2,
          //     new Lambertian(Vec3(drand48() * drand48(), drand48() * drand48(),
          //                         drand48() * drand48())));
        } else if (choose_mat < 0.95) {
          list[i++] = new Sphere(center, 0.2, new Metal(Vec3(0.5*(1+drand48()), 0.5*(1+drand48()),0.5*(1+drand48())), 0.5*drand48()));
        } else {
          list[i++] = new Sphere(center, 0.2, new Dielectic(1.5));
        }
      }
    }
  }

  list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectic(1.5));
  list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
  list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

  return new Hitable_List(list, i);
}
#endif
