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
    a.setXResolution(800);
    a.setYResolution(400);

    a.setCameraPosition(Vec3(5, 1, 2));
    a.setCameraTarget(Vec3(0, 0, 0));
    a.setCameraAperature(0.0);
    a.setCameraFocalDist(1.0);
    a.setCameraVFOV(20);

    std::vector<std::unique_ptr<Hitable>> scene;
    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)))));
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
