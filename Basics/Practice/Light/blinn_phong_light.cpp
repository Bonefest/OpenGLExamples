#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../Program.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 640
#define HEIGHT 480

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);

float roll  = 0.0f;
float pitch = 0.0f;
float yaw   = 0.0f;


glm::vec3 offset;

glm::mat4 lookAt(glm::vec3 cameraPos, glm::vec3 target, glm::vec3 up) {
    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, glm::vec3(-cameraPos));

    glm::vec3 f = glm::normalize(target - cameraPos);
    glm::vec3 r = glm::normalize(glm::cross(f, up));
    glm::vec3 u = glm::cross(r, f);

    glm::mat4 basis = glm::mat4(1.0f);
    basis[0] = glm::vec4(r, 0.0f);
    basis[1] = glm::vec4(u, 0.0f);
    basis[2] = glm::vec4(-f, 0.0f);
    basis[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return basis * translate;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Blinn-Phong", NULL, NULL);
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
    Program program("Practice/Light/vertex.glsl", "Practice/Light/fragment.glsl");
    if(program.hasError()) {
        std::cout << program.getErrorMessage() << std::endl;
        return -1;
    }

    // TEXTURE LOADING -------------------------------------------------------

        // FILE LOADING
    int width, height, channels, width2, height2, channels2;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* texData = stbi_load("Resources/box.jpg", &width, &height, &channels, 0);
    if(!texData) {
        std::cout << "Unabled to load texture data!\n";
        return -1;
    }

    unsigned char* texData2 = stbi_load("Resources/awesomeface.png", &width2, &height2, &channels2, 0);
    if(!texData2) {
        std::cout << "Unable to load texture data!\n";
        return -1;
    }

        // IMPORTING TEXTURE TO TEXTURE OBJECT
    unsigned int texids[2];
    glGenTextures(2, texids);
    glBindTexture(GL_TEXTURE_2D, texids[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    float borderColor[] = {0.5f, 0.5f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texids[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData2);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    stbi_image_free(texData);
    stbi_image_free(texData2);

    // DATA PREPARING  -------------------------------------------------------
    float positions[] = {
        /*    POSITION   */   /* TEX POS */  /* NORMAL */

         //FRONT FACE
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,  1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f,

         //BACK FACE
        -0.5f, -0.5f,-0.5f,  0.0f, 0.0f,    0.0f, 0.0f,-1.0f,
        -0.5f,  0.5f,-0.5f,  0.0f, 1.0f,    0.0f, 0.0f,-1.0f,
         0.5f,  0.5f,-0.5f,  1.0f, 1.0f,    0.0f, 0.0f,-1.0f,

        -0.5f, -0.5f,-0.5f,  0.0f, 0.0f,    0.0f, 0.0f,-1.0f,
         0.5f, -0.5f,-0.5f,  1.0f, 0.0f,    0.0f, 0.0f,-1.0f,
         0.5f,  0.5f,-0.5f,  1.0f, 1.0f,    0.0f, 0.0f,-1.0f,

         //LEFT FACE
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,-0.5f,  1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,-0.5f,  1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,-0.5f,  1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,

         //RIGHT FACE
         0.5f, -0.5f, 0.5f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.5f,  0.0f, 1.0f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,-0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,

         0.5f, -0.5f, 0.5f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,-0.5f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,-0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,

         //BOTTON FACE
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,    0.0f,-1.0f, 0.0f,
        -0.5f, -0.5f,-0.5f,  0.0f, 1.0f,    0.0f,-1.0f, 0.0f,
         0.5f, -0.5f,-0.5f,  1.0f, 1.0f,    0.0f,-1.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,    0.0f,-1.0f, 0.0f,
         0.5f, -0.5f, 0.5f,  1.0f, 0.0f,    0.0f,-1.0f, 0.0f,
         0.5f, -0.5f,-0.5f,  1.0f, 1.0f,    0.0f,-1.0f, 0.0f,

         //TOP FACE
        -0.5f,  0.5f, 0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,-0.5f,  0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,-0.5f,  1.0f, 1.0f,    0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, 0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.5f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,-0.5f,  1.0f, 1.0f,    0.0f, 1.0f, 0.0f
    };

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*5));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // MISC ------------------------------------------------------------------
    GLint modelLoc = glGetUniformLocation(program.getProgramID(), "model");
    GLint viewLoc = glGetUniformLocation(program.getProgramID(), "view");
    GLint projLoc = glGetUniformLocation(program.getProgramID(), "projection");
    GLint cameraLoc = glGetUniformLocation(program.getProgramID(), "cameraPosition");

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection = glm::perspective(45.0f, float(WIDTH)/float(HEIGHT), 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    offset = glm::vec3(0.0f, 1.0f, 2.0f);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program.getProgramID());

        // Transformation
        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f,std::sin(glfwGetTime()) * 3.0f));

        view = glm::mat4(1.0);
//        view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.0f, 0.0f, 1.0f));
//        view = glm::rotate(view, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
//        view = glm::rotate(view, glm::radians(roll), glm::vec3(0.0f, 1.0f, 0.0f));
//        view = glm::translate(view, -offset);

        view = lookAt(offset, glm::vec3(0.0f, std::sin(glfwGetTime()) * 2, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(cameraLoc, 1, glm::value_ptr(offset));

        // --------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texids[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texids[1]);

        glUniform1i(glGetUniformLocation(program.getProgramID(), "tex1"), 1);
        glUniform1i(glGetUniformLocation(program.getProgramID(), "tex2"), 0);

        glBindVertexArray(VAO);
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
    glm::mat4 currentView(1.0f);

    currentView = glm::rotate(currentView, glm::radians(-yaw), glm::vec3(0.0f, 0.0f, 1.0f));
    currentView = glm::rotate(currentView, glm::radians(-pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    currentView = glm::rotate(currentView, glm::radians(-roll), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 viewingz = glm::vec3(currentView * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    glm::vec3 viewingx = glm::vec3(currentView * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        /*offset += viewingz * 0.1f;*/ offset.z -= 0.1f;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        /*offset -= viewingz * 0.1f;*/ offset.z += 0.1f;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        offset += viewingx * 0.1f;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        offset -= viewingx * 0.1f;
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        roll -= 2.0f;
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        roll += 2.0f;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch -= 2.0f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch += 2.0f;
}
