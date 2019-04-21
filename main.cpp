#include <QDebug>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <vector>

// Gui headers
#include "appmain.h"
#include <QApplication>
#include <QMainWindow>

#include <qstring.h>
#include <qstringlist.h>

// Rendering headers
#include "inputparser.h"
#include "options.h"
#include "render.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QMainWindow w;
  InputParser input(argc, argv);
  Options optionObject;

  // AppMain client;
  std::string filename;

  if (input.CMDOptionExists("-h")) {
    std::cout << "Usage: MVORT [OPTION]\n \
File output options:\n \
-f [FILENAME.{ppm|jpg|png}]\n\n \
Render options:\n \
  Sample rate:\n \
-rsr [a]\n \
  Resolution:\n \
-rr [a]x[b]\n\n\
  Camera options:\n \
    Camera look from position:\n \
-clf [x],[y],[z]\n \
    Camera look at target:\n \
-cla [x],[y],[z]\n\
    Camera aperature:\n \
-ca [a]\n \
    Camera focal distance:\n \
-cfd [a]\n \
    Camera vertical FOV:\n \
-cvfov [a]\n\n\
  Mesh input file:\n \
-m [FILENAME.{obj}] \n\n\
  Help:\n \
-h\n";

    return 0;
  } else if (input.CMDOptionExists("-f")) {
    std::string temp = input.getCMDOption("-f");
    filename = temp;

    // returned arguments are delineated by " "
    temp = temp.substr(temp.find(".") + 1, temp.size());

    if (temp != "png" && temp != "jpg" && temp != "ppm") {
      std::cout << "Error: Unrecognized filetype.\n \
Supported filetypes are:\n \
                        .png\n \
                        .jpg\n \
                        .ppm\n";

      return -1;
    }

    // Render a(filename);
    optionObject.filename = filename;
    optionObject.xRes = 800;
    optionObject.yRes = 800;
    optionObject.sampleRate = 3;

    if (input.CMDOptionExists("-rsr")) {
      QString renSampleRate =
          QString::fromStdString(input.getCMDOption("-rsr"));

      if (renSampleRate.toInt() < 1) {
        std::cout << "Error: Invalid sample rate.\n \
       was " << renSampleRate.toInt()
                  << ", must be \
greater than 1.\n";

        return -1;
      }

      // a.setSampleRate(renSampleRate.toInt());
      optionObject.sampleRate = renSampleRate.toInt();
    }

    if (input.CMDOptionExists("-rr")) {
      QString renResolution = QString::fromStdString(input.getCMDOption("-rr"));

      QStringList resInt = renResolution.split("x");

      if (resInt.length() != 2) {
        std::cout << "Error: Render resolution argument mismatch,\n \
       number of arguments was "
                  << resInt.length() << ", must be 2.\n";

        return -1;
      }

      // a.setResolution(
      //    std::make_pair<int, int>(resInt.at(0).toInt(),
      //    resInt.at(1).toInt()));
      optionObject.xRes = resInt.at(0).toInt();
      optionObject.yRes = resInt.at(1).toInt();
    }

    // a.setCameraPosition(Eigen::Vector3f(10, 2, 4));
    // a.setCameraTarget(Eigen::Vector3f(1, 0, -5));
    // a.setCameraAperature(0.0);
    // a.setCameraFocalDist(5.0);
    // a.setCameraVFOV(50);

    optionObject.camPos = Eigen::Vector3f(10, 2, 4);
    optionObject.camTarget = Eigen::Vector3f(1, 0, -5);
    optionObject.aperature = 0.0;
    optionObject.focal = 5.0;
    optionObject.vFOV = 50;

    if (input.CMDOptionExists("-clf")) {
      QString camLookFromPos =
          QString::fromStdString(input.getCMDOption("-clf"));
      QStringList floatValue = camLookFromPos.split(",");

      if (floatValue.length() != 3) {
        std::cout << "Error: Camera LookFrom vector argument mismatch.\n \
       number of arguments was "
                  << floatValue.length() << ", must be 3.\n";

        return -1;
      }

      // a.setCameraPosition(Eigen::Vector3f(floatValue.at(0).toFloat(),
      //                                    floatValue.at(1).toFloat(),
      //                                    floatValue.at(2).toFloat()));

      optionObject.camPos = Eigen::Vector3f(floatValue.at(0).toFloat(),
                                            floatValue.at(1).toFloat(),
                                            floatValue.at(2).toFloat());
    }

    if (input.CMDOptionExists("-cla")) {
      QString camLookAtPos = QString::fromStdString(input.getCMDOption("-cla"));
      QStringList floatTarget = camLookAtPos.split(",");

      if (floatTarget.length() != 3) {
        std::cout << "Error: Camera LookAt vector argument mismatch.\n \
       number of arguments was "
                  << floatTarget.length() << ", must be 3.\n";

        return -1;
      }

      // a.setCameraTarget(Eigen::Vector3f(floatTarget.at(0).toFloat(),
      //                                  floatTarget.at(1).toFloat(),
      //                                  floatTarget.at(2).toFloat()));
      optionObject.camTarget = Eigen::Vector3f(floatTarget.at(0).toFloat(),
                                               floatTarget.at(1).toFloat(),
                                               floatTarget.at(2).toFloat());
    }

    if (input.CMDOptionExists("-ca")) {
      QString camAperature = QString::fromStdString(input.getCMDOption("-ca"));

      // a.setCameraAperature(camAperature.toFloat());
      optionObject.aperature = camAperature.toFloat();
    }

    if (input.CMDOptionExists("-cfd")) {
      QString camFocalDist = QString::fromStdString(input.getCMDOption("-cfd"));

      // a.setCameraFocalDist(camFocalDist.toFloat());
      optionObject.focal = camFocalDist.toFloat();
    }

    if (input.CMDOptionExists("-cvfov")) {
      QString camFOV = QString::fromStdString(input.getCMDOption("-cvfov"));

      // a.setCameraVFOV(camFOV.toInt());
      optionObject.vFOV = camFOV.toInt();
    }

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

      modelObject.setRelativePosition(Eigen::Vector3f(-1, 4, 1));

      // TODO: Make this actually work (maybe quarternions?)
      std::cout << modelObject.getCenterOfMass() << std::endl;
      modelObject.setActualRotation(0.0, 90.0, 0.0);
      std::cout << modelObject.getCenterOfMass() << std::endl;

      std::vector<Eigen::Vector3f> modelData = modelObject.getMeshData();

      for (unsigned long i = 0; i < modelData.size(); i += 3) {
        // the model data is represented here as a densely packed vector of
        // vectors, where every 3 consecutive vectors (no offset) define a
        // triangle.  Many formats (including wavefront .obj) support mesh face
        // data for polygons, rather than just triangles, but the triangulation
        // import option from assimp should work just fine.
        Eigen::Vector3f a = modelData.at(i + 0);
        Eigen::Vector3f b = modelData.at(i + 1);
        Eigen::Vector3f c = modelData.at(i + 2);

        // optionObject.scene.push_back(std::unique_ptr<Hitable>(
        //    new Triangle(a, b, c,
        //                 new Lambertian(new ConstantTexture(
        //                     Eigen::Vector3f(0.5, 0.5, 0.5))))));
        scene.push_back(std::unique_ptr<Hitable>(
            new Triangle(a, b, c,
                         new Lambertian(new ConstantTexture(
                             Eigen::Vector3f(0.5, 0.5, 0.5))))));
      }
    }

    std::string textureFile = "earthmap.jpg";

    Texture *checker =
        new CheckerBoard(new ConstantTexture(Eigen::Vector3f(0.5, 0.2, 0.1)),
                         new ConstantTexture(Eigen::Vector3f(0.9, 0.9, 0.9)));

    // optionObject.scene.push_back(std::unique_ptr<Hitable>(new Sphere(
    //    Eigen::Vector3f(0, 0, -1), 0.5,
    //    new Lambertian(new ConstantTexture(Eigen::Vector3f(0.1, 0.5,
    //    0.6))))));
    scene.push_back(std::unique_ptr<Hitable>(new Sphere(
        Eigen::Vector3f(0, 0, -1), 0.5,
        new Lambertian(new ConstantTexture(Eigen::Vector3f(0.1, 0.5, 0.6))))));

    // optionObject.scene.push_back(std::unique_ptr<Hitable>(new Sphere(
    //    Eigen::Vector3f(0, -500.5, -1), 500, new Lambertian(checker))));
    scene.push_back(std::unique_ptr<Hitable>(new Sphere(
        Eigen::Vector3f(0, -500.5, -1), 500, new Lambertian(checker))));

    // scene.push_back(std::unique_ptr<Hitable>(
    //    new Sphere(Eigen::Vector3f(0, 1, -4.75), 2.5, new
    //    Metal(Eigen::Vector3f(0.8, 0.6, 0.2), 0.3))));
    // scene.push_back(std::unique_ptr<Hitable>(new Sphere(
    //    Eigen::Vector3f(-4, 2, 1.5), 2.5, new Lambertian(new
    //    ImageTexture(textureFile)))));
    // scene.push_back(std::unique_ptr<Hitable>(
    //     new Sphere(Eigen::Vector3f(2, 1.5, 0), 2.5, new Dielectic(1.5))));
    // scene.push_back(std::unique_ptr<Hitable>(
    //     new Sphere(Eigen::Vector3f(1, 0, -2), -0.45, new Dielectic(1.5))));
    // scene.push_back(std::unique_ptr<Hitable>(new Sphere(
    //     Eigen::Vector3f(1, 10, -1), 1.5,
    //     new DiffuseLight(new ConstantTexture(Eigen::Vector3f(3, 3, 3))))));
    // scene.push_back(std::unique_ptr<Hitable>(new Sphere(
    //     Eigen::Vector3f(1, 14, -5), 1.5,
    //     new DiffuseLight(new ConstantTexture(Eigen::Vector3f(6, 3.5, 2))))));
    // scene.push_back(std::unique_ptr<Hitable>(new Triangle(Eigen::Vector3f(0,
    // 0, 0), Eigen::Vector3f(5, 5, 5), Eigen::Vector3f(3,1,0), new
    // Metal(Eigen::Vector3f(0.1,0.2,0.3), 0.5))));
    // scene.push_back(std::unique_ptr<Hitable>(
    //     new Sphere(Eigen::Vector3f(4, 4, -3), 1.5,
    //                new DiffuseLight(new ConstantTexture(Eigen::Vector3f(2, 1,
    //                5))))));
    // a.setScene(std::move(scene));

    qDebug() << "before AppMain\n opt xRes: " << optionObject.xRes
             << "\n scene size: " << scene.size() << "\n";
    AppMain *main = new AppMain(std::move(scene), &optionObject, &w);
    // AppMain main;
    w.setCentralWidget(main);
    w.show();
    auto timeStart = std::chrono::high_resolution_clock::now();
    // a.makeRender();
    auto timeEnd = std::chrono::high_resolution_clock::now();

    auto elapsedTime = timeEnd - timeStart;

    // std::cout
    //    << "Rendering took "
    //    <<
    //    std::chrono::duration_cast<std::chrono::minutes>(elapsedTime).count()
    //    << " minutes." << std::endl;

  } else {
    std::cout << "Unrecognized option.\n \
See MVORT -h for usage details.\n1";
    return -1;
  }

  return app.exec();
}
