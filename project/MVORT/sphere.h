#ifndef __SPHERE__
#define __SPHERE__

#include "hitable.h"

/* class Triangle : public Hitable { */
/*  public: */
/*   Triangle() {} */
/*  Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Material *a) : _vertex1(v1), _vertex2(v2), _vertex3(v3), mat_ptr(a) {}; */
/*   virtual bool hit(const Ray &r, float tmin, float tmax, Hit_Record &rec) const; */

/*  private: */
/*   Vec3 _vertex1; */
/*   Vec3 _vertex2; */
/*   Vec3 _vertex3; */
/*   Material *mat_ptr; */
/* }; */

/* bool Triangle::hit(const Ray &r, float t_min, float t_max, Hit_Record &rec) const { */
/*   const float epsilon = 0.0000001; */

/*   Vec3 e1, e2, h, s, q; */
/*   float a, f, u, v; */
/*   Vec3 unitDirection = r.direction(); */
/*   Vec3 unitOrigin = r.origin(); */
/*   Vec3 v0 = _vertex1; */
/*   Vec3 v1 = _vertex2; */
/*   Vec3 v2 = _vertex3; */

/*   e1 = v1 - v0; */
/*   e2 = v2 - v0; */

/*   h = cross(unitDirection, e2); */
/*   a = dot(e1, h); */

/*   if(a > -epsilon && a < epsilon) { */
/*     return false; */
/*   } */

/*   f = 1.0 / a; */
/*   s = unitOrigin - v0; */
/*   u = f * dot(s, h); */

/*   if(u < 0.0 || u > 1.0) { */
/*     return false; */
/*   } */

/*   q = cross(s, e1); */
/*   v = f * dot(unitDirection, q); */

/*   if(v < 0.0 || u + v > 1.0) { */
/*     return false; */
/*   } */

/*   float t = f * dot(e2, q); */

/*   if(t > epsilon) { */
/*     if (t < t_max && t > t_min) { */
/*       // out at r.origin() + r.direction() * t; */
/*       rec.mat_ptr = mat_ptr; */
/*       rec.t = t; */
/*       rec.normal = cross(e1, e2); */
/*       return true; */
/*     } */
/*   } */

/*   return false; */
/* } */

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
