#include <iostream>

#include "sb7.h"
#include "Program.h"

class SAO_AOS_App: public sb7::application {
public:

  virtual void startup() {

    if(!m_program.loadShaders("Shaders/Chapter5/soa_sao.vert",
                              "Shaders/Chapter5/soa_sao.frag")) {

      std::cout << "Error:" << m_program.getError() << '\n';
    }


    float vertices[] = {
                       -0.5f,  0.0f, -0.5f,
                        0.0f,  0.5f, -0.5f,
                        0.5f,  0.0f, -0.5f
    };

    float colors[] = {
                      1.0f,  0.0f,  0.0f,
                      0.0f,  1.0f,  0.0f,
                      0.0f,  0.0f,  1.0f
    };

    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // glCreateBuffers(2, m_sao);
    // glNamedBufferStorage(m_sao[0], sizeof(vertices), vertices, 0);
    // glVertexArrayVertexBuffer(m_vao, 0, m_sao[0], 0, sizeof(float) * 3);
    // glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // glVertexArrayAttribBinding(m_vao, 0, 0);
    // glEnableVertexArrayAttrib(m_vao, 0);

    // glNamedBufferStorage(m_sao[1], sizeof(colors), colors, 0);
    // glVertexArrayVertexBuffer(m_vao, 1, m_sao[1], 0, sizeof(float) * 3);
    // glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
    // glVertexArrayAttribBinding(m_vao, 1, 1);
    // glEnableVertexArrayAttrib(m_vao, 1);


    float vertices_and_colors[] = {
      -0.5f,  0.0f, -0.5f,  1.0f, 0.0f, 0.0f,
       0.0f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
       0.5f,  0.0f, -0.5f,  0.0f, 0.0f, 1.0f
    };

    glCreateBuffers(1, &m_aos);
    glNamedBufferStorage(m_aos, sizeof(vertices_and_colors), vertices_and_colors, 0);
    glVertexArrayVertexBuffer(m_vao, 0, m_aos, 0, sizeof(float) * 6);
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glEnableVertexArrayAttrib(m_vao, 0);

    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glEnableVertexArrayAttrib(m_vao, 1);

  }

  virtual void render(double currentTime) {

    GLfloat color[] = {1.0, 1.0, 1.0, 1.0};

    glClearBufferfv(GL_COLOR, 0, color);

    m_program.useProgram();

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

  }

  virtual void shutdown() {

    glDeleteBuffers(2, m_sao);
    glDeleteVertexArrays(1, &m_vao);

  }


private:
  unsigned int m_vao;
  unsigned int m_sao[2];
  unsigned int m_aos;
  Program      m_program;
  Program      m_displayProgram;
};

//DECLARE_MAIN(SAO_AOS_App)
