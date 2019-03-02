#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "ray.h"
#include "camera.h"
#include "material.h"
#include <stdlib.h>

Vec3 color(const Ray &r, Hitable *world, int depth) {
  Hit_Record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    Ray scattered;
    Vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return (attenuation * color(scattered, world, depth + 1));
    } else {
      return Vec3(0, 0, 0);
    }
  } else {
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return ((1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0));
  }
}

int main() {
  int nx = 400;
  int ny = 200;
  int ns = 100;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  float R = cos(M_PI/4);
  Hitable *list[5];
  list[0] =
      new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));
  list[1] =
      new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
  list[2] =
      new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.3));
  list[3] =
      new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectic(1.5));
  list[4] =
      new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectic(1.5));
  Hitable *world = new Hitable_List(list, 5);

  Vec3 lookat(0, 0, -1);
  Vec3 lookfrom(3, 3, 2);
  float dist_to_focus = (lookfrom - lookat).length();
  float aperature = 2.0;
  Camera cam(lookfrom, lookat, Vec3(0,1,0), 20, float(nx)/float(ny), aperature, dist_to_focus);

  for(int j = ny-1; j >= 0; j--) {
    for(int i = 0; i < nx; i++) {
      Vec3 col(0,0,0);
      for(int k = 0; k < ns; k++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        Ray r = cam.get_ray(u, v);
        Vec3 p = r.point_at_parameter(2.0);
        col += color(r, world, 0);
      }
      col /= float(ns);
      col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
