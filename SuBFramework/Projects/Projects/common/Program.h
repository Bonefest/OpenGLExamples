#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <string>
#include "sb7.h"

class Program {
    enum {BUF_SIZE = 1024};

public:

    Program();
    ~Program();

    bool loadShaders(const std::string& vertexPath, const std::string& fragmentPath);

    void useProgram();

    void setUniform(const std::string& name, double value);
    void setUniform(const std::string& name, int value);
    /* ... */

    inline unsigned int getProgramID() const { return m_programID; }

    // Functions for manual building a program
    void reset();
    bool loadShader(const std::string& path, GLenum shaderType);
    bool link();

    inline std::string getError() const { return m_lastMessageBuffer; }

private:

    unsigned int createShader(const std::string& path, GLenum shaderType);

    unsigned int    m_programID;
    char            m_lastMessageBuffer[BUF_SIZE];


};

#endif // PROGRAM_H_INCLUDED
