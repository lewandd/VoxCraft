#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum DIRECTION {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
};

const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
public:
    glm::vec3 position;
    glm::vec3 Front;
    glm::vec3 Right;
        
    float yaw;
    float pitch;
    float MovementSpeed;
    float mouseSens;

    Camera(glm::vec3 position_) {
        MovementSpeed = 8.5f;
        mouseSens = 0.1f;
        position = position_;
        yaw = 0.0;
        pitch = 0.0;
        update();
    }

    glm::mat4 getViewMatrix(){
        return glm::lookAt(position, position + Front, Up);
    }

    void ProcessKeyboard(DIRECTION direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            position += glm::normalize(glm::vec3(Front.x, 0, Front.z)) * velocity;
        if (direction == BACKWARD)
            position -= glm::normalize(glm::vec3(Front.x, 0, Front.z)) * velocity;
        if (direction == LEFT)
            position -= glm::normalize(glm::vec3(Right.x, 0, Right.z)) * velocity;
        if (direction == RIGHT)
            position += glm::normalize(glm::vec3(Right.x, 0, Right.z)) * velocity;
        if (direction == UP)
            position += Up * velocity;
        if (direction == DOWN)
            position -= Up * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= mouseSens;
        yoffset *= mouseSens;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }
        update();
    }

private:
    void update() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, Up));
    }
};