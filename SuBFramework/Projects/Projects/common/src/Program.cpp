#include "../Program.h"
#include <fstream>
#include <sstream>

Program::Program(): m_programID(0), m_lastMessageBuffer("Empty") { }

Program::~Program() {
    if(m_programID != 0) {
        glDeleteProgram(m_programID);
    }
}

bool Program::loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {

    unsigned int vertexShader, fragmentShader;
    if( (vertexShader = createShader(vertexPath, GL_VERTEX_SHADER)) == 0) {
        return false;
    }

    if( (fragmentShader = createShader(fragmentPath, GL_FRAGMENT_SHADER)) == 0) {
        glDeleteShader(vertexShader);
        return false;
    }


    auto deleteShaders = [&](){
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    };

    unsigned int program = glCreateProgram();
    if(program == 0) {
        strcpy(m_lastMessageBuffer, "Cannot create a program");
        deleteShaders();
        return false;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE) {

        deleteShaders();

        GLint logLen = 0;
        glGetProgramInfoLog(program, BUF_SIZE, &logLen, m_lastMessageBuffer);
        m_lastMessageBuffer[logLen] = '\0';

        return false;
    }

    if(m_programID != 0) {
        glDeleteProgram(m_programID);
    }

    m_programID = program;

    deleteShaders();

    return true;
}
#include <iostream>

unsigned int Program::createShader(const std::string& path, GLenum shaderType) {

    std::ifstream shaderFile(path);
    if(!shaderFile.is_open()) {
        strcpy(m_lastMessageBuffer, (std::string("Cannot open a file '") + path + std::string("'")).c_str());
        return 0;
    }

    std::stringstream stream;
    stream << shaderFile.rdbuf();

    std::string source = stream.str();
    const char* csource = source.c_str();

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &csource, NULL);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus != GL_TRUE) {

        GLint logLen = 0;
        glGetShaderInfoLog(shader, BUF_SIZE, &logLen, m_lastMessageBuffer);
        m_lastMessageBuffer[logLen] = '\0';

        shader = 0;

    }

    return shader;
}

void Program::useProgram() {
    glUseProgram(m_programID);
}
