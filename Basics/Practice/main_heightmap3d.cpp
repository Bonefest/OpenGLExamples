#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Program.h"

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

float rotationx = 30.0f;
float rotationy = 0.0f;

float offsetx = -5.0f;
float offsety = 2.0f;
float offsetz = -5.0f;

GLenum mode;

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
    Program program("Practice/heightmap3d_vertex.glsl", "Practice/heightmap3d_fragment.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    // DATA PREPARATION ------------------------------------------------------
    Noise3D noise(20);
    int size;
    std::vector<double> data = noise.getGLCoords(0.1, 20, &size);

    std::vector<unsigned int> indecies = noise.getIndecies(size);

    GLuint vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(int), indecies.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(double), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));


    glm::mat4 projection = glm::perspective(45.0f, 1.33f, 0.1f, 50.0f);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glUseProgram(program.getProgramID());

        transform = glm::mat4(1.0);
        transform = glm::rotate(transform, glm::radians(rotationx), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotationy), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::translate(transform, glm::vec3(-offsetx, -offsety, -offsetz));

        glm::vec4 color(0.0f, 0.0f, 0.0f, 0.0f);

        if(mode == GL_POINT) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(vao);

            glUniformMatrix4fv(glGetUniformLocation(program.getProgramID(), "transform"), 1, GL_FALSE, glm::value_ptr(transform));
            glUniformMatrix4fv(glGetUniformLocation(program.getProgramID(), "projection"),1, GL_FALSE, glm::value_ptr(projection));
            glUniform4fv(glGetUniformLocation(program.getProgramID(), "ucolor"), 1, glm::value_ptr(color));
            glDrawElements(GL_TRIANGLES, indecies.size(), GL_UNSIGNED_INT, 0);

            glPointSize(5.0f);
            color = glm::vec4(0.5f, 0.0f, 0.0f, 0.0f);
        }

        glPolygonMode(GL_FRONT_AND_BACK, mode);

        glBindVertexArray(vao);

        glUniformMatrix4fv(glGetUniformLocation(program.getProgramID(), "transform"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(glGetUniformLocation(program.getProgramID(), "projection"),1, GL_FALSE, glm::value_ptr(projection));
        glUniform4fv(glGetUniformLocation(program.getProgramID(), "ucolor"), 1, glm::value_ptr(color));
        glDrawElements(GL_TRIANGLES, indecies.size(), GL_UNSIGNED_INT, 0);


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
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        offsetx -= 0.1f;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        offsetx += 0.1f;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        offsetz -= 0.1f;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        offsetz += 0.1f;
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        offsety += 0.1f;
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        offsety -= 0.1f;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        rotationx -= 1.0f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        rotationx += 1.0f;
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotationy -= 1.0f;
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotationy += 1.0f;
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        mode = GL_LINE;
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        mode = GL_FILL;
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        mode = GL_POINT;

}
