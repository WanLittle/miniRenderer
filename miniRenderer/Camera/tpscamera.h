#pragma once

#include "camera.h"

class TPSCamera : public Camera
{
private:
    glm::Vec3f target;
    float distance;

public:
    TPSCamera(glm::Vec3f _target = glm::Vec3f(0.0f, 0.0f, 0.0f), 
        glm::Vec3f world_up = glm::Vec3f(0.0f, 1.0f, 0.0f),
        float _distance = 10.0f, float _yaw = -90.0f, float _pitch = 0.0f)
        : Camera(world_up, _yaw, _pitch), 
        distance(_distance), target(_target)
    {
        updateCameraVectors();
    }

    glm::Matrix GetViewMatrix() override;

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;
    void ProcessMouseScroll(float yoffset) override;

    void setTarget(glm::Vec3f _target) 
    {
        target = _target;
        updateCameraVectors();
    }

private:
    void updateCameraVectors();
};
