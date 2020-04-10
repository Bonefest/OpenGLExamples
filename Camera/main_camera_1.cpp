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

#define WIDTH 640
#define HEIGHT 480

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursorMovementCallback(GLFWwindow* window, double x, double y);

void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);

float t = -2.0f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec2 lastCursorPosition = glm::vec2(0.0f, 0.0f);
glm::vec2 deltaCursorPosition = glm::vec2(0.0f, 0.0f);

float deltaTime = 0.0f;
float lastTime = 0.0f;

float cameraSpeed = 10.0f;
float cameraRotSpeed = 0.1f;

float yaw = 0.0f;
float pitch = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "LearnOpenGL", NULL, NULL);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(window, cursorMovementCallback);


    // SHADER PREPARING ------------------------------------------------------
    Program program("Coordinate Systems/vertex2.glsl", "Coordinate Systems/fragment2.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    // TEXTURE LOADING -------------------------------------------------------

    int width, height, channels;
    unsigned char* data = stbi_load("Resources/box.jpg", &width, &height, &channels, 0);
    if(data == NULL) {
        return -1;
    }

    unsigned int textid;
    glGenTextures(1, &textid);
    glBindTexture(GL_TEXTURE_2D, textid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_image_free(data);

    // DATA PREPARING  -------------------------------------------------------
    float positions[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int indecies[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indecies, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //CUBES COORDS

    glm::vec3 cubesLocations[5] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(3.0f,-1.0f, 1.5f),
        glm::vec3(0.0f, 0.0f, 2.5f),
        glm::vec3(0.0f, 0.0f, 3.5f),
        glm::vec3(0.0f, 0.0f, 4.5f)
    };

    // MISC ------------------------------------------------------------------

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(5.0f, -1.0f, -20.0f));
    view = glm::rotate(view, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);

    GLint modelULoc = glGetUniformLocation(program.getProgramID(), "model");
    GLint viewULoc = glGetUniformLocation(program.getProgramID(), "view");
    GLint projULoc = glGetUniformLocation(program.getProgramID(), "proj");

    // RENDER LOOP -----------------------------------------------------------

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();
        deltaTime = time - lastTime;

        lastTime = time;

        glUseProgram(program.getProgramID());

        //BINDING TEXTURE
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textid);

        glUniform1i(glGetUniformLocation(program.getProgramID(), "texture1"), 0);

        //BINDING TRANSFORMATIONS
        //glUniformMatrix4fv(modelULoc, 1, GL_FALSE, glm::value_ptr(model));
        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        glUniformMatrix4fv(viewULoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projULoc, 1, GL_FALSE, glm::value_ptr(proj));


        glBindVertexArray(VAO);
        for(int i = 0;i < 2; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubesLocations[i]);
            model = glm::rotate(model, glm::radians(i * 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelULoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        velocity += cameraFront * cameraSpeed * deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        velocity -= cameraFront * cameraSpeed * deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        velocity -= glm::cross(cameraFront, cameraUp) * cameraSpeed * deltaTime;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        velocity += glm::cross(cameraFront, cameraUp) * cameraSpeed * deltaTime;
    }

    cameraPosition += velocity;
}

void cursorMovementCallback(GLFWwindow* window, double x, double y) {
    glm::vec2 pos = glm::vec2(x, y);
    deltaCursorPosition = pos - lastCursorPosition;
    lastCursorPosition = pos;

    yaw += deltaCursorPosition.x * cameraRotSpeed;
    pitch -= deltaCursorPosition.y * cameraRotSpeed;
//
//    glm::mat4 m = glm::mat4(1.0f);
//    m = glm::rotate(m, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
//    m = glm::rotate(m, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
//    cameraFront = glm::vec3(m * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

    cameraFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    cameraFront.y = std::sin(glm::radians(pitch));
    cameraFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
}
