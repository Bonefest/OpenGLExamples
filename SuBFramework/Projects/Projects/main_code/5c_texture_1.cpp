#include <iostream>

#include "sb7.h"
#include "Program.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Texture_1_App: public sb7::application {
public:

  virtual void startup() {
    glViewport(0, 0, 256, 256);

    m_program.loadShaders("Shaders/Chapter5/texture_1.vert", "Shaders/Chapter5/texture_1.frag");

    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    float vertices[] = {
      -1.0f, -1.0f, -0.5f,
       1.0f, -1.0f, -0.5f,
       1.0f,  1.0f, -0.5f,

       1.0f,  1.0f, -0.5f,
      -1.0f,  1.0f, -0.5f,
      -1.0f, -1.0f, -0.5f
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(float) * 3);
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 0);
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glEnableVertexArrayAttrib(m_vao, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    int width = 256;
    int height = 256;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
    glTextureStorage2D(m_texture, 1, GL_RGBA32F, width, height);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);


    float* data = new float[width * height * 4];
    memset(data, 1, sizeof(float) * width * height * 4);

    generateTexture(data, width, height);

    glTextureSubImage2D(m_texture, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, data);

    delete [] data;

  }

  virtual void render(double currentTime) {

    GLfloat color[] = {1.0, 1.0, 1.0, 1.0};

    glClearBufferfv(GL_COLOR, 0, color);

    glBindVertexArray(m_vao);

    m_program.useProgram();

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  virtual void shutdown() {

    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);

  }

  void generateTexture(float* data, int width, int height) {
    recurDrawing(data, width, height, width, height, 0, 0);
  }

private:
    void recurDrawing(float* data,
                      int texWidth, int texHeight, int width, int height,
                      int offsetX, int offsetY) {
      if(width > 2 && height > 2) {
        fillRect(data, offsetX + width / 4, offsetY + height / 4,
                 texWidth, texHeight, width / 2, height / 2,
                 0.5f, 0.5f, 0.5f);

        recurDrawing(data, texWidth, texHeight, width / 4, height / 4,
                     offsetX, offsetY);
        recurDrawing(data, texWidth, texHeight, width / 4, height / 4,
                     offsetX + 3 * (width / 4), offsetY);
        recurDrawing(data, texWidth, texHeight, width / 4, height / 4,
                     offsetX, offsetY + 3 * (height / 4));
        recurDrawing(data, texWidth, texHeight, width / 4, height / 4,
                     offsetX + 3 * (width / 4), offsetY + 3 * (height / 4));
      }
    }

    void fillRect(float* data, int offsetX, int offsetY,
                  int texWidth, int texHeight, int width, int height,
                  float r, float g, float b) {

      float* row = data + offsetY * texWidth * 4;
      for(int y = 0; y < height; ++y) {

        float* pixel = row + offsetX * 4;

        for(int x = 0; x < width; ++x) {
          pixel[0] = r;
          pixel[1] = g;
          pixel[2] = b;
          pixel[3] = 1.0;

          pixel += 4;
        }

        row += texWidth * 4;
      }



    }

  unsigned int m_vao;
  unsigned int m_vbo;
  unsigned int m_texture;

  Program      m_program;
};

DECLARE_MAIN(Texture_1_App);
