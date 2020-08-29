#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

struct Vertex {

  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texturePosition;

};

struct FaceUnit {

 union {

    unsigned int idx[12];

    struct {
      unsigned int
        vp1, vt1, vn1,
        vp2, vt2, vn2,
        vp3, vt3, vn3,
        vp4, vt4, vn4;
    };
  };

  bool quadrilateral;

};

class ObjectModel {
public:

  ~ObjectModel();
  bool load(const char* filename, const char* textureName);
  bool loadTexture(const std::string& name);
  void draw(unsigned int programID);

private:
  unsigned int m_VBO{};
  unsigned int m_VAO{};

  unsigned int m_verticesNumber;
  unsigned int m_texture;
};

#endif
