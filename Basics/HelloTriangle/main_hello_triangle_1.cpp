#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 vPos;\n"
"void main() { \n"
"    gl_Position = vec4(vPos, 1.0);\n"
"}\n";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 pixColor;\n"
"void main() {\n"
"    pixColor = vec4(0, 1, 0, 1);\n"
"}\n";


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

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    if(!checkShaderCompilationStatus(vertexShader)) return -1;

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    if(!checkShaderCompilationStatus(fragmentShader)) return -1;

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if(!checkProgramCompilationStatus(shaderProgram)) return -1;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float positions[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.5f,  0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f
    };

    float positions2[] = {
        -1.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,
        -1.0f,  0.0,  0.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int VAO2, VBO2;

    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions2), positions2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.73f, 0.88f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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

bool checkShaderCompilationStatus(GLuint shader) {
    int status;
    int messageLength = 0;
    char statusMessage[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, &messageLength, statusMessage);
        std::cout << statusMessage << "\n";
    }

    return (status == GL_TRUE);
}

bool checkProgramCompilationStatus(GLuint program) {
    int status;
    int messageLength = 0;
    char statusMessage[512];

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        glGetProgramInfoLog(program, 512, &messageLength, statusMessage);
        std::cout << statusMessage << "\n";
    }

    return status == GL_TRUE;
}
