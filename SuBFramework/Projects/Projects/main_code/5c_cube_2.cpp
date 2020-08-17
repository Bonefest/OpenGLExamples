#include <iostream>

#include "sb7.h"
#include "Program.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Cube_2_App: public sb7::application {
public:

  virtual void startup() {

    onResize(640, 480);
    m_program.loadShaders("Shaders/Chapter5/cube.vert", "Shaders/Chapter5/cube_1.frag");

    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
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

    glEnable(GL_DEPTH_TEST);
  }

  virtual void render(double currentTime) {

    GLfloat color[] = {1.0, 1.0, 1.0, 1.0};

    glClearBufferfv(GL_COLOR, 0, color);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::mat4(1.0f);

    for(int i = 0; i < 10; ++i) {

      glm::mat4 model = glm::mat4(1.0f);

      float param = currentTime * 0.2f;

      model = glm::translate(model, glm::vec3(i * 0.3f, 0.0f, -i * 0.5f));

      model = glm::translate(model, glm::vec3(sinf(param * 1.3f),
                                              cosf(param * 1.72f),
                                              sinf(param * 1.19f) * cosf(param * 1.33f) - 5.0f));

      model = glm::rotate(model, (float)currentTime, glm::vec3(-1.0f, 1.0f, float(i) / 10.0f));

      glUniformMatrix4fv(glGetUniformLocation(m_program.getProgramID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

      glUniformMatrix4fv(glGetUniformLocation(m_program.getProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

      glUniformMatrix4fv(glGetUniformLocation(m_program.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

      glBindVertexArray(m_vao);

      m_program.useProgram();

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
  }

  virtual void onResize(int w, int h) {
    sb7::application::onResize(w, h);

    m_aspect = float(info.windowWidth) / float(info.windowHeight);
    m_projection = glm::perspective(glm::radians(45.0f), m_aspect, 0.1f, 100.0f);

    GLint viewportData[4];
    glGetIntegerv(GL_VIEWPORT, viewportData);

    glViewport(viewportData[0], viewportData[1], w, h);

  }

  virtual void shutdown() {

    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);

  }


private:
  unsigned int m_vao;
  unsigned int m_vbo;

  Program      m_program;

  glm::mat4    m_projection;
  float        m_aspect;
};

DECLARE_MAIN(Cube_2_App);
