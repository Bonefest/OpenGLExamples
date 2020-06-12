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

    Program postProgram("Advanced/FragmentBuffers/post_vertex.glsl", "Advanced/FragmentBuffers/blur_fragment.glsl");
    if(postProgram.hasError()) {
        std::cout << postProgram.getErrorMessage() << std::endl;
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

    unsigned int VAO, VAO2, postVAO, VBO, postVBO;

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


    //~~~~ Frame buffer ~~~~

    float quad[] = {
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,

         1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &postVAO);
    glGenBuffers(1, &postVBO);

    glBindVertexArray(postVAO);
    glBindBuffer(GL_ARRAY_BUFFER, postVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int screentex;
    glGenTextures(1, &screentex);
    glBindTexture(GL_TEXTURE_2D, screentex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screentex, 0);


    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 640, 480);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return -1;

    // DATA ------------------------------------------------------------------

    float objectShininess = 16.0f;

    // Directional light

    glm::vec3 dirLightDirection(0.707f, -0.707f, 0.0f);

    // Point lights
    glm::vec3 pointLightPositions[4] = {
        glm::vec3( 0.0f, 3.0f, 0.0f),
        glm::vec3( 3.0f, 3.0f, 0.0f),
        glm::vec3(-3.0f, 3.0f, 0.0f),
        glm::vec3( 0.0f, 3.0f, 0.0f)
    };

    glm::vec3 lightSourceAmbientColor = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 lightSourceDiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightSourceSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

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

    //glEnable(GL_DEPTH_TEST);
    glClearColor(0.73f, 0.88f, 0.98f, 1.0f);

    glUseProgram(program.getProgramID());

    glUniform1i(glGetUniformLocation(program.getProgramID(), "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(program.getProgramID(), "material.specular"), 1);



    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;


        view = camera.getCameraTransform();


        pointLightPositions[0] = glm::vec3(std::cos(time) * 5.0f, 0.0f, std::sin(time) * 5.0f);
        pointLightPositions[1] = glm::vec3(std::cos(time) * 5.0f, std::sin(time) * 5.0f, 0.0f);
        pointLightPositions[2] = glm::vec3(0.0f, std::cos(time) * 5.0f, std::sin(time) * 5.0f);

        spotlightPosition = camera.position;
        spotlightDirection = camera.getCameraDirection();

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
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "dirLight.direction"), 1, glm::value_ptr(dirLightDirection));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "dirLight.light.ambient"), 1, glm::value_ptr(lightSourceAmbientColor));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "dirLight.light.diffuse"), 1, glm::value_ptr(lightSourceDiffuseColor));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "dirLight.light.specular"), 1, glm::value_ptr(lightSourceSpecularColor));

        //Point lights
        for(int i = 0;i < 4; ++i) {
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
            glUniform1f(glGetUniformLocation(program.getProgramID(), format), 0.14f);

            sprintf(format, "pointLights[%d].quadratic", i);
            glUniform1f(glGetUniformLocation(program.getProgramID(), format), 0.07f);
        }

        glUniform3fv(glGetUniformLocation(program.getProgramID(), "cameraPosition"), 1, glm::value_ptr(camera.position));

        //Spotlight
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "spotlight.position"), 1, glm::value_ptr(camera.position));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "spotlight.direction"), 1, glm::value_ptr(camera.getCameraDirection()));
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.innerAngle"), std::cos(glm::radians(spotlightInnerAngle)));
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.outerAngle"), std::cos(glm::radians(spotlightOuterAngle)));

        glUniform3fv(glGetUniformLocation(program.getProgramID(), "spotlight.light.ambient"), 1, glm::value_ptr(lightSourceAmbientColor));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "spotlight.light.diffuse"), 1, glm::value_ptr(lightSourceDiffuseColor));
        glUniform3fv(glGetUniformLocation(program.getProgramID(), "spotlight.light.specular"), 1, glm::value_ptr(lightSourceSpecularColor));

        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.linear"), 0.14f);
        glUniform1f(glGetUniformLocation(program.getProgramID(), "spotlight.quadratic"), 0.07f);

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


        // Rendering to default screen


        glUseProgram(postProgram.getProgramID());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screentex);
        glUniform1i(glGetUniformLocation(postProgram.getProgramID(), "texture"), 0);

        glBindVertexArray(postVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteFramebuffers(1, &fbo);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteVertexArrays(1, &postVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &postVBO);

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
