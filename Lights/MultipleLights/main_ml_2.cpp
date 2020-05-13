#include <iostream>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Program.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "helper.h"

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
    Program program("Lights/Shaders/lc_vertex.glsl", "Lights/Shaders/ml_fragment.glsl");
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

    unsigned int textureDiffuse, textureSpecular;
    glGenTextures(1, &textureDiffuse);
    if(!loadTexture("Resources/container.png", textureDiffuse, GL_RGBA, GL_RGBA, true)) {
        return -1;
    }

    glGenTextures(1, &textureSpecular);
    if(!loadTexture("Resources/container_specular.png", textureSpecular, GL_RGBA, GL_RGBA, true)) {
        return -1;
    }


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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

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

    //glm::vec3 objectLocation = glm::vec3(0.0f, 0.0f, 0.0f);
    //DEPRECATED glm::vec3 objectAmbientColor = glm::vec3(1.0f, 0.5f, 0.33f);
    //DEPRECATED glm::vec3 objectDiffuseColor = glm::vec3(1.0f, 0.5f, 0.33f);
    //DEPRECATED glm::vec3 objectSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

    float objectShininess = 16.0f;

    // Directional light

    glm::vec3 dirLightDirection(0.707f, -0.707f, 0.0f);

    // Point lights
    glm::vec3 pointLightPositions[4] = {
        glm::vec3( 0.0f, 3.0f, 0.0f),
        glm::vec3( 3.0f, 5.0f, 0.0f),
        glm::vec3(-3.0f, 5.0f, 0.0f),
        glm::vec3( 0.0f, 5.0f, 0.0f)
    };

    glm::vec3 lightSourceAmbientColor = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightSourceDiffuseColor = glm::vec3(0.3f, 0.15f, 0.15f);
    glm::vec3 lightSourceSpecularColor = glm::vec3(0.3f, 0.3f, 0.3f);

    glm::vec3 spotlightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 spotlightPosition = glm::vec3(0.0f, 1.0f, 2.0f);
    float spotlightInnerAngle = 12.5f;
    float spotlightOuterAngle = 18.5f;

    // CUBES INFORMATION -----------------------------------------------------

                          //Position, Rotation
    std::vector<std::pair<glm::vec3, glm::vec3>> cubesData;
    cubesData.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    cubesData.emplace_back(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(45.0f, 0.0f, 0.0f));
    cubesData.emplace_back(glm::vec3(-1.5f, 0.4f, 0.0f), glm::vec3(17.6f, 23.0f, 0.0f));
    cubesData.emplace_back(glm::vec3(0.5f, 1.3f, 3.2f), glm::vec3(0.6f, 23.0f, 76.17f));

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.getProgramID());

    glUniform1i(glGetUniformLocation(program.getProgramID(), "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(program.getProgramID(), "material.specular"), 1);



    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;


        view = camera.getCameraTransform();

        spotlightPosition = camera.position;
        spotlightDirection = camera.getCameraDirection();


        pointLightPositions[0] = glm::vec3(0.0f, (std::sin(time) + 1.0f) * 1.5f + 1.5f, 0.0f);

        // ~~~~ RENDERING OBJECTS ~~~~
        glUseProgram(program.getProgramID());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureDiffuse);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureSpecular);

        glUniformMatrix4fv(viewULoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projULoc, 1, GL_FALSE, glm::value_ptr(proj));

        glUniform1f(glGetUniformLocation(program.getProgramID(), "material.shininess"), objectShininess);

        //Directional light
        //glUniform3fv(glGetUniformLocation(program.getProgramID(), "dirLight.direction"), 1, glm::value_ptr(dirLightDirection));
        //glUniform3f(glGetUniformLocation(program.getProgramID(), "dirLight.light.ambient"), 1, glm::value_ptr(lightSourceAmbientColor));
        //glUniform3fv(glGetUniformLocation(program.getProgramID(), "dirLight.light.diffuse"), 1, glm::value_ptr(lightSourceDiffuseColor));
        //glUniform3fv(glGetUniformLocation(program.getProgramID(), "dirLight.light.specular"), 1, glm::value_ptr(lightSourceSpecularColor));

        //Point lights
        for(int i = 0;i < 1; ++i) {
            char format[64]{};
            sprintf(format, "pointLights[%d].position", i);
            glUniform3fv(glGetUniformLocation(program.getProgramID(), format), 1, glm::value_ptr(pointLightPositions[i]));

            sprintf(format, "pointLights[%d].light.ambient", i);
            glUniform3fv(glGetUniformLocation(program.getProgramID(), format), 1, glm::value_ptr(lightSourceAmbientColor));

            sprintf(format, "pointLights[%d].light.diffuse", i);
            glUniform3fv(glGetUniformLocation(program.getProgramID(), format), 1, glm::value_ptr(lightSourceDiffuseColor));

            sprintf(format, "pointLights[%d].light.specular", i);
            glUniform3fv(glGetUniformLocation(program.getProgramID(), format), 1, glm::value_ptr(lightSourceSpecularColor));

            sprintf(format, "pointLights[%d].constant", i);
            glUniform1f(glGetUniformLocation(program.getProgramID(), format), 1.0f);

            sprintf(format, "pointLights[%d].linear", i);
            glUniform1f(glGetUniformLocation(program.getProgramID(), format), 0.35f);

            sprintf(format, "pointLights[%d].quadratic", i);
            glUniform1f(glGetUniformLocation(program.getProgramID(), format), 0.44f);
        }

        glUniform3fv(glGetUniformLocation(program.getProgramID(), "cameraPosition"), 1, glm::value_ptr(camera.position));

        //Spotlight
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "spotlight.position"), 1, glm::value_ptr(camera.position));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "spotlight.direction"), 1, glm::value_ptr(camera.getCameraDirection()));
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.innerAngle"), std::cos(glm::radians(spotlightInnerAngle)));
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.outerAngle"), std::cos(glm::radians(spotlightOuterAngle)));

        glUniform3f(glGetUniformLocation(program.getProgramID(), "spotlight.light.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(program.getProgramID(), "spotlight.light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(program.getProgramID(), "spotlight.light.specular"),1.0f, 1.0f, 1.0f);

        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.linear"), 1.1f);
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.quadratic"), 2.4f);

        glBindVertexArray(VAO);

        for(auto cubeData : cubesData) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, std::get<0>(cubeData));

            glm::vec3 cubeOrientation = std::get<1>(cubeData);
            model = glm::rotate(model, glm::radians(cubeOrientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(cubeOrientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(cubeOrientation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            glUniformMatrix4fv(modelULoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ~~~~ RENDERING LIGHTS SOURCES ~~~~
        glUseProgram(lightProgram.getProgramID());

        glUniformMatrix4fv(lightViewULoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lightProjULoc, 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(VAO2);

        // Rendering point lights only!
        for(int i = 0;i < 4; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(pointLightPositions[i]));
            model = glm::scale(model, glm::vec3(0.2f));

            glUniformMatrix4fv(lightModelULoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(lightProgram.getProgramID(), "sourceColor"), 1, glm::value_ptr(lightSourceDiffuseColor));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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