#include <iostream>

#include "sb7.h"
#include "Program.h"

class TesselationApp: public sb7::application {
public:

    virtual void startup() {

        m_program.reset();
        if(!m_program.loadShader("Shaders/Chapter3/attribute.vert", GL_VERTEX_SHADER) ||
           !m_program.loadShader("Shaders/Chapter3/attribute.frag", GL_FRAGMENT_SHADER) ||
           !m_program.loadShader("Shaders/Chapter3/tessellation.tcs", GL_TESS_CONTROL_SHADER) ||
           !m_program.loadShader("Shaders/Chapter3/tessellation.tes", GL_TESS_EVALUATION_SHADER) ||
           !m_program.link()) {

            std::cout << m_program.getError();

        }

        glPatchParameteri(GL_PATCH_VERTICES, 3);

        glLineWidth(4.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glCreateVertexArrays(1, &m_vao);
    }

    virtual void render(double currentTime) {

        GLfloat color[] = {1.0, 1.0, 1.0, 1.0};

        glClearBufferfv(GL_COLOR, 0, color);

        color[0] = std::cos(currentTime * 0.5f) * 0.5f + 0.5f;
        color[2] = std::sin(currentTime * 0.5f) * 0.5f + 0.5f;

        m_program.useProgram();

        glVertexAttrib3f(0, 0.1f, 0.25f, 0.0f);
        glVertexAttrib3fv(1, color);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_PATCHES, 0, 3);
    }

    virtual void shutdown() {

        glDeleteVertexArrays(1, &m_vao);
    }


private:
    unsigned int m_vao;
    Program      m_program;

};

DECLARE_MAIN(TesselationApp);
