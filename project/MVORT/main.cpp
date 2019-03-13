#include "inputparser.h"
#include "render.h"

#include <memory>
#include <iostream>
#include <vector>
#include <stdlib.h>

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
    // TODO Do not read until end of line in case there are other arguments,
    // find some other way of ending substr
    temp = temp.substr(temp.find(".") + 1, temp.size());

    if(temp != "png" && temp != "jpg" && temp != "ppm") {
      std::cout << "Error: Unrecognized filetype.\n \
Supported filetypes are:\n \
                        .png\n \
                        .jpg\n \
                        .ppm\n";

      return -1;
    }

    Render a(filename);

    a.setSampleRate(100);
    a.setXResolution(1600);
    a.setYResolution(800);

    a.setCameraPosition(Vec3(5, 1, 2));
    a.setCameraTarget(Vec3(1, 0, -1));
    a.setCameraAperature(0.15);
    a.setCameraFocalDist(5.0);
    a.setCameraVFOV(20);

    // To render we cannot simply pass a vector of Spheres, as we they are
    // derived from the abstract class Hitable (for instance how would we add a
    // new class Triangle?) so we must build a vector of pointers to the
    // abstract class Hitable each with its specific class instantiation.  This
    // object is moved (not copied) to the setScene function.  This allows us to
    // easily interface with arbitrary scene data abstractly rather than
    // hard-coding it in the render functionality itself
    std::vector<std::unique_ptr<Hitable>> scene;

    std::string textureFile = "earthmap.jpg";

    Texture *checker =
        new CheckerBoard(new ConstantTexture(Vec3(0.5, 0.2, 0.1)),
                         new ConstantTexture(Vec3(0.9, 0.9, 0.9)));

    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Vec3(0, 0, -1), 0.5,
                   new Lambertian(new ConstantTexture(Vec3(0.1, 0.2, 0.5))))));
    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Vec3(0, -100.5, -1), 100,
                   new Lambertian(checker))));
    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Vec3(0, 0, -2.75), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.3))));
    scene.push_back(std::unique_ptr<Hitable>(new Sphere(
        Vec3(1, 0, -1), 0.5, new Lambertian(new ImageTexture(textureFile)))));
    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Vec3(1, 0, -2), 0.5, new Dielectic(1.5))));
    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Vec3(1, 0, -2), -0.45, new Dielectic(1.5))));

    a.setScene(std::move(scene));

    a.makeRender();

  } else {
    std::cout << "Unrecognized option.\n \
Usage: MVORT [OPTION]\n \
File output options:\n \
-f [FILENAME.{ppm|jpg|png}]\n\n \
Help:\n \
-h\n";
    return -1;
  }

  return 0;
}
