#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Program.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);

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

    // SHADER PREPARING ------------------------------------------------------
    Program program("Textures/vertex.glsl", "Textures/fragment.glsl");
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texids[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData2);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    stbi_image_free(texData);
    stbi_image_free(texData2);

    // DATA PREPARING  -------------------------------------------------------
    float positions[] = {
        -0.5f, -0.5f,  0.0f, /* color */ 1.0f, 0.0f, 0.0f, /* a = 1.0 */ /* texture */ 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, /* color */ 0.0f, 1.0f, 0.0f, /* a = 1.0 */ /* texture */ 2.0f, 0.0f,
         0.5f,  0.5f,  0.0f, /* color */ 0.0f, 0.0f, 1.0f, /* a = 1.0 */ /* texture */ 2.0f, 2.0f,
        -0.5f,  0.5f,  0.0f, /* color */ 0.0f, 0.0f, 1.0f, /* a = 1.0 */ /* texture */ 0.0f, 2.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // MISC ------------------------------------------------------------------
    GLuint offsetUniformLocation = glGetUniformLocation(program.getProgramID(), "offset");

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program.getProgramID());
        double time = glfwGetTime();
        glUniform2f(glGetUniformLocation(program.getProgramID(), "offset"), std::cos(time) * 0 , std::sin(time) * 0);
        glUniform1f(glGetUniformLocation(program.getProgramID(), "t"), (std::sin(time) + 1.0f) * 0.5f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texids[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texids[1]);

        glUniform1i(glGetUniformLocation(program.getProgramID(), "tex1"), 0);
        glUniform1i(glGetUniformLocation(program.getProgramID(), "tex2"), 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
