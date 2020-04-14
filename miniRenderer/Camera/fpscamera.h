#pragma once

#include "camera.h"

#include "../Math/vec.h"
#include "../Math/mat.h"

#include <vector>

class FPSCamera : public Camera
{
public:
    FPSCamera(glm::Vec3f camera_pos = glm::Vec3f(0.0f, 0.0f, 0.0f),
        glm::Vec3f world_up = glm::Vec3f(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f)
        : Camera(world_up, yaw, pitch)
    {
        this->camera_pos = camera_pos;
        this->camera_forward = glm::Vec3f(0.0f, 0.0f, -1.0f);
        updateCameraVectors();
    }

    glm::Matrix GetViewMatrix() override;
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;
    void ProcessMouseScroll(float yoffset) override;

    void lookToPos(glm::Vec3f lookPos);

private:
    void updateCameraVectors();
    void updateCameraEulerAngles();
};
