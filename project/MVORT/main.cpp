
#include "inputparser.h"
#include "render.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <vector>

int main(int argc, char **argv) {
  InputParser input(argc, argv);
  std::string filename;

  if(input.CMDOptionExists("-h")) {
    std::cout << "Usage: MVORT [OPTION]\n \
File output options:\n \
-f [FILENAME.{ppm|jpg|png}]\n\n \
Render options:\n \
  Mesh input file:\n \
-m [FILENAME.{obj}] \n\n\
Help:\n \
-h\n";
    return 0;
  } else if (input.CMDOptionExists("-f")) {
    std::string temp = input.getCMDOption("-f");
    filename = temp;

    //returned arguments are delineated by " "
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

    a.setSampleRate(4);
    a.setXResolution(800);
    a.setYResolution(400);

    a.setCameraPosition(Eigen::Vector3f(10, 2, 4));
    a.setCameraTarget(Eigen::Vector3f(1, 0, -5));
    a.setCameraAperature(0.0);
    a.setCameraFocalDist(5.0);
    a.setCameraVFOV(50);

    // To render we cannot simply pass a vector of Spheres, as we they are
    // derived from the abstract class Hitable (for instance how would we add a
    // new class Triangle?) so we must build a vector of pointers to the
    // abstract class Hitable each with its specific class instantiation.  This
    // object is moved (not copied) to the setScene function.  This allows us to
    // easily interface with arbitrary scene data abstractly rather than
    // hard-coding it in the render functionality itself
    std::vector<std::unique_ptr<Hitable>> scene;

    if (input.CMDOptionExists("-m")) {
      std::string meshFile = input.getCMDOption("-m");

      // Just checking if it is empty for now as Assimp (should) support 40+
      // formats.  So were going to let the library figure that one out
      if (meshFile.empty()) {
        std::cout << "Error: Unrecognized mesh filetype.\n \
See Assimp documentation for supported filetypes.\n";

        return -1;
      }

      // TODO: Add support for textures, and materials from the imported data
      Model modelObject(meshFile);

      modelObject.setRotationTranslation(0.0*M_PI, 0.0*M_PI, 0.0*M_PI, Eigen::Vector3f(-1,4,1));
      std::vector<Eigen::Vector3f> modelData = modelObject.getMeshData();

      for(unsigned long i = 0; i < modelData.size(); i+=3) {
        // the model data is represented here as a densely packed vector of
        // vectors, where every 3 consecutive vectors (no offset) define a
        // triangle.  Many formats (including wavefront .obj) support mesh face
        // data for polygons, rather than just triangles, but the triangulation
        // import option from assimp should work just fine.
        Eigen::Vector3f a = modelData.at(i + 0);
        Eigen::Vector3f b = modelData.at(i + 1);
        Eigen::Vector3f c = modelData.at(i + 2);

        scene.push_back(std::unique_ptr<Hitable>(new Triangle(a, b, c, new Lambertian(new ConstantTexture(Eigen::Vector3f(0.5, 0.5, 0.5))))));
      }
    }

    std::string textureFile = "earthmap.jpg";

    Texture *checker =
        new CheckerBoard(new ConstantTexture(Eigen::Vector3f(0.5, 0.2, 0.1)),
                         new ConstantTexture(Eigen::Vector3f(0.9, 0.9, 0.9)));

    // scene.push_back(std::unique_ptr<Hitable>(
    //     new Sphere(Eigen::Vector3f(0, 0, -1), 0.5,
    //                new Lambertian(new ConstantTexture(Eigen::Vector3f(0.1, 0.2, 0.5))))));

    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Eigen::Vector3f(0, -500.5, -1), 500,
                   new Lambertian(checker))));

    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Eigen::Vector3f(0, 1, -4.75), 2.5, new Metal(Eigen::Vector3f(0.8, 0.6, 0.2), 0.3))));
    scene.push_back(std::unique_ptr<Hitable>(new Sphere(
        Eigen::Vector3f(-4, 2, 1.5), 2.5, new Lambertian(new ImageTexture(textureFile)))));
    scene.push_back(std::unique_ptr<Hitable>(
         new Sphere(Eigen::Vector3f(2, 1.5, 0), 2.5, new Dielectic(1.5))));
    // scene.push_back(std::unique_ptr<Hitable>(
    //     new Sphere(Eigen::Vector3f(1, 0, -2), -0.45, new Dielectic(1.5))));
    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Eigen::Vector3f(1, 10, -1), 1.5,
                   new DiffuseLight(new ConstantTexture(Eigen::Vector3f(3, 3, 3))))));
    scene.push_back(std::unique_ptr<Hitable>(
        new Sphere(Eigen::Vector3f(1, 14, -5), 1.5,
                   new DiffuseLight(new ConstantTexture(Eigen::Vector3f(6, 3.5, 2))))));
    // scene.push_back(std::unique_ptr<Hitable>(
    //     new Sphere(Eigen::Vector3f(4, 4, -3), 1.5,
    //                new DiffuseLight(new ConstantTexture(Eigen::Vector3f(2, 1, 5))))));
    a.setScene(std::move(scene));

    auto timeStart = std::chrono::high_resolution_clock::now();
    a.makeRender();
    auto timeEnd = std::chrono::high_resolution_clock::now();

    auto elapsedTime = timeEnd - timeStart;


    std::cout << "Rendering took " << std::chrono::duration_cast<std::chrono::minutes>(elapsedTime).count() << " minutes." << std::endl;

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
