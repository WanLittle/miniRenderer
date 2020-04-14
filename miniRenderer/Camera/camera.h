#pragma once

#include "../Math/vec.h"
#include "../Math/mat.h"
#include "../Math/transform.h"

//��������˶��ļ��ֿ���ѡ���������Զ�봰��ϵͳ�ض������뷽��
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float SPEED = 2.5f;       //Ĭ�ϵ��ٶ�
const float SENSITIVITY = 0.1f; //Ĭ�ϵ�������
const float ZOOM = 45.0f;       //Ĭ�ϵ��ӽ�

class Camera
{
protected:
    glm::Vec3f world_up;        //����y�᷽��

    glm::Vec3f camera_pos;      //����ͷλ��
    glm::Vec3f camera_up;       //����ͷ���Ϸ���
    glm::Vec3f camera_forward;  //����ͷ�ĳ���
    glm::Vec3f camera_right;    //����ͷ���ҷ���

    float yaw;                  //ƫ����
    float pitch;                //������

    float MovementSpeed;
    float MouseSensitivity;
    float zoom;                 // �����fov
public:
    Camera(glm::Vec3f _world_up, float _yaw, float _pitch) 
        : world_up(_world_up), yaw(_yaw), pitch(_pitch),
        MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM) {}
    virtual ~Camera(){}

    // ��ȡ�������View����
    virtual glm::Matrix GetViewMatrix() = 0;

    // ���̰���
    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;
    // ����ƶ�
    virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;
    // ��������
    virtual void ProcessMouseScroll(float yoffset) = 0;

    float getZoom() { return zoom; }
    void setCameraPos(glm::Vec3f _camera_pos) 
    {
        camera_pos = _camera_pos;
    }

protected:
    void updateCameraVectors();
};
