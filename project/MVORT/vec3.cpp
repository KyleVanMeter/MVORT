#include "vec3.h"

inline Vec3& Vec3::operator+=(const Vec3 &v) {
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];

  return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v) {
  e[0] -= v.e[0];
  e[1] -= v.e[1];
  e[2] -= v.e[2];

  return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v) {
  e[0] *= v.e[0];
  e[1] *= v.e[1];
  e[2] *= v.e[2];

  return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v) {
  e[0] /= v.e[0];
  e[1] /= v.e[1];
  e[2] /= v.e[2];

  return *this;
}

inline Vec3& Vec3::operator*=(const float t) {
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;

  return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
  float k = 1.0/t;

  e[0] *= k;
  e[1] *= k;
  e[2] *= k;

  return *this;
}

inline Vec3& 
