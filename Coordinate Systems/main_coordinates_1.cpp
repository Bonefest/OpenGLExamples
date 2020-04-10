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
void processInput(GLFWwindow* window);
bool checkShaderCompilationStatus(GLuint shader);
bool checkProgramCompilationStatus(GLuint program);

float t = -2.0f;

glm::mat4 getPerspective(float left, float right, float top, float bottom, float near, float far) {
    glm::mat4 result = glm::mat4();
    result[0] = glm::vec4((2*near)/(right - left), 0, (right+left)/(right-left), 0);
    result[1] = glm::vec4(0, (2*near)/(top-bottom), (top + bottom)/(top - bottom), 0);
    result[2] = glm::vec4(0, 0, -(near + far)/(far-near), (-2*near*far)/(far - near));
    result[3] = glm::vec4(0, 0, -1, 0);

    return result;
}

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
    Program program("Coordinate Systems/vertex.glsl", "Coordinate Systems/fragment.glsl");
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
        -0.5f, -0.5f,  0.0f, /* color */ 1.0f, 0.0f, 0.0f, /* a = 1.0 */ /* texture */ 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, /* color */ 0.0f, 1.0f, 0.0f, /* a = 1.0 */ /* texture */ 1.0f, 0.0f,
         0.5f,  0.5f,  0.0f, /* color */ 0.0f, 0.0f, 1.0f, /* a = 1.0 */ /* texture */ 1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, /* color */ 0.0f, 0.0f, 1.0f, /* a = 1.0 */ /* texture */ 0.0f, 1.0f,
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
    GLint trUniformLoc = glGetUniformLocation(program.getProgramID(), "transform");
    GLint projLoc = glGetUniformLocation(program.getProgramID(), "proj");


    glm::mat4 projection(1.0f);
    projection = getPerspective(-1.0f, 1.0f, 1.0f, -1.0f, 2.0f, 100.0f);
    //projection = glm::perspective(glm::radians(45.0f), float(WIDTH)/float(HEIGHT), 0.1f, 100.0f);
    for(int i = 0;i < 4;++i) {
        for(int j = 0;j < 4;++j)
            std::cout << projection[i][j] << " ";
        std::cout << std::endl;
    }

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glEnable(GL_DEPTH_TEST);

        glUseProgram(program.getProgramID());
        double time = glfwGetTime();
        glUniform2f(glGetUniformLocation(program.getProgramID(), "offset"), std::cos(time) * 0 , std::sin(time) * 0);
        //glUniform1f(glGetUniformLocation(program.getProgramID(), "t"), (std::sin(time) + 1.0f) * 0.5f);
        glUniform1f(glGetUniformLocation(program.getProgramID(), "t"), t);

        std::cout << t << std::endl;

        // Transformation

        glm::mat4 transformation(1.0f);
        transformation = glm::translate(transformation, glm::vec3(0, 0, t));
        //transformation = glm::rotate(transformation , (float)std::sin(time), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(trUniformLoc, 1, GL_FALSE, glm::value_ptr(transformation));

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // --------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texids[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texids[1]);

        glUniform1i(glGetUniformLocation(program.getProgramID(), "tex1"), 1);
        glUniform1i(glGetUniformLocation(program.getProgramID(), "tex2"), 0);

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
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        t = std::min(t + 0.01f, 10.0f);
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        t = std::max(t - 0.01f, -10.0f);
}
