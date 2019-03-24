#ifndef __RAY__
#define __RAY__

#include <eigen3/Eigen/Dense>

class Ray {
 public:
  Ray() {}
  Ray(const Eigen::Vector3f &a, const Eigen::Vector3f &b, float ti = 0.0) {A = a; B = b; _time = ti; }
  Eigen::Vector3f origin() const { return A; }
  Eigen::Vector3f direction() const { return B; }
  float time() const { return _time; }
  Eigen::Vector3f point_at_parameter(float t) const { return A + t*B; }

  Eigen::Vector3f A;
  Eigen::Vector3f B;
  float _time;
};

#endif
