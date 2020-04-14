#include "TPSCamera.h"

glm::Matrix TPSCamera::GetViewMatrix()
{
    return glm::lookat(camera_pos, target, world_up);
}

void TPSCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{

}


void TPSCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch /*= true*/)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    fmod(yaw, 360.0f);

    updateCameraVectors();
}

void TPSCamera::ProcessMouseScroll(float yoffset)
{
    if (distance >= 1.0f && distance <= 45.0f)
        distance -= yoffset;
    if (distance <= 1.0f)
        distance = 1.0f;
    if (distance >= 45.0f)
        distance = 45.0f;
    updateCameraVectors();
}

void TPSCamera::updateCameraVectors()
{
    float height = distance * sin(glm::radians(pitch));
    float horizon = distance * cos(glm::radians(pitch));

    camera_pos.y = target.y + height;
    camera_pos.x = target.x + horizon * sin(glm::radians(yaw));
    camera_pos.z = target.z + horizon * cos(glm::radians(yaw));
}

