#pragma once

#include "camera.h"

#include "../Math/vec.h"
#include "../Math/mat.h"

#include <vector>

class FPSCamera : public Camera
{
public:
    FPSCamera(glm::vec3f camera_pos = glm::vec3f(0.0f, 0.0f, 0.0f),
        glm::vec3f world_up = glm::vec3f(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f)
        : Camera(world_up, yaw, pitch)
    {
        this->camera_pos = camera_pos;
        this->camera_forward = glm::vec3f(0.0f, 0.0f, -1.0f);
        updateCameraVectors();
    }

    glm::Matrix GetViewMatrix() override;
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;
    void ProcessMouseScroll(float yoffset) override;

    void lookToPos(glm::vec3f lookPos);

private:
    void updateCameraVectors();
    void updateCameraEulerAngles();
};
