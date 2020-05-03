#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

bool loadTexture(const std::string& path,
                 unsigned int target,
                 GLint internalFormat, GLenum format,
                 bool generateMipmap = true);

#endif // HELPER_H_INCLUDED
