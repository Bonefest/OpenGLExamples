#include <iostream>

#include "sb7.h"

class PointApp: public sb7::application {
public:

    virtual void startup() {

        m_program = createProgram();
        glCreateVertexArrays(1, &m_vao);
        glPointSize(32.0f);
    }

    virtual void render(double currentTime) {

        GLfloat color[] = {1.0, 1.0, 1.0, 1.0};
        color[0] = std::cos(currentTime * 0.5f) * 0.5f + 0.5f;
        color[2] = std::sin(currentTime * 0.5f) * 0.5f + 0.5f;

        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(m_program);
        glBindVertexArray(m_vao);
        glDrawArrays(GL_POINTS, 0, 1);
    }

    virtual void shutdown() {

        glDeleteVertexArrays(1, &m_vao);
        glDeleteProgram(m_program);
    }

    unsigned int createProgram() {

        unsigned int vertex, fragment;

        const char* vertexCode[] = {
            "#version 330 core                                   \n"
            "void main() {                                      \n"
            "   gl_Position = vec4(0.0f, 0.0f, 0.5f, 1.0f);     \n"
            "}                                                  \n"
        };

        const char* fragmentCode[] = {
            "#version 330 core                                   \n"
            "out vec4 outputColor;                              \n"
            "void main() {                                      \n"
            "   outputColor = vec4(0.8f, 0.1f, 0.9f, 1.0f);     \n"
            "}                                                  \n"
        };

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, vertexCode, NULL);
        glCompileShader(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, fragmentCode, NULL);
        glCompileShader(fragment);

        unsigned int program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return program;
    }

private:
    unsigned int    m_program;
    unsigned int    m_vao;

};

//DECLARE_MAIN(PointApp)
