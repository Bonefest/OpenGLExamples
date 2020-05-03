#include "helper.h"
#include "stb_image.h"

#include <iostream>

GLuint loadTexture(const std::string& path,
                   GLint internalFormat, GLenum format,
                   bool generateMipmap) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if(data == NULL) {
        std::cerr << "error!" << std::endl;
    }

    std::cout << path << std::endl;

    GLuint textureBuffer;
    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_TEXTURE_2D, textureBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    if(generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindBuffer(GL_TEXTURE_2D, 0);

    //stbi_image_free(data);

    return textureBuffer;
}
