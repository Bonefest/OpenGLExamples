#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Direction {
    FRONT, BACK, LEFT, RIGHT
};

class Camera {
public:

    Camera(glm::vec3 cameraPosition, float Yaw, float Pitch, float Zoom): position(cameraPosition),
                                                                          yaw(Yaw), pitch(Pitch), zoom(Zoom), speed(5.0f), sensetivity(0.1f),
                                                                          front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                          up(glm::vec3(0.0f, 1.0f, 0.0f)) { }

    void processKeyboard(GLFWwindow* window, float dt, Direction direction) {


        if(direction == FRONT)
            position += glm::normalize(front) * speed * dt;
        else if(direction == BACK)
            position -= glm::normalize(front) * speed * dt;
        else if(direction == LEFT)
            position -= glm::cross(glm::normalize(front), up) * speed * dt;
        else if(direction == RIGHT)
            position += glm::cross(glm::normalize(front), up) * speed * dt;

        position.y = 0;
    }

    void processMouse(GLFWwindow* window, double x, double y) {
        glm::vec2 deltaCursorPosition = glm::vec2(x, y) - m_lastCursorPosition;
        m_lastCursorPosition = glm::vec2(x, y);

        yaw += deltaCursorPosition.x * sensetivity;
        pitch -= deltaCursorPosition.y * sensetivity;
        pitch = std::min(89.0f, pitch);
        pitch = std::max(-89.0f, pitch);

        front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front.y = std::sin(glm::radians(pitch));
        front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    }

    glm::mat4 getCameraTransform() const {
        return glm::lookAt(position, position + front, up);
    }

    glm::vec3 position;

    float yaw;
    float pitch;
    float zoom;
    float speed;
    float sensetivity;

private:
    glm::vec3 front;
    glm::vec3 up;

    glm::vec2 m_lastCursorPosition;
};

#endif // CAMERA_H_INCLUDED
