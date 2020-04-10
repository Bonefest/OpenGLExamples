#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../Program.h"

#include "Practice/heightmap.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);
float* generateCircle(int points);

const int WIDTH = 640;
const int HEIGHT = 480;

float* data;
std::vector<float> pairs;

int pointsNumber = 0;
int pairNumber = 0;
float p = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Random connections", NULL, NULL);
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
    Program program("Practice/Algorithms/1131_vertex.glsl", "Practice/Algorithms/1131_fragment.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    // DATA PREPARATION ------------------------------------------------------
    std::cout << "Enter points number >>> ";
    std::cin >> pointsNumber;

    std::cout << "Enter probability >>> ";
    std::cin >> p;

    data = generateCircle(pointsNumber);
    for(int i = 0;i < pointsNumber; ++i) {
        for(int j = 0;j < pointsNumber; ++j) {
            if(i == j) continue;

            if(drand48() <= p) {
                pairs.push_back(data[2*i]);
                pairs.push_back(data[2*i + 1]);

                pairs.push_back(data[2*j]);
                pairs.push_back(data[2*j + 1]);
            }
        }
    }

    GLuint vbo[2], vao[2], ebo;
    glGenVertexArrays(2, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 2 * pointsNumber * sizeof(float), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pairs.size(), pairs.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.getProgramID());
        glPointSize(5.0f);
        glUniform4f(glGetUniformLocation(program.getProgramID(), "ucolor"),1.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_POINTS, 0, pointsNumber);

        glPointSize(1.0f);
        glUniform4f(glGetUniformLocation(program.getProgramID(), "ucolor"),0.5f, 0.5f, 0.5f, 1.0f);
        glBindVertexArray(vao[1]);
        glDrawArrays(GL_LINES, 0, pairs.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete [] data;
    glfwTerminate();
    return 0;
}

float* generateCircle(int points) {
    float* data = new float[points * 2];
    float angle = 2.0f * M_PI / points;
    int i = 0;
    for(float step = 0.0f;step < 2.0f * M_PI; step += angle, i += 2) {
        data[i] = std::cos(step);
        data[i + 1] = std::sin(step);
    }

    return data;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
