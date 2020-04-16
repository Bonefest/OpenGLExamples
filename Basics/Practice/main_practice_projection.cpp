#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Program.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);

float t = -2.0f;
float fov = 90.0f;

const int WIDTH = 640;
const int HEIGHT = 480;

glm::mat4 perspectice(float fov, float aspect, float near, float far) {
    glm::mat4 proj(1.0);
    float zoomx = 1.0 / std::tan(glm::radians(fov / 2.0f));
    float zoomy = zoomx * aspect;
    proj[0] = glm::vec4(zoomx, 0, 0, 0);
    proj[1] = glm::vec4(0, zoomy, 0, 0);
    proj[2] = glm::vec4(0, 0, -(far + near) / (far - near), -2.0f * far * near / (far - near) );
    proj[3] = glm::vec4(0, 0, -1, 0);

    return proj;
}

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
    Program program("Practice/projection_vertex.glsl", "Practice/projection_fragment.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    // TEXTURE LOADING -------------------------------------------------------
    int width, height, channels;
    unsigned char* imageData = stbi_load("Resources/box.jpg", &width, &height, &channels, 0);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(imageData);

    // DATA PREPARATION ------------------------------------------------------
    float data[] = {
        /* x      y      z      s      t*/

        //FRONT
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,

        //LEFT
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,

        //RIGHT
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f

    };

    unsigned int indecies[] = {
        0, 1, 2,
        1, 2, 3,

        0, 1, 4,
        1, 4, 5,

        2, 3, 6,
        3, 6, 7,

        4, 5, 6,
        5, 6, 7
    };

    GLuint vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glm::mat4 transform(1.0f), projection;
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f));
    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //projection = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f); //glm::perspectice(45.0f, 1.0f, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.getProgramID());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(program.getProgramID(), "texture1"), 0);

        transform = glm::rotate(transform, (float)std::sin(glfwGetTime()) * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
        projection = perspectice(fov / 2, float(WIDTH) / float(HEIGHT), 1.0f, 100.0f);
        //projection = glm::perspective(45.0f, float(WIDTH) / float(HEIGHT), 1.0f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(program.getProgramID(), "transform"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(glGetUniformLocation(program.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

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
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        fov = std::max(1.0f, fov - 1);
        std::cout << "FOV: " << fov << std::endl;
    } else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        fov = std::min(180.0f, fov + 1);
        std::cout << "FOV: " << fov << std::endl;
    }
}
