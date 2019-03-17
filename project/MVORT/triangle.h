#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "hitable.h"
#include "material.h"

#include <typeinfo>

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


class Triangle : public Hitable {
 public:
  Triangle() {}
 Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Material *a) : _vertex1(v1), _vertex2(v2), _vertex3(v3), mat_ptr(a) {};
  virtual bool hit(const Ray &r, float tmin, float tmax, Hit_Record &rec) const;

 private:
  Vec3 _vertex1;
  Vec3 _vertex2;
  Vec3 _vertex3;
  Material *mat_ptr;
};

bool Triangle::hit(const Ray &r, float t_min, float t_max,
                   Hit_Record &rec) const {
  const float epsilon = 0.0000001;

  Vec3 e1, e2, h, s, q;
  float a, f, u, v;
  Vec3 unitDirection = r.direction();
  Vec3 unitOrigin = r.origin();
  Vec3 v0 = _vertex1;
  Vec3 v1 = _vertex2;
  Vec3 v2 = _vertex3;

  e1 = v1 - v0;
  e2 = v2 - v0;

  h = cross(unitDirection, e2);
  a = dot(e1, h);

  if (a > -epsilon && a < epsilon) {
    return false;
  }

  f = 1.0 / a;
  s = unitOrigin - v0;
  u = f * dot(s, h);

  if (u < 0.0 || u > 1.0) {
    return false;
  }

  q = cross(s, e1);
  v = f * dot(unitDirection, q);

  if (v < 0.0 || u + v > 1.0) {
    return false;
  }

  float t = f * dot(e2, q);

  if (t > epsilon) {
    if (t < t_max && t > t_min) {
      // out at r.origin() + r.direction() * t;
      rec.mat_ptr = mat_ptr;
      rec.t = t;
      rec.normal = cross(e1, e2);
      return true;
    }
  }

  return false;
}

struct Vertex {
  Vec3 position;
  Vec3 normal;
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
    if (!meshScene || meshScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !meshScene->mRootNode) {
      std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;

      std::exit(-1);
    }

    processNode(meshScene->mRootNode, meshScene);
  }

  void print() {
    Mesh printMe = _meshes.at(0);

    std::cout << printMe._vertices.size() << "\n"
              << printMe._indeces.size() << "\n";

    for (unsigned long int i = 0; i < printMe._vertices.size(); i++) {
      float x = printMe._vertices.at(i).position.x();
      float y = printMe._vertices.at(i).position.y();
      float z = printMe._vertices.at(i).position.z();

      std::cout << "{" << x << ", " << y << ", " << z << "}, Index: " << printMe._indeces.at(i) << "\n";
    }

    // for(unsigned long int i = 0; i < printMe._indeces.size(); i++) {
    //   std::cout << "Index at " << i << ": " << printMe._indeces.at(i) << "\n";
    // }
  }

private:
  std::vector<Mesh> _meshes;
  std::string _meshFile;

  void processNode(aiNode *node, const aiScene *scene) {
    for (uint i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      _meshes.push_back(processMesh(mesh, scene));
    }

    for(uint i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> verteces;
    std::vector<uint> index;
    std::vector<TexturesType> textures;

    for(uint i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;

      Vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y,
               mesh->mVertices[i].z);

      vertex.position = pos;

      Vec3 nom(mesh->mNormals[i].x, mesh->mNormals[i].y,
               mesh->mNormals[i].z);

      vertex.normal = nom;

      if(mesh->mTextureCoords[0]) {
        vertex.u = mesh->mTextureCoords[0][i].x;
        vertex.v = mesh->mTextureCoords[0][i].y;
      } else {
        vertex.u = 0.0;
        vertex.v = 0.0;
      }

      verteces.push_back(vertex);
    }

    for(uint i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];

      for(uint j = 0; j < face.mNumIndices; j++) {
        index.push_back(face.mIndices[j]);
      }
    }

    return Mesh(verteces, index, textures);
  }
};

#endif
