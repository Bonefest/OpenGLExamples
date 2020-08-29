#include <iostream>

#include "sb7.h"
#include "object.h"
#include "Program.h"
#include "sb7ktx.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace sb7;

class Torus_App: public sb7::application {
public:

  virtual void startup() {

    m_program.loadShaders("Shaders/Chapter5/torus.vert", "Shaders/Chapter5/torus.frag");
    m_torusObject.load("resources/objects/dragon.sbm");

    m_projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    m_view = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);

    glGenTextures(1, &m_texture);
    sb7::ktx::file::load("resources/textures/pattern1.ktx", m_texture);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  virtual void render(double currentTime) {

    GLfloat color[] = {1.0, 1.0, 1.0, 1.0};
    glClearBufferfv(GL_COLOR, 0, color);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));

    m_program.useProgram();
    glUniformMatrix4fv(glGetUniformLocation(m_program.getProgramID(), "proj"),
                       1, GL_FALSE, glm::value_ptr(m_projection));

    glUniformMatrix4fv(glGetUniformLocation(m_program.getProgramID(), "view"),
                       1, GL_FALSE, glm::value_ptr(m_view));

    glUniformMatrix4fv(glGetUniformLocation(m_program.getProgramID(), "model"),
                       1, GL_FALSE, glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(glGetUniformLocation(m_program.getProgramID(), "texture1"), 0);

    m_torusObject.render();
    glBindVertexArray(m_torusObject.get_vao());
    glDrawArrays(GL_TRIANGLES, 0, 3);

  }

  virtual void shutdown() {

    m_torusObject.free();

  }


private:
  Program      m_program;
  object  m_torusObject;

  glm::mat4    m_projection;
  glm::mat4    m_view;

  GLuint       m_texture;
};

DECLARE_MAIN(Torus_App);
