#ifndef __CAMERA__
#define __CAMERA__

#include "ray.h"

Eigen::Vector3f random_in_unit_disk() {
  Eigen::Vector3f p;
  do {
    p = 2.0 * Eigen::Vector3f(drand48(), drand48(), 0) - Eigen::Vector3f(1, 1, 0);
  } while (p.dot(p) >= 1.0);

  return p;
}

class Camera {
 public:
   // The argument list for Camera are: Camera position, Camera target, roll
   // normal vector, vertical fov in degrees, aspect ratio, aperature size,
   // focal distance, time0, and time1
   Camera(Eigen::Vector3f lookfrom, Eigen::Vector3f lookat, Eigen::Vector3f vup, float vfov, float aspect,
          float aperature, float focus_dist, float t0, float t1) {
     time0 = t0;
     time1 = t1;
     lens_radius = aperature / 2;
     float theta = vfov * M_PI / 180;
     float half_height = tan(theta / 2);
     float half_width = aspect * half_height;
     origin = lookfrom;
     w = (lookfrom - lookat).normalized();
     u = (vup.cross(w).normalized());
     v = w.cross(u);
     lower_left_corner = origin - half_width * focus_dist * u -
                         half_height * focus_dist * v - focus_dist * w;
     horizontal = 2 * half_width * focus_dist * u;
     vertical = 2 * half_height * focus_dist * v;
   }

  Ray get_ray(float s, float t) {
    Eigen::Vector3f rd = lens_radius * random_in_unit_disk();
    Eigen::Vector3f offset = u * rd.x() + v * rd.y();
    float time = time0 + drand48() * (time1 - time0);
    return Ray(origin + offset,
               lower_left_corner + s * horizontal + t * vertical - origin -
               offset, time);
  }

  float lens_radius, time0, time1;
  Eigen::Vector3f u, v, w, origin, lower_left_corner, horizontal, vertical;
};

#endif
