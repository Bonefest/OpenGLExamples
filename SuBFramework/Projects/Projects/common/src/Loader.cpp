#include "Loader.h"
#include "GL/gl3w.h"

#include <vector>

#include "helper.h"

ObjectModel::~ObjectModel() {
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
}

bool ObjectModel::load(const char* filename, const char* textureName) {

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texturePositions;

  std::vector<FaceUnit> faces;

  char lineBuffer[1024];
  FILE* objectFile = fopen(filename, "r");
  if(objectFile == nullptr) {
    return false;
  }

  bool hasError = false;

  while(fgets(lineBuffer, 1023, objectFile) != NULL && !hasError) {

    char prefix[128];
    sscanf(lineBuffer, "%s", prefix);
    if(strcmp(prefix, "#") == 0) {
      fprintf(stderr, "%s", lineBuffer);
    }
    else if(strcmp(prefix, "v") == 0) {
      glm::vec3 position;
      if(sscanf(lineBuffer + 2, "%f %f %f", &position[0], &position[1], &position[2]) != 3) {
        fprintf(stderr, "[Error] Cannot read %s\n", lineBuffer);
        hasError = true;
        break;
      }

      positions.push_back(position);
    }
    else if(strcmp(prefix, "vt") == 0) {
      glm::vec2 texturePosition;
      if(sscanf(lineBuffer + 3, "%f %f", &texturePosition[0], &texturePosition[1]) != 2) {
        fprintf(stderr, "[Error] Cannot read %s\n", lineBuffer);
        hasError = true;
        break;
      }

      texturePositions.push_back(texturePosition);
    }
    else if(strcmp(prefix, "vn") == 0) {
      glm::vec3 normal;
      if(sscanf(lineBuffer + 3, "%f %f %f", &normal[0], &normal[1], &normal[2]) != 3) {
        fprintf(stderr, "[Error] Cannot read %s\n", lineBuffer);
        hasError = true;
        break;
      }

      normals.push_back(normal);
    }
    else if(strcmp(prefix, "f") == 0) {
      FaceUnit u{};
      u.quadrilateral = false;

      if(sscanf(lineBuffer + 2, "%u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u ",
                &u.vp1, &u.vt1, &u.vn1,
                &u.vp2, &u.vt2, &u.vn2,
                &u.vp3, &u.vt3, &u.vn3,
                &u.vp4, &u.vt4, &u.vn4) == 12) {
        u.quadrilateral = true;
      }

      faces.push_back(u);

    }

  }
  auto readVertex = [&](unsigned int* data) {
                      Vertex vertex;
                      vertex.position = positions[data[0] - 1];
                      if(data[1] > 0) {
                        vertex.texturePosition = texturePositions[data[1] - 1];
                      }

                      if(data[2] > 0) {
                        vertex.normal = normals[data[2] - 1];
                      }

                      return vertex;
                    };
  std::vector<Vertex> result;
  for(auto& face: faces) {

    result.push_back(readVertex(face.idx + 0));
    result.push_back(readVertex(face.idx + 3));
    result.push_back(readVertex(face.idx + 6));

    if(face.quadrilateral) {
      result.push_back(readVertex(face.idx + 0));
      result.push_back(readVertex(face.idx + 6));
      result.push_back(readVertex(face.idx + 9));
    }

  }

  if(hasError) return false;

  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, result.size() * sizeof(Vertex), &result[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(offsetof(Vertex, position)));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(offsetof(Vertex, texturePosition)));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(offsetof(Vertex, normal)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  m_verticesNumber = result.size();

  return loadTexture(textureName);

}

bool ObjectModel::loadTexture(const std::string& name) {
  glDeleteTextures(1, &m_texture);
  glGenTextures(1, &m_texture);
  return ::loadTexture(name, m_texture, true);
}

void ObjectModel::draw(unsigned int programID) {
  glBindVertexArray(m_VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glUniform1i(glGetUniformLocation(programID, "texture1"), 0);
  glDrawArrays(GL_TRIANGLES, 0, m_verticesNumber);
  glBindVertexArray(0);
}
