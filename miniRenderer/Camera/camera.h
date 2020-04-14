#pragma once

#include "../Math/vec.h"
#include "../Math/mat.h"
#include "../Math/transform.h"

//定义相机运动的几种可能选项。用作抽象，远离窗口系统特定的输入方法
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float SPEED = 2.5f;       //默认的速度
const float SENSITIVITY = 0.1f; //默认的灵敏度
const float ZOOM = 45.0f;       //默认的视角

class Camera
{
protected:
    glm::Vec3f world_up;        //世界y轴方向

    glm::Vec3f camera_pos;      //摄像头位置
    glm::Vec3f camera_up;       //摄像头的上方向
    glm::Vec3f camera_forward;  //摄像头的朝向
    glm::Vec3f camera_right;    //摄像头的右方向

    float yaw;                  //偏航角
    float pitch;                //俯仰角

    float MovementSpeed;
    float MouseSensitivity;
    float zoom;                 // 摄像机fov
public:
    Camera(glm::Vec3f _world_up, float _yaw, float _pitch) 
        : world_up(_world_up), yaw(_yaw), pitch(_pitch),
        MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM) {}
    virtual ~Camera(){}

    // 获取摄像机的View矩阵
    virtual glm::Matrix GetViewMatrix() = 0;

    // 键盘按键
    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;
    // 鼠标移动
    virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;
    // 滚轮缩放
    virtual void ProcessMouseScroll(float yoffset) = 0;

    float getZoom() { return zoom; }
    void setCameraPos(glm::Vec3f _camera_pos) 
    {
        camera_pos = _camera_pos;
    }

protected:
    void updateCameraVectors();
};
