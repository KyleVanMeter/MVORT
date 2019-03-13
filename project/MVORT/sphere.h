#ifndef __SPHERE__
#define __SPHERE__

#include "hitable.h"

class Sphere : public Hitable {
 public:
  Sphere() {}
  Sphere(Vec3 cen, float r, Material *a) : center(cen), radius(r), mat_ptr(a){};
  virtual bool hit(const Ray &r, float tmin, float tmax,
                   Hit_Record &rec) const;
  Vec3 center;
  float radius;
  Material *mat_ptr;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const {
  Vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;

  float discriminant = b*b - a*c;
  if(discriminant > 0) {
    float temp = (-b - sqrt(discriminant))/a;
    if(temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      getSphereCoords((rec.p-center) / radius, rec.u, rec.v);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;

      return true;
    }

    temp = (-b + sqrt(discriminant))/a;
    if(temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;

      return true;
    }
  }

  return false;
}

class Moving_Sphere : public Hitable {
 public:
  Moving_Sphere() {}
  Moving_Sphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material *m)
      : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r),
        mat_ptr(m){};
  virtual bool hit(const Ray &r, float tmin, float tmax, Hit_Record &rec) const;
  Vec3 center(float time) const {
    return (center0 + ((time - time0) / (time1 - time0)) * (center1 - center0));
  }

  Vec3 center0, center1;
  float time0, time1, radius;
  Material *mat_ptr;
};

bool Moving_Sphere::hit(const Ray &r, float tmin, float tmax, Hit_Record &rec) const {
  Vec3 oc = r.origin() - center(r.time());
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - a * c;
  if(discriminant > 0) {
    float temp = (-b - sqrt(discriminant)) / a;

    if(temp < tmax && temp > tmin) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center(r.time())) / radius;
      rec.mat_ptr = mat_ptr;

      return true;
    }

    temp = (-b + sqrt(discriminant)) / a;
    if(temp < tmax && temp > tmin) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center(r.time())) / radius;
      rec.mat_ptr = mat_ptr;

      return true;
    }
  }

  return false;
}

#endif
