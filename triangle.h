#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "hitable.h"
#include "material.h"

#include <iostream>
#include <typeinfo>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

class Triangle : public Hitable {
public:
  Triangle() {}
  Triangle(Eigen::Vector3f v1, Eigen::Vector3f v2, Eigen::Vector3f v3,
           Material *a)
      : _vertex1(v1), _vertex2(v2), _vertex3(v3), mat_ptr(a){};
  virtual bool hit(const Ray &r, float tmin, float tmax, Hit_Record &rec) const;

private:
  Eigen::Vector3f _vertex1;
  Eigen::Vector3f _vertex2;
  Eigen::Vector3f _vertex3;
  Material *mat_ptr;
};

bool Triangle::hit(const Ray &r, float t_min, float t_max,
                   Hit_Record &rec) const {
  const float epsilon = 0.0000001;

  Eigen::Vector3f e1, e2, h, s, q;
  float a, f, u, v;
  Eigen::Vector3f unitDirection = r.direction();
  Eigen::Vector3f unitOrigin = r.origin();
  Eigen::Vector3f v0 = _vertex1;
  Eigen::Vector3f v1 = _vertex2;
  Eigen::Vector3f v2 = _vertex3;

  e1 = v1 - v0;
  e2 = v2 - v0;

  h = unitDirection.cross(e2);
  a = e1.dot(h);

  if (a > -epsilon && a < epsilon) {
    return false;
  }

  f = 1.0 / a;
  s = unitOrigin - v0;
  u = f * s.dot(h);

  if (u < 0.0 || u > 1.0) {
    return false;
  }

  q = s.cross(e1);
  v = f * unitDirection.dot(q);

  if (v < 0.0 || u + v > 1.0) {
    return false;
  }

  float t = f * e2.dot(q);

  if (t > epsilon) {
    if (t < t_max && t > t_min) {
      // out at r.origin() + r.direction() * t;
      rec.mat_ptr = mat_ptr;
      rec.t = t;
      rec.normal = e1.cross(e2);
      return true;
    }
  }

  return false;
}

struct Vertex {
  Eigen::Vector3f position;
  Eigen::Vector3f normal;
  float u;
  float v;
};

struct TexturesType {
  uint id;
  std::string type;
};

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<uint> indeces,
       std::vector<TexturesType> textures)
      : _vertices(vertices), _indeces(indeces), _textures(textures) {}

  std::vector<Vertex> _vertices;
  std::vector<uint> _indeces;
  std::vector<TexturesType> _textures;

private:
};

class Model {
public:
  Model(std::string &file) : _meshFile(file) {
    Assimp::Importer importer;
    const aiScene *meshScene = importer.ReadFile(
        _meshFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                       aiProcess_SortByPType | aiProcess_GenNormals);
    _scene = meshScene;
    if (!meshScene || meshScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !meshScene->mRootNode) {
      std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;

      std::exit(-1);
    }

    processNode(meshScene->mRootNode, meshScene);

    for (auto const &i : _meshes) {
      Mesh currentMesh = i;
      for (unsigned long int j = 0; j < currentMesh._indeces.size(); j += 3) {
        _meshData.push_back(
            currentMesh._vertices.at(currentMesh._indeces.at(j + 0)).position);
        _meshData.push_back(
            currentMesh._vertices.at(currentMesh._indeces.at(j + 1)).position);
        _meshData.push_back(
            currentMesh._vertices.at(currentMesh._indeces.at(j + 2)).position);
      }
    }
  }

  void print() {}

  // Right now the object is rotated from the axis, regardless of
  // where it is in the scene, thus making it unusable.  The idea
  // of rotation is rotation the object itself (as if it were
  // centered).  We are not going to go into arbitrary axis
  // rotations as that sounds too damn complicated.  Instead the
  // plan is:
  //  1) Translate the center of the mesh to (0,0,0)
  //  2) Do the rotation
  //  3) Translate back to the original position
  void setActualRotation(float x, float y, float z) {
    Eigen::Vector3f originLoc = getCenterOfMass();
    Eigen::Vector3f newLoc = -1.0 * originLoc;

    setRotationTranslation(x, y, z, newLoc);
    setRelativePosition(originLoc);
  }

  Eigen::Vector3f getCenterOfMass() {
    float minX, maxX, maxY, minY, maxZ, minZ;

    std::vector<Eigen::Vector3f>::iterator iter_minX = std::min_element(
        _meshData.begin(), _meshData.end(),
        [](const Eigen::Vector3f &a, const Eigen::Vector3f &b) -> bool {
          return a.x() < b.x();
        });
    std::vector<Eigen::Vector3f>::iterator iter_maxX = std::max_element(
        _meshData.begin(), _meshData.end(),
        [](const Eigen::Vector3f &a, const Eigen::Vector3f &b) -> bool {
          return a.x() > b.x();
        });
    minX = iter_minX->x();
    maxX = iter_maxX->x();

    std::vector<Eigen::Vector3f>::iterator iter_minY = std::min_element(
        _meshData.begin(), _meshData.end(),
        [](const Eigen::Vector3f &a, const Eigen::Vector3f &b) -> bool {
          return a.y() < b.y();
        });
    std::vector<Eigen::Vector3f>::iterator iter_maxY = std::max_element(
        _meshData.begin(), _meshData.end(),
        [](const Eigen::Vector3f &a, const Eigen::Vector3f &b) -> bool {
          return a.y() > b.y();
        });
    minY = iter_minY->y();
    maxY = iter_maxY->y();

    std::vector<Eigen::Vector3f>::iterator iter_minZ = std::min_element(
        _meshData.begin(), _meshData.end(),
        [](const Eigen::Vector3f &a, const Eigen::Vector3f &b) -> bool {
          return a.z() < b.z();
        });
    std::vector<Eigen::Vector3f>::iterator iter_maxZ = std::max_element(
        _meshData.begin(), _meshData.end(),
        [](const Eigen::Vector3f &a, const Eigen::Vector3f &b) -> bool {
          return a.z() > b.z();
        });
    minZ = iter_minZ->z();
    maxZ = iter_maxZ->z();

    float y = (minY + maxY) / 2.0;
    float x = (minX + maxX) / 2.0;
    float z = (minZ + maxZ) / 2.0;

    return Eigen::Vector3f(x, y, z);
  }

  void setRelativePosition(Eigen::Vector3f input) {
    setRotationTranslation(0.0, 0.0, 0.0, input);
  }

  void setRotation(float x, float y, float z) {
    setRotationTranslation(x, y, z, Eigen::Vector3f(0, 0, 0));
  }

  void setRotationTranslation(float x, float y, float z, Eigen::Vector3f tran) {
    // TODO: figure out how homogenous coordinates work, and how to get
    // world absolute (rather than relative) coordinates into the matrix
    // currently this will move, and rotate your object in a sort of
    // unintuitive way (movement is relative, so is rotation about an axis)
    Eigen::Matrix4f rotMatrix =
        createAffineMatrix(x * M_PI, y * M_PI, z * M_PI, tran);
    Eigen::Affine3f affineRotation;
    affineRotation.matrix() = rotMatrix;

    std::cout << rotMatrix << std::endl;
    for (unsigned long i = 0; i < _meshData.size(); i += 3) {
      _meshData.at(i + 0) = affineRotation.linear() * _meshData.at(i + 0);
      _meshData.at(i + 1) = affineRotation.linear() * _meshData.at(i + 1);
      _meshData.at(i + 2) = affineRotation.linear() * _meshData.at(i + 2);
    }
  }

  std::vector<Eigen::Vector3f> getMeshData() { return _meshData; }

private:
  std::vector<Eigen::Vector3f> _meshData;
  const aiScene *_scene;
  std::vector<Mesh> _meshes;
  std::string _meshFile;

  Eigen::Matrix4f createAffineMatrix(float x, float y, float z,
                                     Eigen::Vector3f translationVector) {
    Eigen::Transform<float, 3, Eigen::Affine> t;
    t = Eigen::Translation<float, 3>(translationVector);
    t.rotate(Eigen::AngleAxis<float>(x, Eigen::Vector3f::UnitX()));
    t.rotate(Eigen::AngleAxis<float>(y, Eigen::Vector3f::UnitY()));
    t.rotate(Eigen::AngleAxis<float>(z, Eigen::Vector3f::UnitZ()));

    return t.matrix();
  }

  void processNode(aiNode *node, const aiScene *scene) {
    for (uint i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      _meshes.push_back(processMesh(mesh, scene));
    }

    for (uint i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> verteces;
    std::vector<uint> index;
    std::vector<TexturesType> textures;

    for (uint i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;

      Eigen::Vector3f pos(mesh->mVertices[i].x, mesh->mVertices[i].y,
                          mesh->mVertices[i].z);

      vertex.position = pos;

      Eigen::Vector3f nom(mesh->mNormals[i].x, mesh->mNormals[i].y,
                          mesh->mNormals[i].z);

      vertex.normal = nom;

      if (mesh->mTextureCoords[0]) {
        vertex.u = mesh->mTextureCoords[0][i].x;
        vertex.v = mesh->mTextureCoords[0][i].y;
      } else {
        vertex.u = 0.0;
        vertex.v = 0.0;
      }

      verteces.push_back(vertex);
    }

    for (uint i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];

      for (uint j = 0; j < face.mNumIndices; j++) {
        index.push_back(face.mIndices[j]);
      }
    }

    return Mesh(verteces, index, textures);
  }
};

#endif
