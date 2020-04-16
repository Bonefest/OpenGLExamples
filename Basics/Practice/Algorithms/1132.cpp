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

const int WIDTH = 640;
const int HEIGHT = 480;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Histogram", NULL, NULL);
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
    Program program("Practice/Algorithms/vertex.glsl", "Practice/Algorithms/fragment.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    // DATA PREPARATION ------------------------------------------------------
    std::vector<double> values;
    for(int i = 0;i < 1000; ++i)
        values.push_back(drand48());
    double value;
//    std::cout << " >>> ";
//    while(std::cin >> value) {
//        values.push_back(value);
//        std::cout << " >>> ";
//    }
//    std::cin.clear();
//    while(std::cin.get() != '\n');

    int N;
    std::cout << "N >>> ";
    std::cin >> N;

    float l, r;
    std::cout << "l,r >>> ";
    std::cin >> l >> r;

    std::vector<int> counter(N / (r - l), 0);
    for(double v : values) {
        if(v < r && v > l) {
            //std::cout << int( (v / (r - l) / ((r - l) / N)) ) << std::endl;
            counter[int( (v / (r - l) / ((r - l) / N)) )]++;
        }
    }

    int maxCount = 0;
    for(int count : counter) {
        maxCount = std::max(count, maxCount);
        std::cout << count << std::endl;
    }

    std::vector<float> data;
    for(int i = 0; i < counter.size(); ++i) {
        data.push_back( (2 * i * (r - l) / N) / (r - l) - 1.0f);
        data.push_back(0.0f);

        data.push_back( (2 * (i + 1) * (r - l) / N) / (r - l) - 1.0f);
        data.push_back(0.0f);

        data.push_back( (2 * (i + 1) * (r - l) / N) / (r - l) - 1.0f);
        data.push_back(float(counter[i]) / maxCount);
        //////////////////////////////////////////////////
        data.push_back( (2 * i * (r - l) / N) / (r - l) - 1.0f);
        data.push_back(0.0f);

        data.push_back( (2 * i * (r - l) / N) / (r - l) - 1.0f);
        data.push_back(float(counter[i]) / maxCount);

        data.push_back( (2 * (i + 1) * (r - l) / N) / (r - l) - 1.0f);
        data.push_back(float(counter[i]) / maxCount);
    }

    GLuint vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.getProgramID());
        glUniform4f(glGetUniformLocation(program.getProgramID(), "ucolor"),0.5f, 0.5f, 0.5f, 1.0f);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, data.size() / 2);

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
}
