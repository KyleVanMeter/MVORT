#ifndef __HITABLE_LIST__
#define __HITABLE_LIST__

#include "hitable.h"

class Hitable_List : public Hitable {
public:
  Hitable_List() {}
  Hitable_List(Hitable **l, int n) {
    list = l;
    list_size = n;
  }
  virtual bool hit(const Ray &r, float tmin, float tmax, Hit_Record &rec) const;
  Hitable **list;
  int list_size;
};

inline bool Hitable_List::hit(const Ray &r, float t_min, float t_max,
                              Hit_Record &rec) const {
  Hit_Record temp_rec;
  bool hit_anything = false;
  double closet_so_far = t_max;
  for (int i = 0; i < list_size; i++) {
    if (list[i]->hit(r, t_min, closet_so_far, temp_rec)) {
      hit_anything = true;
      closet_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

#endif
