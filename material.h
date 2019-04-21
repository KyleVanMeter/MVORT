#ifndef __MATERIAL__
#define __MATERIAL__

#include "hitable.h"
#include "ray.h"
#include <QtGui/qimagereader.h>

struct Hit_Record;

inline float schlick(float cosine, float ref_idx) {
  float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

inline Eigen::Vector3f random_in_unit_sphere() {
  Eigen::Vector3f p;
  do {
    p = 2.0 * Eigen::Vector3f(drand48(), drand48(), drand48()) -
        Eigen::Vector3f(1, 1, 1);
  } while (p.squaredNorm() >= 1.0);

  return p;
}

inline Eigen::Vector3f reflect(const Eigen::Vector3f &v, const Eigen::Vector3f &n) {
  return (v - 2 * v.dot(n) * n);
}

inline bool refract(const Eigen::Vector3f &v, const Eigen::Vector3f &n,
             float ni_over_nt, Eigen::Vector3f &refracted) {
  Eigen::Vector3f uv = v.normalized();
  float dt = uv.dot(n);
  float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
  if (discriminant > 0) {
    refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
    return true;
  }

  return false;
}

class Texture {
public:
  virtual Eigen::Vector3f value(float u, float v,
                                const Eigen::Vector3f &p) const = 0;
};

class ConstantTexture : public Texture {
public:
  ConstantTexture() {}
  ConstantTexture(Eigen::Vector3f c) : _color(c) {}
  virtual Eigen::Vector3f value(float u, float v,
                                const Eigen::Vector3f &p) const {
    return _color;
  }

private:
  Eigen::Vector3f _color;
};

class CheckerBoard : public Texture {
public:
  CheckerBoard() {}
  CheckerBoard(Texture *t0, Texture *t1) : _even(t0), _odd(t1) {}
  virtual Eigen::Vector3f value(float u, float v,
                                const Eigen::Vector3f &p) const {
    float pattern = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (pattern < 0) {
      return _odd->value(u, v, p);
    }

    return _even->value(u, v, p);
  }

private:
  Texture *_even;
  Texture *_odd;
};

class ImageTexture : public Texture {
public:
  ImageTexture(std::string &fileName) {
    QString qFileName = QString::fromStdString(fileName);
    QImage image(qFileName);
    image = image.convertToFormat(QImage::Format_RGB888);
    _pixelData = new uchar[image.byteCount()];

    memcpy(_pixelData, image.bits(), image.byteCount());

    _x = image.width();
    _y = image.height();
  }

  virtual Eigen::Vector3f value(float u, float v,
                                const Eigen::Vector3f &p) const {
    int i = (u)*_x;
    int j = (1 - v) * _y - 0.001;
    if (i < 0)
      i = 0;
    if (j < 0)
      j = 0;
    if (i > _x - 1)
      i = _x - 1;
    if (j > _y - 1)
      j = _y - 1;
    float r = int(_pixelData[3 * i + 3 * _x * j]) / 255.0;
    float g = int(_pixelData[3 * i + 3 * _x * j + 1]) / 255.0;
    float b = int(_pixelData[3 * i + 3 * _x * j + 2]) / 255.0;

    return Eigen::Vector3f(r, g, b);
  }

private:
  std::string _fileName;
  uchar *_pixelData;
  int _x, _y;
};

class Material {
public:
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Eigen::Vector3f &attenuation, Ray &scattered) const = 0;
  virtual Eigen::Vector3f emitted(float u, float v,
                                  const Eigen::Vector3f &p) const {
    return Eigen::Vector3f(0, 0, 0);
  }
};

class Lambertian : public Material {
public:
  Lambertian(Texture *a) : _albedo(a) {}
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Eigen::Vector3f &attenuation, Ray &scattered) const {
    Eigen::Vector3f target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = Ray(rec.p, target - rec.p);
    attenuation = _albedo->value(rec.u, rec.v, rec.p);

    return true;
  }

private:
  Texture *_albedo;
};

class Metal : public Material {
public:
  Metal(const Eigen::Vector3f a, float f) : albedo(a) {
    if (f < 1) {
      fuzz = f;
    } else {
      fuzz = 1;
    }
  }
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Eigen::Vector3f &attenuation, Ray &scattered) const {
    Eigen::Vector3f reflected =
        reflect(r_in.direction().normalized(), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (scattered.direction().dot(rec.normal) > 0);
  }

  float fuzz;
  Eigen::Vector3f albedo;
};

class Dielectic : public Material {
public:
  Dielectic(float ri) : ref_idx(ri) {}
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Eigen::Vector3f &attenuation, Ray &scattered) const {
    Eigen::Vector3f outward_normal;
    Eigen::Vector3f reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = Eigen::Vector3f(1.0, 1.0, 1.0);
    Eigen::Vector3f refracted;
    float reflect_prob;
    float cosine;

    if (r_in.direction().dot(rec.normal) > 0) {
      outward_normal = -rec.normal;
      ni_over_nt = ref_idx;

      cosine =
          ref_idx + r_in.direction().dot(rec.normal) / r_in.direction().norm();
    } else {
      outward_normal = rec.normal;
      ni_over_nt = 1.0 / ref_idx;

      cosine = -r_in.direction().dot(rec.normal) / r_in.direction().norm();
    }

    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
      reflect_prob = schlick(cosine, ref_idx);
    } else {
      scattered = Ray(rec.p, refracted);
      reflect_prob = 1.0;
    }

    if (drand48() < reflect_prob) {
      scattered = Ray(rec.p, reflected);
    } else {
      scattered = Ray(rec.p, refracted);
    }

    return true;
  }

  float ref_idx;
};

class DiffuseLight : public Material {
public:
  DiffuseLight(Texture *a) : _emit(a) {}
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Eigen::Vector3f &attenuation, Ray &scattered) const {
    return false;
  }
  virtual Eigen::Vector3f emitted(float u, float v,
                                  const Eigen::Vector3f &p) const {
    return _emit->value(u, v, p);
  }

private:
  Texture *_emit;
};
#endif
