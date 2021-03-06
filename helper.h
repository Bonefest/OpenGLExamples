#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool loadTexture(const std::string& path,
                 unsigned int target,
                 GLint internalFormat, GLenum format,
                 bool generateMipmap = true);

glm::mat4 eulerRotate(glm::mat4 mat, glm::vec3 angles);

#endif // HELPER_H_INCLUDED
