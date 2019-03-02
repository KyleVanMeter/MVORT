#ifndef __MATERIAL__
#define __MATERIAL__

#include "ray.h"
#include "hitable.h"

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
#endif
