#include "sphere.h"
#include "hitable_list.h"
#include "ray.h"
#include "camera.h"
#include "material.h"
#include "inputparser.h"

#include "QtGui/QImage"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

Vec3 color(const Ray &r, Hitable *world, int depth) {
  Hit_Record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    Ray scattered;
    Vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return (attenuation * color(scattered, world, depth + 1));
    }
    return Vec3(0, 0, 0);

  } else {
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return ((1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0));
  }
}

Hitable *random_scene() {
  int n = 500;
  Hitable **list = new Hitable*[n+1];

  list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
  int i = 1;
  for(int a = -11; a < 11; a++) {
    for(int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      Vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
      if((center-Vec3(4,0.2,0)).length() > 0.9) {
        if(choose_mat < 0.8) {
          list[i++] = new Moving_Sphere(
              center, center + Vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2,
              new Lambertian(Vec3(drand48() * drand48(), drand48() * drand48(),
                                  drand48() * drand48())));
          // list[i++] = new Sphere(
          //     center, 0.2,
          //     new Lambertian(Vec3(drand48() * drand48(), drand48() * drand48(),
          //                         drand48() * drand48())));
        } else if (choose_mat < 0.95) {
          list[i++] = new Sphere(center, 0.2, new Metal(Vec3(0.5*(1+drand48()), 0.5*(1+drand48()),0.5*(1+drand48())), 0.5*drand48()));
        } else {
          list[i++] = new Sphere(center, 0.2, new Dielectic(1.5));
        }
      }
    }
  }

  list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectic(1.5));
  list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
  list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

  return new Hitable_List(list, i);
}

void render(const std::string &filename, const std::string &format) {
  int nx = 800;
  int ny = 400;
  int pixel_count = nx * ny;
  int ns = 100;

  std::cout << "Rendering image to " + filename + " with " + std::to_string(nx) + "x" + std::to_string(ny) + " resolution.\n";
  QImage *image = new QImage(nx, ny, QImage::Format::Format_RGB32);
  QRgb pixel;
  QString qfilename = QString::fromStdString(filename);
  const char * qformat = format.c_str();

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
  //world = random_scene();

  Vec3 lookat(0, 0, 0);
  Vec3 lookfrom(13, 2, 3);
  float dist_to_focus = 10.0;
  float aperature = 0.0;
  Camera cam(lookfrom, lookat, Vec3(0,1,0), 20, float(nx)/float(ny), aperature, dist_to_focus, 0.0, 1.0);

  int count = 0;

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

      count++;
      printf("\rRendering pixel %d out of %d", count, pixel_count);

      col /= float(ns);
      col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

      Vec3 rgb = 255.99 * col;
      pixel = qRgb(rgb.r(), rgb.g(), rgb.b());
      image->setPixel(i, -1 * (j-ny+1), pixel);
    }
  }

  image->save(qfilename, qformat, 100);

  std::cout << "\nDone.\n";
}

int main(int argc, char **argv) {

  InputParser input(argc, argv);
  std::string filename;


  if(input.CMDOptionExists("-h")) {
    std::cout << "Usage: MVORT [OPTION]\n \
File output options:\n \
-f [FILENAME.{ppm|jpg|png}]\n\n \
Help:\n \
-h\n";
    return 0;
  } else if (input.CMDOptionExists("-f")) {
    std::string temp = input.getCMDOption("-f");
    filename = temp;
    temp = temp.substr(temp.find(".") + 1, temp.size());

    if(temp != "png" && temp != "jpg" && temp != "ppm") {
      std::cout << "Error: Unrecognized filetype.\n \
Supported filetypes are:\n \
                        .png\n \
                        .jpg\n \
                        .ppm\n";

      return -1;
    }

    std::cout << temp << std::endl;

    render(filename, temp);
  }

  return 0;
}
