#include <iostream>

#include "sb7.h"

class RainbowScreen: public sb7::application {
public:
    virtual void render(double currentTime) {
        GLfloat color[] = {1.0, 1.0, 1.0, 1.0};
        color[0] = std::cos(currentTime * 0.5f) * 0.5f + 0.5f;
        color[2] = std::sin(currentTime * 0.5f) * 0.5f + 0.5f;

        glClearBufferfv(GL_COLOR, 0, color);
    }


};

//DECLARE_MAIN(RainbowScreen)
