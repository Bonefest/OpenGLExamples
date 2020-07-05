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

    reset();
    if(m_programID == 0) {
        strcpy(m_lastMessageBuffer, "Cannot create a program");
        return false;
    }

    if(!loadShader(vertexPath,GL_VERTEX_SHADER) ||
       !loadShader(fragmentPath, GL_FRAGMENT_SHADER)) {
        return false;
    }

    if(!link()) {
        return false;
    }


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

void Program::reset() {
    if(m_programID != 0) {
        glDeleteProgram(m_programID);
    }

    m_programID = glCreateProgram();
}

bool Program::loadShader(const std::string& path, GLenum shaderType) {
    unsigned int shader = createShader(path, shaderType);
    if(shader == 0) {
        return false;
    }

    if(m_programID == 0) {
        strcpy(m_lastMessageBuffer, "Cannot attach the shader cause program is not created.");
        return false;
    }

    glAttachShader(m_programID, shader);
    glDeleteShader(shader);

    return true;
}

bool Program::link() {
    if(m_programID == 0) {
        strcpy(m_lastMessageBuffer, "Cannot link the program cause program is not created.");
        return false;
    }

    glLinkProgram(m_programID);

    GLint linkStatus;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE) {

        GLint logLen = 0;
        glGetProgramInfoLog(m_programID, BUF_SIZE, &logLen, m_lastMessageBuffer);
        m_lastMessageBuffer[logLen] = '\0';

        return false;
    }

    return true;
}
