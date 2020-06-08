#include "helper.h"
#include "stb_image.h"

#include <iostream>

bool loadTexture(const std::string& path,
                 unsigned int target,
                 GLint internalFormat, GLenum format,
                 bool generateMipmap) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if(data == NULL) {
        std::cerr << "error!" << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, target);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    if(generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    return true;
}

glm::mat4 eulerRotate(glm::mat4 mat, glm::vec3 angles) {
    glm::mat4 result = mat;
    result = glm::rotate(result, angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
    result = glm::rotate(result, angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
    result = glm::rotate(result, angles.z, glm::vec3(0.0f, 0.0f, 1.0f));

    return result;
}
