#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Program.h"

#include "Practice/heightmap.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);

const int WIDTH = 640;
const int HEIGHT = 480;

double scale = 1.0;
double offset = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Perspective", NULL, NULL);
    if(window == NULL) {
        std::cout << "Unable to create window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Unable to initialize GLAD!\n";
        return -1;
    }

    //glViewport(0, 0, 640, 480); will be called as well for the first loading
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // SHADER PREPARING ------------------------------------------------------
    Program program("Practice/heightmap2d_vertex.glsl", "Practice/heightmap2d_fragment.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    // DATA PREPARATION ------------------------------------------------------
    Noise noise(200);
    int size;
    std::vector<double> data = noise.getGLCoords(0.1, 200, &size);
    for(double v : data) {
        std::cout << v << std::endl;
    }

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(double), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.getProgramID());
        glUniform1f(glGetUniformLocation(program.getProgramID(), "scale"), scale);
        glUniform1f(glGetUniformLocation(program.getProgramID(), "offset"), offset);

        glBindVertexArray(vao);
        glDrawArrays(GL_LINE_STRIP, 0, size);

            glPointSize(2.0f);

        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, size);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        scale = std::max(0.1, scale - 0.005);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        scale = std::min(10.0, scale + 0.005);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        offset += 0.01;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        offset -= 0.01;
    }

}
