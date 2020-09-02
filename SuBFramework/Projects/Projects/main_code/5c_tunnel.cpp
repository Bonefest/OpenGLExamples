#include <iostream>

#include "sb7.h"
#include "object.h"
#include "Program.h"
#include "sb7ktx.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "helper.h"

using namespace sb7;

class Tunnel_App: public sb7::application {
public:

  virtual void startup() {

    m_program.loadShaders("Shaders/Chapter5/tunnel.vert", "Shaders/Chapter5/tunnel.frag");
    printf("%s\n", m_program.getError().c_str());

    initBuffers();
    initTransforms();

    glEnable(GL_DEPTH_TEST);
  }

  void initBuffers() {
    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLfloat quadVertices[] =
    {
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
       -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f,

       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenTextures(1, &m_wallTexture);
    loadTexture("resources/textures/brick.jpg", m_wallTexture, true);

    glGenTextures(1, &m_floorTexture);
    loadTexture("resources/textures/floor.jpg", m_floorTexture, true);

    glGenTextures(1, &m_ceilingTexture);
    loadTexture("resources/textures/ceiling.jpg", m_ceilingTexture, true);
  }

  void initTransforms() {
    m_projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  //m_view = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -5.0f, -5.0f));
    m_view = glm::mat4(1.0f);

    m_leftWall = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_rightWall = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_floor = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_ceiling = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  }

  virtual void render(double currentTime) {

    glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float offset = std::fmod(currentTime, 1.0f);

    m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, offset));

    m_program.useProgram();
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_view));

    glBindVertexArray(m_vao);

    int iterations = 20;

    for(int i = 0; i < iterations; ++i) {
      // Walls rendering
      glBindTexture(GL_TEXTURE_2D, m_wallTexture);

      glm::mat4 leftWall = glm::translate(glm::mat4(1.0f),
                                          glm::vec3(-0.5f, 0.0f, -i * 1.0f)) * m_leftWall;
      glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(leftWall));
      glDrawArrays(GL_TRIANGLES, 0, 6);

      glm::mat4 rightWall = glm::translate(glm::mat4(1.0f),
                                           glm::vec3( 0.5f, 0.0f, -i * 1.0f)) * m_rightWall;
      glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(rightWall));
      glDrawArrays(GL_TRIANGLES, 0, 6);

      // Floor rendering
      glBindTexture(GL_TEXTURE_2D, m_floorTexture);

      glm::mat4 floor = glm::translate(glm::mat4(1.0f),
                                       glm::vec3(0.0f, -0.5f, -i * 1.0f)) * m_floor;
      glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(floor));
      glDrawArrays(GL_TRIANGLES, 0, 6);

      // Ceiling rendering
      glBindTexture(GL_TEXTURE_2D, m_ceilingTexture);

      glm::mat4 ceiling = glm::translate(glm::mat4(1.0f),
                                         glm::vec3(0.0f, 0.5f, -i * 1.0f)) * m_ceiling;
      glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(ceiling));
      glDrawArrays(GL_TRIANGLES, 0, 6);

    }

    glBindTexture(GL_TEXTURE_2D, m_wallTexture);

    glm::mat4 end = glm::translate(glm::mat4(1.0f),
                                   glm::vec3(0.0f, 0.0f, -(iterations - 2) - offset));

    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(end));
    glDrawArrays(GL_TRIANGLES, 0, 6);

  }

  virtual void shutdown() {

  }


private:
  Program      m_program;

  glm::mat4    m_projection;
  glm::mat4    m_view;

  glm::mat4    m_leftWall;
  glm::mat4    m_rightWall;
  glm::mat4    m_floor;
  glm::mat4    m_ceiling;

  unsigned int m_vao;
  unsigned int m_vbo;

  unsigned int m_wallTexture;
  unsigned int m_floorTexture;
  unsigned int m_ceilingTexture;

};

DECLARE_MAIN(Tunnel_App);
