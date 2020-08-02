#include <iostream>

#include "sb7.h"
#include "Program.h"

class BufferTest: public sb7::application {
public:

  virtual void startup() {

    if(!m_program.loadShaders("Shaders/Chapter5/vao_buffer.vert",
                              "Shaders/Chapter5/vao_buffer.frag")) {

      std::cout << "Error:" << m_program.getError() << '\n';
    }


    float vertices[] = {
                       -0.5f,  0.0f, -0.5f,
                        0.0f,  0.5f, -0.5f,
                        0.5f,  0.0f, -0.5f
    };


    glCreateBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    // glVertexAttribBinding(m_vao, 0, 0);
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(float) * 3);
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_vao, 0);

  }

  virtual void render(double currentTime) {

    GLfloat color[] = {1.0, 1.0, 1.0, 1.0};

    glClearBufferfv(GL_COLOR, 0, color);

    m_program.useProgram();

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

  }

  virtual void shutdown() {

    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);

  }


private:
  unsigned int m_vao;
  unsigned int m_vbo;
  Program      m_program;
  Program      m_displayProgram;

};

DECLARE_MAIN(BufferTest)
