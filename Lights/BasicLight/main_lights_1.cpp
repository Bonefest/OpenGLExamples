#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Program.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

#define WIDTH 640
#define HEIGHT 480

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursorMovementCallback(GLFWwindow* window, double x, double y);

void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);


Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 67.5f);

float deltaTime = 0.0f;
float lastTime = 0.0f;

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
    Program program("Lights/Shaders/light_vertex.glsl", "Lights/Shaders/light_fragment.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    Program lightProgram("Lights/Shaders/source_vertex.glsl", "Lights/Shaders/source_fragment.glsl");
    if(lightProgram.hasError()) {
        std::cout << lightProgram.getErrorMessage() << std::endl;
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
        //BACK
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int VAO, VAO2, VBO;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // DATA ------------------------------------------------------------------

    glm::vec3 objectLocation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.33f);

    glm::vec3 lightSourceLocation = glm::vec3(0.0f, 1.0f, -1.0f);
    glm::vec3 lightSourceColor = glm::vec3(1.0f, 1.0f, 1.f);
    float ambientFactor = 0.15f;

    // MISC ------------------------------------------------------------------

    glm::mat4 model, view, proj;

    proj = glm::perspective(glm::radians(camera.zoom), float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);

    GLint modelULoc = glGetUniformLocation(program.getProgramID(), "model");
    GLint viewULoc = glGetUniformLocation(program.getProgramID(), "view");
    GLint projULoc = glGetUniformLocation(program.getProgramID(), "proj");

    GLint lightModelULoc = glGetUniformLocation(program.getProgramID(), "model");
    GLint lightViewULoc = glGetUniformLocation(program.getProgramID(), "view");
    GLint lightProjULoc = glGetUniformLocation(program.getProgramID(), "proj");

    // RENDER LOOP -----------------------------------------------------------

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.73f, 0.88f, 0.98f, 1.0f);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;


        view = camera.getCameraTransform();

        //lightSourceColor = glm::vec3( (std::sin(time) + 1.0f) * 0.5f );
        lightSourceLocation = glm::vec3(0.0f, (std::sin(time) + 1.0f) * 2.0f + 1.0f, -(std::sin(time) + 1.0f) * 2.0f);

        // ~~~~ RENDERING OBJECTS ~~~~
        glUseProgram(program.getProgramID());

        glUniformMatrix4fv(viewULoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projULoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniform1f(glGetUniformLocation(program.getProgramID(), "ambientFactor"), ambientFactor);
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "objectColor"), 1, glm::value_ptr(objectColor));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "sourceColor"), 1, glm::value_ptr(lightSourceColor));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "sourcePosition"), 1, glm::value_ptr(lightSourceLocation));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "cameraPosition"), 1, glm::value_ptr(camera.position));

        glBindVertexArray(VAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, objectLocation);
        glUniformMatrix4fv(modelULoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ~~~~ RENDERING LIGHTS SOURCES ~~~~
        glUseProgram(lightProgram.getProgramID());

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightSourceLocation);
        model = glm::scale(model, glm::vec3(0.2f));

        glUniformMatrix4fv(lightModelULoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(lightViewULoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lightProjULoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniform3fv(glGetUniformLocation(lightProgram.getProgramID(), "sourceColor"), 1, glm::value_ptr(lightSourceColor));

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.zoom -= deltaTime * 10;
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.zoom += deltaTime * 10;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(window, deltaTime, Direction::FRONT);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(window, deltaTime, Direction::BACK);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(window, deltaTime, Direction::LEFT);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(window, deltaTime, Direction::RIGHT);
    }

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
}

void cursorMovementCallback(GLFWwindow* window, double x, double y) {
    camera.processMouse(window, x, y);
}
