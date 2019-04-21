#include "render.h"
#include "camera.h"
#include "hitable_list.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"

#include <QMetaType>
#include <QDebug>

Render::Render(const std::string &filename) {
  assert(!filename.empty());

  _filename.assign(filename);

  // It should be noted here that the amount added to the delimiters is the
  // length of the delimiters themselves
  unsigned startDelim = _filename.find(".");
  unsigned endDelim = _filename.find(" ");
  _format.assign(
      _filename.substr(startDelim + 1, endDelim - (startDelim + 1)));

  assert(_format == "png" || _format == "jpg" || _format == "ppm");

  _xRes = 800;
  _yRes = 400;
  _sampleRate = 4;
  _aperature = 0.0;
  _focal = 10.0;
  _vFOV = 20;
  _time0 = 0.0;
  _time1 = 1.0;
  _camRoll = Eigen::Vector3f(0, 1, 0);
  _camPos = Eigen::Vector3f(0, 0, 0);
  _camTarget = Eigen::Vector3f(1, 1, 1);

  _cam = new Camera(_camPos, _camTarget, _camRoll, _vFOV,
                    float(_xRes) / (float(_yRes)), _aperature, _focal, _time0,
                    _time1);
}

Render::Render(const Options &opt) {
  assert(!opt.filename.empty());

  _filename.assign(opt.filename);

  // It should be noted here that the amount added to the delimiters is the
  // length of the delimiters themselves
  unsigned startDelim = _filename.find(".");
  unsigned endDelim = _filename.find(" ");
  _format.assign(
      _filename.substr(startDelim + 1, endDelim - (startDelim + 1)));

  assert(_format == "png" || _format == "jpg" || _format == "ppm");

  _xRes = opt.xRes;
  _yRes = opt.yRes;
  _sampleRate = opt.sampleRate;
  _aperature = opt.aperature;
  _focal = opt.focal;
  _vFOV = opt.vFOV;
  _time0 = opt.time0;
  _time1 = opt.time1;
  _camRoll = opt.camRoll;
  _camPos = opt.camPos;
  _camTarget = opt.camTarget;

  _cam = new Camera(_camPos, _camTarget, _camRoll, _vFOV,
                    float(_xRes) / (float(_yRes)), _aperature, _focal, _time0,
                    _time1);
}

Camera *Render::getCamera() { return _cam; }

Eigen::Vector3f Render::getCameraTarget() { return _camTarget; }
void Render::setCameraTarget(Eigen::Vector3f pos) { _camTarget = pos; }

Eigen::Vector3f Render::getCameraPosition() { return _camPos; }
void Render::setCameraPosition(Eigen::Vector3f pos) { _camPos = pos; }

float Render::getCameraAperature() { return _aperature; }
void Render::setCameraAperature(float x) { _aperature = x; }

float Render::getCameraFocalDist() { return _focal; }
void Render::setCameraFocalDist(float x) { _focal = x; }

int Render::getCameraVFOV() { return _vFOV; }
void Render::setCameraVFOV(int x) { _vFOV = x; }

Eigen::Vector3f Render::getCameraRoll() { return _camRoll; }
void Render::setCameraRoll(Eigen::Vector3f roll) { _camRoll = roll; }

float Render::getInitialTime() { return _time0; }
void Render::setInitialTime(float x) { _time0 = x; }

float Render::getEndTime() { return _time1; }
void Render::setEndTime(float x) { _time1 = x; }

int Render::getSampleRate() { return _sampleRate; }
void Render::setSampleRate(int k) { _sampleRate = k; }

std::pair<int, int> Render::getResolution() { return std::make_pair(_xRes, _yRes); }
void Render::setResolution(std::pair<int, int> a) {
  _xRes = a.first;
  _yRes = a.second;
}

void Render::setXResolution(int x) { _xRes = x; }
void Render::setYResolution(int y) { _yRes = y; }

void Render::setScene(std::vector<std::unique_ptr<Hitable>> sceneDescription) {
  Hitable **list = new Hitable *[sceneDescription.size() + 1];

  int k = 0;
  for (auto &e : sceneDescription) {
    // We release the control of the object from the unique_ptr to that of
    // list[], is no longer automatically deleted, but is instead bound to
    // the lifetime of the list[] object
    list[k++] = e.release();
  }

  _world = new Hitable_List(list, k);
}

Eigen::Vector3f Render::oldColor(const Ray &r, Hitable *world) {
  Hit_Record rec;
  if(world->hit(r, 0.001, MAXFLOAT, rec)) {
    Eigen::Vector3f target = rec.p + rec.normal;
    return 0.5 * oldColor(Ray(rec.p, rec.normal), world);
  } else {
    Eigen::Vector3f unit_direction = r.direction().normalized();
    float t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * Eigen::Vector3f(1.0, 1.0, 1.0) + t * Eigen::Vector3f(0.5, 0.7, 0.5);
  }
}

Eigen::Vector3f Render::color(const Ray &r, Hitable *world, int depth) {
  Hit_Record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    Ray scattered;
    Eigen::Vector3f attenuation;
    Eigen::Vector3f emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      // TODO: Either find a better way of doing this, or overload the *
      // operator for Vector3f, by Vector3f element-wise multiplication.

      Eigen::Vector3f ret = color(scattered, world, depth + 1);
      Eigen::Vector3f result(attenuation.x() * ret.x(),
                             attenuation.y() * ret.y(),
                             attenuation.z() * ret.z());
      return (emitted + result);
    }
    return emitted;

  } else {
    /* Eigen::Vector3f unit_direction = unit_vector(r.direction()); */
    /* float t = 0.5 * (unit_direction.y() + 1.0); */
    /* return ((1.0 - t) * Eigen::Vector3f(1.0, 1.0, 1.0) + t *
     * Eigen::Vector3f(0.5, 0.7, 1.0)); */
    return Eigen::Vector3f(0, 0, 0);
  }
}

// This function makes the call to render each pixel stored in the QRgb object,
// which is rendered to the filename passed to the Render constructor after the
// rendering is done.  This function also contains the anti-aliasing logic
// (since we are just averaging out several rays per pixel)
QImage *Render::_Render() {
  int nx = _xRes;
  int ny = _yRes;
  int pixel_count = nx * ny;

  std::cout << "Rendering image to " + _filename + " with " +
                   std::to_string(nx) + "x" + std::to_string(ny) +
                   " resolution.\n";

  QImage *image = new QImage(nx, ny, QImage::Format::Format_RGB32);

  Camera cam(_camPos, _camTarget, _camRoll, _vFOV, float(nx) / float(ny),
             _aperature, _focal, _time0, _time1);

  int count = 0;

#pragma omp parallel for
  for (int j = 0; j < ny; j++) {
    QRgb *line = (QRgb*)image->scanLine(j);
    for (int i = 0; i < nx; i++) {

      count++;
      printf("\rRendering pixel %d out of %d", count, pixel_count);

      Eigen::Vector3f col(0, 0, 0);
      for (int k = 0; k < _sampleRate; k++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        Ray r = cam.get_ray(u, v);
        col += oldColor(r, _world);
      }

      col /= float(_sampleRate);
      col = Eigen::Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

      Eigen::Vector3f rgb = 255.99 * col;

      line[i] = qRgb(rgb.x(), rgb.y(), rgb.z());
      //emit dataGenerated(qRgb color, int x, int y);
    }
  }

  *image = image->mirrored(false, true);
  return (image);
}

QImage *Render::getRender() { return (_Render()); }

void Render::makeRender() {
  QImage *image = _Render();
  QString qfilename = QString::fromStdString(_filename);
  const char *qformat = _format.c_str();

  // TODO: Look into updating this so it can be written (QPainter) is real time
  image->save(qfilename, qformat, 100);

  std::cout << "\nDone.\n";
}

Hitable *Render::randomScene() {
  int n = 500;
  Hitable **list = new Hitable *[n + 1];

  list[0] = new Sphere(
      Eigen::Vector3f(0, -1000, 0), 1000,
      new Lambertian(new ConstantTexture(Eigen::Vector3f(0.5, 0.5, 0.5))));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      Eigen::Vector3f center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
      if ((center - Eigen::Vector3f(4, 0.2, 0)).norm() > 0.9) {
        if (choose_mat < 0.8) {
          list[i++] = new Moving_Sphere(
              center, center + Eigen::Vector3f(0, 0.5 * drand48(), 0), 0.0, 1.0,
              0.2,
              new Lambertian(new ConstantTexture(
                  Eigen::Vector3f(drand48() * drand48(), drand48() * drand48(),
                                  drand48() * drand48()))));
          // list[i++] = new Sphere(
          //     center, 0.2,
          //     new Lambertian(Eigen::Vector3f(drand48() * drand48(), drand48()
          //     * drand48(),
          //                         drand48() * drand48())));
        } else if (choose_mat < 0.95) {
          list[i++] =
              new Sphere(center, 0.2,
                         new Metal(Eigen::Vector3f(0.5 * (1 + drand48()),
                                                   0.5 * (1 + drand48()),
                                                   0.5 * (1 + drand48())),
                                   0.5 * drand48()));
        } else {
          list[i++] = new Sphere(center, 0.2, new Dielectic(1.5));
        }
      }
    }
  }

  list[i++] = new Sphere(Eigen::Vector3f(0, 1, 0), 1.0, new Dielectic(1.5));
  list[i++] = new Sphere(
      Eigen::Vector3f(-4, 1, 0), 1.0,
      new Lambertian(new ConstantTexture(Eigen::Vector3f(0.4, 0.2, 0.1))));
  list[i++] = new Sphere(Eigen::Vector3f(4, 1, 0), 1.0,
                         new Metal(Eigen::Vector3f(0.7, 0.6, 0.5), 0.0));

  return new Hitable_List(list, i);
}

void Render::init() {
  bool status = true;

  qDebug() << "init";
  emit initialized(status);
}

void Render::start() {
  bool status = true;

  qDebug() << "start";
  emit started(status);
}

void Render::stop() {
  bool status = true;

  qDebug() << "stop";
  emit stopped(status);
}

Q_DECLARE_METATYPE(QRgb);

void Render::generateData() {
  qRegisterMetaType<QRgb>("QRgb");
  qDebug() << "generate, " << _xRes << "x" << _yRes << "\n";
  Camera cam(_camPos, _camTarget, _camRoll, _vFOV, float(_xRes) / float(_yRes),
             _aperature, _focal, _time0, _time1);

  //int count = 0;
//#pragma omp parallel for
  for (int j = 0; j < _yRes; j++) {
    for (int i = 0; i < _xRes; i++) {

     //count++;
     //printf("\rRendering pixel %d out of ALOT", count);

      Eigen::Vector3f col(0, 0, 0);
      for (int k = 0; k < _sampleRate; k++) {
        float u = float(i + drand48()) / float(_xRes);
        float v = float(j + drand48()) / float(_yRes);
        Ray r = cam.get_ray(u, v);
        col += oldColor(r, _world);
      }

      col /= float(_sampleRate);
      col = Eigen::Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

      Eigen::Vector3f rgb = 255.99 * col;

      QRgb color = qRgb(rgb.x(), rgb.y(), rgb.z());
      //emit dataGenerated(qRgb(rgb.x(), rgb.y(), rgb.z()), i, j);
      emit dataGenerated(color, i, j);
    }
  }
}
