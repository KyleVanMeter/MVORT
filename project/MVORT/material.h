#ifndef __MATERIAL__
#define __MATERIAL__

#include "ray.h"
#include "hitable.h"
#include <QtGui/qimagereader.h>

struct Hit_Record;

float schlick(float cosine, float ref_idx) {
  float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

Vec3 random_in_unit_sphere() {
  Vec3 p;
  do {
    p = 2.0 * Vec3(drand48(), drand48(), drand48()) - Vec3(1, 1, 1);
  } while (p.squared_length() >= 1.0);

  return p;
}

Vec3 reflect(const Vec3 &v, const Vec3 &n) { return (v - 2 * dot(v, n) * n); }

bool refract(const Vec3 &v, const Vec3 &n, float ni_over_nt, Vec3 &refracted) {
  Vec3 uv = unit_vector(v);
  float dt = dot(uv, n);
  float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
  if (discriminant > 0) {
    refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
    return true;
  }

  return false;
}

class Texture {
 public:
  virtual Vec3 value(float u, float v, const Vec3 &p) const = 0;
};

class ConstantTexture : public Texture {
 public:
  ConstantTexture() {}
  ConstantTexture(Vec3 c) : _color(c) {}
  virtual Vec3 value(float u, float v, const Vec3 &p) const {
    return _color;
  }

 private:
  Vec3 _color;
};

class CheckerBoard : public Texture {
 public:
  CheckerBoard() {}
  CheckerBoard(Texture *t0, Texture *t1) : _even(t0), _odd(t1) {}
  virtual Vec3 value(float u, float v, const Vec3 &p) const {
    float pattern = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if(pattern < 0) {
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

  virtual Vec3 value(float u, float v, const Vec3 &p) const {
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

    return Vec3(r, g, b);
  }

private:
  std::string _fileName;
  uchar *_pixelData;
  int _x, _y;
};

class Material {
public:
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Vec3 &attenuation, Ray &scattered) const = 0;
  virtual Vec3 emitted(float u, float v, const Vec3 &p) const {
    return Vec3(0, 0, 0);
  }
};

class Lambertian : public Material {
public:
  Lambertian(Texture *a) : _albedo(a) {}
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Vec3 &attenuation, Ray &scattered) const {
    Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = Ray(rec.p, target - rec.p);
    attenuation = _albedo->value(rec.u, rec.v, rec.p);

    return true;
  }

private:
  Texture *_albedo;
};

class Metal : public Material {
public:
  Metal(const Vec3 a, float f) : albedo(a) {
    if (f < 1) {
      fuzz = f;
    } else {
      fuzz = 1;
    }
  }
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Vec3 &attenuation, Ray &scattered) const {
    Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

  float fuzz;
  Vec3 albedo;
};

class Dielectic : public Material {
 public:
 Dielectic(float ri) : ref_idx(ri) {}
 virtual bool scatter(const Ray &r_in, const Hit_Record &rec, Vec3 &attenuation,
                      Ray &scattered) const {
   Vec3 outward_normal;
   Vec3 reflected = reflect(r_in.direction(), rec.normal);
   float ni_over_nt;
   attenuation = Vec3(1.0, 1.0, 1.0);
   Vec3 refracted;
   float reflect_prob;
   float cosine;

   if(dot(r_in.direction(), rec.normal) > 0) {
     outward_normal = -rec.normal;
     ni_over_nt = ref_idx;

     cosine = ref_idx + dot(r_in.direction(), rec.normal) / r_in.direction().length();
   } else {
     outward_normal = rec.normal;
     ni_over_nt = 1.0 / ref_idx;

     cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
   }

   if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
     reflect_prob = schlick(cosine, ref_idx);
   } else {
     scattered = Ray(rec.p, refracted);
     reflect_prob = 1.0;
   }

   if(drand48() < reflect_prob) {
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
                       Vec3 &attenuation, Ray &scattered) const {
    return false;
  }
  virtual Vec3 emitted(float u, float v, const Vec3 &p) const {
    return _emit->value(u, v, p);
  }

private:
  Texture *_emit;
};
#endif
