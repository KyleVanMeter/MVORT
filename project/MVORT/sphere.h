#ifndef __SPHERE__
#define __SPHERE__

#include "hitable.h"

class sphere : public Hitable {
 public:
   sphere() {}
   sphere(Vec3 cen, float r) : center(cen), radius(r){};
   virtual bool hit(const Ray &r, float tmin, float tmax,
                    Hit_Record &rec) const;
   Vec3 center;
   float radius;
};

bool sphere::hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const {
  Vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;

  float discriminant = b*b - a*c;
  if(discriminant > 0) {
    float temp = (-b - sqrt(b*b - a*c))/a;
    if(temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;

      return true;
    }

    temp = (-b + sqrt(b*b - a*c))/a;
    if(temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;

      return true;
    }
  }

  return false;
}

#endif
