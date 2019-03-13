#ifndef __HITABLE__
#define __HITABLE__

#include "ray.h"

class Material;

// transforms from a texture coordinate to a spherical coordinate
// also known as UV mapping on a sphere
void getSphereCoords(const Vec3 &p, float &u, float &v) {
  /* u = 1 - (atan2(p.z(), p.x()) + M_PI) / (2 * M_PI); */
  /* v = (asin(p.y()) + M_PI / 2) / M_PI; */
  u = 0.5 - (atan2(p.z(), p.x()) / (2 * M_PI));
  v = 0.5 + (asin(p.y()) / M_PI);
}

struct Hit_Record {
  float t;
  float u;
  float v;
  Vec3 p;
  Vec3 normal;
  Material *mat_ptr;
};

class Hitable {
 public:
  virtual bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const = 0;
};
#endif
