#include <iostream>

#include "sb7.h"
#include "object.h"
#include "Program.h"
#include "sb7ktx.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Loader.h"

using namespace sb7;

class Torus_App: public sb7::application {
public:

  virtual void startup() {

    m_program.loadShaders("Shaders/Chapter5/torus.vert", "Shaders/Chapter5/torus.frag");
    printf("%s\n", m_program.getError().c_str());
    m_torusObject.load("resources/objects/torus2.obj", "resources/textures/chess.jpg");

    m_projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    m_view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);
  }

  virtual void render(double currentTime) {

    glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));

    m_program.useProgram();
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(model));

    m_torusObject.draw(m_program.getProgramID());

  }

  virtual void shutdown() {

  }


private:
  Program      m_program;
  ObjectModel  m_torusObject;

  glm::mat4    m_projection;
  glm::mat4    m_view;
};

DECLARE_MAIN(Torus_App);
