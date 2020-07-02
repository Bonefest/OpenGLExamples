#include <iostream>

#include "sb7.h"
#include "Program.h"

class AttributeTestApp: public sb7::application {
public:

    virtual void startup() {

        if(!m_program.loadShaders("Shaders/Chapter3/attribute.vert",
                                  "Shaders/Chapter3/attribute.frag")) {

            std::cout << "Error:" << m_program.getError() << '\n';
        }


        glCreateVertexArrays(1, &m_vao);
        glPointSize(32.0f);
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
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    virtual void shutdown() {

        glDeleteVertexArrays(1, &m_vao);
    }


private:
    unsigned int m_vao;
    Program      m_program;

};

DECLARE_MAIN(AttributeTestApp)
