#include <iostream>

#include "sb7.h"
#include "Program.h"

class AttributeTestApp: public sb7::application {
public:

  virtual void startup() {

    if(!m_program.loadShaders("Shaders/Chapter5/curve.vert",
                              "Shaders/Chapter5/curve.frag")) {

      std::cout << "Error:" << m_program.getError() << '\n';
    }

    if(!m_displayProgram.loadShaders("Shaders/Chapter5/curve_point.vert",
                                     "Shaders/Chapter5/curve.frag")) {

      std::cout << "Error:" << m_displayProgram.getError() << '\n';
    }


    glCreateVertexArrays(1, &m_vao);
    glPointSize(16.0f);

    m_iterationsNumber = 100;
  }

  virtual void render(double currentTime) {

    GLfloat color[] = {1.0, 1.0, 1.0, 1.0};

    glClearBufferfv(GL_COLOR, 0, color);


    float vertices[] = {
       -0.5f,  0.0f, -0.5f,
        0.0f,  0.5f, -0.5f,
        0.5f,  0.0f, -0.5f,
        0.0f, -0.5f, -0.5f
    };

    m_program.useProgram();

    glUniform3fv(glGetUniformLocation(m_program.getProgramID(), "vertices"),
                 4, vertices);


    glUniform1i(glGetUniformLocation(m_program.getProgramID(), "vertices_number"),
                m_iterationsNumber);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, m_iterationsNumber);

    // Curve points displaying
    m_displayProgram.useProgram();

    glUniform3fv(glGetUniformLocation(m_displayProgram.getProgramID(), "vertices"),
                 4, vertices);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, 4);

  }

  virtual void shutdown() {

    glDeleteVertexArrays(1, &m_vao);
  }


private:
  unsigned int m_vao;
  Program      m_program;
  Program      m_displayProgram;

  unsigned int m_iterationsNumber;
};

DECLARE_MAIN(AttributeTestApp)
