#include <iostream>

#include "sb7.h"
#include "object.h"
#include "Program.h"
#include "sb7ktx.h"
#include "vmath.h"

using namespace sb7;

struct Alien {
  float offsetX;
  float offsetY;
  float orientation;
  float speed;
  float angleSpeed;
};

class Aliens_App: public sb7::application {
public:

  virtual void startup() {

    m_program.loadShaders("Shaders/Chapter5/aliens.vert", "Shaders/Chapter5/aliens.frag");
    printf("%s\n", m_program.getError().c_str());

    m_aliensCount = 256;

    initBuffers();
    initAliens();

  }

  void initBuffers() {
    glCreateVertexArrays(1, &m_vao);

    glGenBuffers(1, &m_aliensUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_aliensUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(vmath::vec4) * m_aliensCount, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_aliensUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_aliensTexture = sb7::ktx::file::load("resources/textures/aliens.ktx");
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_aliensTexture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void initAliens() {
    m_aliens = new Alien[m_aliensCount];
    for(int i = 0;i < m_aliensCount; ++i) {
      m_aliens[i].offsetX = drand48() * 2.0 - 1.0f;
      m_aliens[i].offsetY = drand48() - 0.5f;
      m_aliens[i].orientation = drand48() * 6.28;
      m_aliens[i].speed = drand48() * 0.15 + 0.1;
      m_aliens[i].angleSpeed = drand48();
    }
  }

  virtual void render(double currentTime) {
    GLfloat color[] = {0.0, 0.0, 0.0, 0.0};
    glClearBufferfv(GL_COLOR, 0, color);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_aliensUBO);
    vmath::vec4* aliensMap = (vmath::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER,
                                                           0,
                                                           m_aliensCount * sizeof(vmath::vec4),
                                                           GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    for(int i = 0; i < m_aliensCount; ++i) {
      aliensMap[i][0] = m_aliens[i].offsetX;
      aliensMap[i][1] = 1.25 - std::fmod(m_aliens[i].offsetY + m_aliens[i].speed * currentTime, 2.5f);
      aliensMap[i][2] = m_aliens[i].orientation + m_aliens[i].angleSpeed * currentTime;
    }

    glUnmapBuffer(GL_UNIFORM_BUFFER);

    m_program.useProgram();
    glBindVertexArray(m_vao);
    for(int i = 0; i < m_aliensCount; ++i) {
      glVertexAttribI1i(0, i);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

  }

  virtual void shutdown() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_aliensTexture);

    delete [] m_aliens;
  }

private:
  Program      m_program;

  unsigned int m_vao;
  unsigned int m_aliensUBO;
  unsigned int m_aliensCount;
  unsigned int m_aliensTexture;

  Alien*       m_aliens;

};

DECLARE_MAIN(Aliens_App);
