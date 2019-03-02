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

  Hitable *list[4];
  list[0] =
      new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.8, 0.3, 0.3)));
  list[1] =
    new Sphere(Vec3(0, -100.5, -1), 100.0, new Lambertian(Vec3(0.8, 0.8, 0.0)));
  list[2] =
    new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.3));
  list[3] =
    new Sphere(Vec3(-1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.8, 0.8), 1.0));
  Hitable *world = new Hitable_List(list, 4);
  Camera cam;

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
