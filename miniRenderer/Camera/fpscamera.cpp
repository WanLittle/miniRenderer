#include "fpscamera.h"

glm::Matrix FPSCamera::GetViewMatrix()
{
    return glm::lookat(camera_pos, camera_pos + camera_forward, world_up);
}

void FPSCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        camera_pos = camera_pos + camera_forward * velocity;
    if (direction == BACKWARD)
        camera_pos = camera_pos - camera_forward * velocity;
    if (direction == LEFT)
        camera_pos = camera_pos - camera_right * velocity;
    if (direction == RIGHT)
        camera_pos = camera_pos + camera_right * velocity;
}

void FPSCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
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

    updateCameraVectors();
}

void FPSCamera::ProcessMouseScroll(float yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}

void FPSCamera::lookToPos(glm::vec3f lookPos)
{
    camera_forward = lookPos - camera_pos;
    updateCameraEulerAngles();
    updateCameraVectors();
}

void FPSCamera::updateCameraVectors()
{
    glm::vec3f front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_forward = front.normalize();

    camera_right = glm::normalize(glm::cross(camera_forward, world_up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_forward));
}

void FPSCamera::updateCameraEulerAngles()
{
    float radPitch, radYaw;
    camera_forward = glm::normalize(camera_forward);
    float y = camera_forward.y;
    radPitch = asin(y);
    float cosRadP = cos(radPitch);
    float cosY = camera_forward.x / cosRadP;
    float sinY = camera_forward.z / cosRadP;
    if (sinY >= 0) 
    {
        radYaw = acos(cosY);
    }
    else 
    {
        radYaw = -acos(cosY);
    }
    yaw = glm::degrees(radYaw);
    pitch = glm::degrees(radPitch);
    camera_right = glm::normalize(glm::cross(camera_forward, world_up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_forward));
}
