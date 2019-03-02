#ifndef __MATERIAL__
#define __MATERIAL__

#include "ray.h"
#include "hitable.h"

struct Hit_Record;

Vec3 random_in_unit_sphere() {
  Vec3 p;
  do {
    p = 2.0 * Vec3(drand48(), drand48(), drand48()) - Vec3(1, 1, 1);
  } while (p.squared_length() >= 1.0);

  return p;
}

Vec3 reflect(const Vec3 &v, const Vec3 &n) { return (v - 2 * dot(v, n) * n); }

class Material {
public:
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Vec3 &attenuation, Ray &scattered) const = 0;
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

class Lambertian : public Material {
public:
  Lambertian(const Vec3 &a) : albedo(a){};
  virtual bool scatter(const Ray &r_in, const Hit_Record &rec,
                       Vec3 &attenuation, Ray &scattered) const {
    Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = Ray(rec.p, target - rec.p);
    attenuation = albedo;
    return true;
  }

  Vec3 albedo;
};
#endif
